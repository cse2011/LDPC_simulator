#include "bp_error_corrector.h"

/******************************************************************
* Constructor                                                     *
******************************************************************/
bp_error_corrector::bp_error_corrector(channel& chn, pcheck_mat& H, int imax)
	: H(H)
	, chn(chn)
{
	this->imax = imax;
	this->n = H.get_n();
	this->k = H.get_k();
	var_nodes = new node[n];
	chk_nodes = new node[n - k];
	build_graph();
}

/******************************************************************
* Destructor                                                      *
******************************************************************/
bp_error_corrector::~bp_error_corrector(void){};


/******************************************************************
* This function performs error correction using standard Belief   *
* Propagation algorithm, described in chapter 11.5 of "Inside     *
* Solid State Drives" book.                                       *
******************************************************************/
word bp_error_corrector::correct(channel_data& in)
{
	word c(n);

	if(in.length() != n){
		c.set_err();
		return c;
	}

	/* build matrices of "messages" r and m plus
	 * auxiliary matrix w
	 */
	double *w = new double[n];
	double **r = new double*[n];
	double **m = new double*[n];
	for(int i = 0; i < n; i++){
		r[i] = new double[n - k]();
		m[i] = new double[n - k]();

		/* channel likelihood ratio */
		w[i] = chn.like_ratio(in.get(i));
	}


	/* initialization step */
	for(int i = 0; i < n; i++){

		std::list<edge> edges = var_nodes[i].get_edges();
		std::list<edge>::iterator it;

		for(it = edges.begin(); it != edges.end(); it++){

			int j = (*it).get_dest().get_idx();
			r[i][j] = w[i];

		}
	}

	/* iteration in decoding */
	int idx = 0;

	/* true if a codeword has been found */
	bool iscodeword = false;

	while((idx < imax) && (iscodeword == false)){

		/* horizontal step */
		for(int j = 0; j < n - k; j++){

			std::list<edge> edges = chk_nodes[j].get_edges();
			std::list<edge>::iterator it;

			for(it = edges.begin(); it != edges.end(); it++){

				int i = (*it).get_dest().get_idx();
				m[i][j] = horizontal(r,i,j);

			}
		}


		/* vertical step */
		for(int i = 0; i < n; i++){

			std::list<edge> edges = var_nodes[i].get_edges();
			std::list<edge>::iterator it;

			for(it = edges.begin(); it != edges.end(); it++){

				int j = (*it).get_dest().get_idx();
				r[i][j] = vertical(m,w[i],i,j);

			}
		}


		/* hard decision and stopping criterion step */
		for(int i = 0; i < n; i++){
			double l = like_ratio(m,w[i],i);

			if(l >= 1) c.set(false, i);
			else c.set(true, i);
		}

		/* check if the current word is a codeword */
		iscodeword = H.is_codeword(c);

		idx++;
	}

	/* free memory */
	for(int i = 0; i < n - k; i++){
		delete [] r[i];
		delete [] m[i];
	}
	delete [] r;
	delete [] m;

	if(iscodeword == false){
		c.set_err();
	}

	return c;
}

/******************************************************************
* This function performs the horizontal step of the Belief        *
* Propagation algorithm, formula 11.20 of "Inside Solid State     *
* Drives"                                                         *
******************************************************************/
double bp_error_corrector::horizontal(double** r, int i, int j)
{
	double prod;

	prod = 1;

	std::list<edge> edges = chk_nodes[j].get_edges();
	std::list<edge>::iterator it;

	for(it = edges.begin(); it != edges.end(); it++){

		int l = (*it).get_dest().get_idx();

		if(l != i ){
			prod *= (r[l][j] + 1)/(r[l][j] - 1);
		}

	}

	return (prod + 1.0)/(prod - 1.0);
}

/******************************************************************
* This function performs the vertical step of the Belief          *
* Propagation algorithm, formula 11.21 of "Inside Solid State     *
* Drives" book.                                                   *
******************************************************************/
double bp_error_corrector::vertical(double** m, double w, int i, int j)
{
	double res = w;

	std::list<edge> edges = var_nodes[i].get_edges();
	std::list<edge>::iterator it;

	for(it = edges.begin(); it != edges.end(); it++){

		int l = (*it).get_dest().get_idx();

		//exclude node i.
		if(l != j){
			compute_res(res, m[i][l]);
		}
	}

	return res;
}

/*****************************************************************
* this function is defined like this because it is different in  *
* the logarithmic version of the BP error correction algorithm   *
******************************************************************/
void bp_error_corrector::compute_res(double& res, double& m)
{
	res *= m;
}


/******************************************************************
* This function performs the hard decision step of the Belief     *
* Propagation algorithm, described in formula 11.22 of "Inside    *
* Solid State Drives" book.                                       *
******************************************************************/
double bp_error_corrector::like_ratio(double** m, double w, int i)
{
	double res = w;

	std::list<edge> edges = var_nodes[i].get_edges();
	std::list<edge>::iterator it;

	for(it = edges.begin(); it != edges.end(); it++){

		int l = (*it).get_dest().get_idx();
		compute_res(res,m[i][l]);
	}

	return res;
}

/******************************************************************
* This function builds the Tanner Graph of the LDPC code, given   *
* the parity check matrix H.                                      *
******************************************************************/
void bp_error_corrector::build_graph(void){

	/* check nodes */
	for(int i = 0; i < n - k; i++){

		chk_nodes[i].set_idx(i);
		chk_nodes[i].set_type(CHECK);
	}

	/* variable nodes */
	for(int i = 0; i < n; i++){
		var_nodes[i].set_idx(i);
		var_nodes[i].set_type(VARIABLE);
	}

	/* connections */
	for(int i = 0; i < n - k; i++){
		for(int j = 0; j < n; j++){
			if(H.get(i,j) == true){
				var_nodes[j].add_bidirectional_edge(
					chk_nodes[i]);
			}
		}
	}
}

/******************************************************************
* This function prints the topology of the Tanner graph.          *
******************************************************************/
void bp_error_corrector::print_graph(node_type t){
	if(t == VARIABLE){
		for(int i = 0; i < n; i++)
			std::cout<<var_nodes[i]<<std::endl;
	}else{
		for(int j = 0; j < n - k; j++)
			std::cout<<chk_nodes[j]<<std::endl;
	}
}

