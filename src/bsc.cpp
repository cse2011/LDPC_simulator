#include "bsc.h"


/******************************************************************
* Constructors/Destructor                                         *
******************************************************************/
bsc::bsc(void)
{
	p = DEFAULT_P; 
}

bsc::bsc(double p)
{
	this->p = p;
}

bsc::~bsc(void){}


/******************************************************************
* This function computes the a priori Likelihood ratio of the BSC *
* channel. ("Inside Solid State Drives", formula 1.18)            *
******************************************************************/
double bsc::like_ratio(double x)
{

	/* OPPOSITE W.R.T. book */
	if(x == 0){ 
	/* quantized as +1 */
		return ((1 - p)/p);
	}else{
	/* quantized as -1 */
		return (p/(1 - p));
	}
}


/******************************************************************
* According to crossover probability of the channel, this         *
* function simulates the behavior or the BSC by flipping randomly *
* some bits of the word to be transmitted.                        *
******************************************************************/
channel_data bsc::transport(word input)
{

	channel_data output (input.length());

	for(int i = 0; i < input.length(); i++){
	
		double prob = (((double)rand())/RAND_MAX);
		
		if(input.get(i) == true){
			if(prob < p){
				output.set(0.0, i);
			}else{
				output.set(1.0, i);
			}
		}else{
			if(prob < p){
				output.set(1.0, i);	
			}else{
				output.set(0.0, i);
			}
		}

	}

	return output;
}
