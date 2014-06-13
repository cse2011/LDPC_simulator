/******************************************************************
* Main function of the LDPC Simulator project for the course on   *
* Reliable Embedded Systems Design.                               *
*                                                                 *
* Authors:                                                        *
*     Alessandro de Gennaro   (s194672)                           *
*     Daniele Jahier Pagliari (s196046)                           *
*                                                                 *
* Year:                                                           *
*     2014                                                        *
*                                                                 *
* University:                                                     *
*     Politecnico di Torino                                       *
*                                                                 *
******************************************************************/
#include <iostream> 
#include "channel.h"
#include "bsc.h"
#include "word.h"
#include "channel_data.h"
#include "pcheck_mat.h"
#include "pcheck_mat_dense.h"
#include "generator.h"
#include "even_ones_generator.h"
#include "encoder.h"
#include "systematic_encoder.h"
#include "error_corrector.h"
#include "bp_error_corrector.h"
#include "log_bp_error_corrector.h"
#include "decoder.h"
#include "systematic_decoder.h"
#include <iostream>
#include <fstream>

using namespace std;

/* Elements that compose our simulation system.
 * (there is also a code generator, local to main() */
pcheck_mat* mat;
encoder* enc;
channel* chn;
error_corrector* erc;
decoder* dcd;


void simulate(void);

int main(int argc, char *argv[])
{

	int choice;
	char filename[256];
	int flag;
	int ones;
	int k, n;
	generator* gen;
	
	/* random number generation seed */
	srand(time(NULL));

	/* main menu */
	flag = 0;
	while(!flag){
		cout<<"LDPC Simulator. De Gennaro/Jahier Pagliari, RESD Project, 2014"<<endl
			<<"Select operation:"<<endl
			<<"1) Generate new LDPC code"<<endl
			<<"2) Load H matrix from file"<<endl
			<<"3) Exit"<<endl
			<<"> ";
		cin >> choice;

		switch(choice){
		case 1:
		case 2:
			flag = 1;
			break;
		case 3:
			cout<<"Quitting..."<<endl;
			return 0;
			break;
		default:
			break;
		}
	}

	/* generate a new code */
	if(choice == 1){
		flag = 0;
		while(!flag){
			cout<<"Insert message length (k): ";
			cin>>k;
			cout<<"Insert code length (n) (must be > k): ";
			cin>>n;
			if(n<=k || k <=0){
				cout<<"Error: wrong parameters!"<<endl;
				continue;
			}

			/* this simulator only supports the "even ones method" */
			cout<<"Generating new code with the even ones method."<<endl
				<<"Select number of ones: ";
			cin>>ones;
			if(ones<=0){
				cout<<"Error: wrong number of ones"<<endl;
			}else{
				/* all parameters correct */
				flag = 1;
			}
		}

		/* create container for parity check data */
		mat = new pcheck_mat_dense(n,k);

		/* generate code */
		gen = new even_ones_generator(n,k,ones);
		gen->generate((*mat));
	}

	/* read existing parity check matrix H from file */
	if(choice == 2){
		cout<<"Insert filename: ";
		cin>>filename;
		mat = new pcheck_mat_dense();
		if(mat->load(filename) < 0){
			return -1;
		}
		cout<<"Data loaded correctly."<<endl;
	}

	/* create encoder for this code */
	/* Important: the systematic encoder may change the matrix H
	 * (exchanging some columns). This does not change the
	 * properties of the code, but it is important to remember
	 * that for the following.
	 */
	enc = new systematic_encoder(*mat);
	dcd = new systematic_decoder(*mat);

	/* print parity check matrix if requested */
	flag = 0;
	while(!flag){
		cout<<"Print the parity check matrix for the code? "<<endl
			<<"1) yes"<<endl
			<<"2) no"<<endl
			<<"> ";
		cin >> choice;

		switch(choice){
		case 1:
			/* print the obtained parity check matrix */
			cout<<"k = "<<mat->get_k()<<", n = "<<mat->get_n()<<endl;
			cout<<"Parity check matrix: "<<endl<<(*mat)<<endl;
			flag = 1;
			break;
		case 2:
			flag = 1;
			break;
		default:
			break;
		}
	}

	/* second menu */
	while(1){
		cout<<"Select operation: "<<endl
			<<"1) Start simulation"<<endl
			<<"2) Save parity check matrix to file"<<endl
			<<"3) Exit"<<endl
			<<"> ";
		cin >> choice;

		switch(choice){

		/* simulation */
		case 1:
			simulate();
			break;

		/* save */
		case 2:
			cout<<"Insert filename: ";
			cin>>filename;
			mat->save(filename);
			cout<<"Data saved correctly."<<endl;
			break;

		/* exit */
		case 3:
			cout<<"Quitting..."<<endl;
			return 0;
			break;
		default:
			break;
		}
	}

	return 0;

}

void simulate(void){

	char filename[256];
	ifstream sim_input;
	bool iscodeword;
	double trans_prob;
	int algo;
	int imax;
	int i;
	unsigned long long int word_value;
	int tmp_errors;
	int raw_errors, errors;
	double avg_raw, avg;

	/* create a memory equivalent channel model */
	trans_prob = 0;
	while((trans_prob<= 0) || (trans_prob>=1)){
		cout<<"Select BSC equivalent channel transition probability (between 0 and 1): ";
		cin >> trans_prob;
		if((trans_prob<=0)||(trans_prob>=1)){
			cout<<"Error: wrong transition probability!"<<endl;
		}else{
			chn = new bsc(trans_prob);
		}
	}	

	/* select an error correction algorithm */
	cout<<"Select error correction algorithm: "<<endl
		<<"1) Standard Belief Propagation"<<endl
		<<"2) Logarithmic Belief Propagation"<<endl
		<<"> ";
	cin >> algo;


	/* select number of iterations */
	cout<<"Select maximum number of iterations of the algorithm: ";
	cin>>imax;

	/* create the error corrector object (depending on the algorithm selected) */
	if(algo == 1){
		erc = new bp_error_corrector(*chn, *mat, imax);
	}else if(algo == 2){
		erc = new log_bp_error_corrector(*chn, *mat, imax);
	}else{
		cout<<"Wrong algorithm selection. Terminating simulation."<<endl;
		return;
	}

	/* get simulation inputs */
	cout<<"Select filename containing simulation inputs: ";
	cin >> hex >> filename;

	sim_input.open(filename);

	if(sim_input.fail()){
		cout<<"File not found. Terminating simulation."<<endl;
		return;
	}


	/* initialize simulation statistics */
	i = 0;
	raw_errors = 0;
	errors = 0;
	avg_raw = 0;
	avg = 0;

	while(sim_input >> hex>> word_value){
		cout<<"==================================================="<<endl
			<<"Simulation vector #"<<i<<endl;

		/* create a new word */
		word initial((long long int) word_value, mat->get_k());
		cout<<"Input word: "<<initial<<endl;

		/* encode it */
		word encoded = enc->encode(initial);
		cout<<"Encoded word: "<<encoded<<endl;

		/* commented out because answer is always yes, obviously */
		//iscodeword = mat->is_codeword(encoded);
		//cout<<"The encoded word is "<<
		//		(iscodeword? "" : "not ")<<"a codeword"<<endl;


		/* simulate effect of memory equivalent channel */
		channel_data mem_out = chn->transport(encoded);
		cout<<"Memory output: "<<mem_out<<endl;
		word mem_out_w(mem_out);

		/* count number of errors before error correction */
		tmp_errors = hamming_distance(encoded, mem_out_w);
		cout<<"Number of raw errors: "<<tmp_errors<<endl;
		raw_errors += tmp_errors;

		/* is the received word a codeword? */
		iscodeword = mat->is_codeword(mem_out_w);
		cout<<"The received word is "<<
				(iscodeword? "" : "not ")<<"a codeword"<<endl;


		/* perform error correction */
		word corrected = erc->correct(mem_out);
		cout<<"Output of error correction: "<<corrected<<endl;

		/* count number of errors after error correction */
		tmp_errors = hamming_distance(encoded, corrected);
		cout<<"Number of errors: "<<tmp_errors<<endl;
		errors += tmp_errors;

		/* is the corrected word a codeword? */
		iscodeword = mat->is_codeword(corrected);
		cout<<"The corrected word is "<<
				(iscodeword? "" : "not ")<<"a codeword"<<endl;

		/* decode the word (extract data bits) */
		word decoded = dcd->decode(corrected);
		cout<<"Decoded word: "<<decoded<<endl;

		i++;

	}

	/* compute simulation statistics */
	avg_raw = ((double) raw_errors) / ((double) i);
	avg = ((double) errors) / ((double) i);

	cout<<"==================================================="<<endl
		<<"Simulation statistics: "<<endl
		<<"Number of simulation vectors: "<<i<<endl
		<<"Channel error probability: "<<trans_prob<<endl
		<<"Number of raw errors: "<<raw_errors<<endl
		<<"Average raw errors per vector: "<<avg_raw<<endl
		<<"Number of errors after correction: "<<errors<<endl
		<<"Average errors per vector after correction: "<<avg<<endl;

	sim_input.close();
}
