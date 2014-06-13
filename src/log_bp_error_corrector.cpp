#include "log_bp_error_corrector.h"

/******************************************************************
* Constructor/destructor                                          *
******************************************************************/
log_bp_error_corrector::log_bp_error_corrector(channel& chn, pcheck_mat& H, int imax)
	: bp_error_corrector(chn,H,imax)
{}

/* destructor */
log_bp_error_corrector::~log_bp_error_corrector(void){};

/******************************************************************
* It implements error correction using standard Log_Domain Belief *
* Propagation algorithm, described in chapter 11.5.4 of "Inside   *
* Solid State Drives" book.                                       *
******************************************************************/
word log_bp_error_corrector::correct(channel_data& in)
{
	word c(n);

	if(in.length() != n){
		c.set_err();
		return c;
	}

	double *W = new double[n];
	double **R = new double*[n];
	double **M = new double*[n];
	for(int i = 0; i < n; i++){
		R[i] = new double[n - k]();
		M[i] = new double[n - k]();

		/* logarithmic channel likelihood */
		W[i] = log(chn.like_ratio(in.get(i)));
	}


	/* initialization step */
	for(int i = 0; i < n; i++){
		std::list<edge> edges = var_nodes[i].get_edges();
		std::list<edge>::iterator it;

		for(it = edges.begin(); it != edges.end(); it++){
			int j = (*it).get_dest().get_idx();
			R[i][j] = W[i];
		}
	}


	int idx = 0;
	bool iscodeword = false;

	while((idx < imax) && (iscodeword == false)){

		/* horizontal step */
		for(int j = 0; j < n - k; j++){
			std::list<edge> edges = chk_nodes[j].get_edges();
			std::list<edge>::iterator it;

			for(it = edges.begin(); it != edges.end(); it++){

				int i = (*it).get_dest().get_idx();
				M[i][j] = sgn(R,i,j) * phi(phi(R,i,j));

			}
		}


		/* vertical step */
		for(int i = 0; i < n; i++){
			std::list<edge> edges = var_nodes[i].get_edges();
			std::list<edge>::iterator it;

			for(it = edges.begin(); it != edges.end(); it++){
				int j = (*it).get_dest().get_idx();
				R[i][j] = vertical(M,W[i],i,j);
			}
		}

		/* hard decision and stopping criterion step */
		for(int i = 0; i < n; i++){
			double l = like_ratio(M,W[i],i);

			if(l >= 0) c.set(false, i);
			else c.set(true, i);
		}

		/* check if the current word is a codeword */
		iscodeword = H.is_codeword(c);

		idx++;
	}

	/* free memory */
	for(int i = 0; i < n - k; i++){
		delete [] R[i];
		delete [] M[i];
	}
	delete [] R;
	delete [] M;

	if(iscodeword == false){
		c.set_err();
	}

	return c;

}

/******************************************************************
* this function is redefined because it has a different           *
* implementation with respect to the standard BP algorithm        *
*******************************************************************/
void log_bp_error_corrector::compute_res(double& res, double& m)
{
	res += m;
}

/******************************************************************
* It computes the product of a series of values returned by sign  *
* function. It's used to perform Horizontal step of the           *
* Log-Domain Belief Propagation Decoding algorithm, formula 11.25 *
* of "Inside Solid State Drives" book.                            *
******************************************************************/
double log_bp_error_corrector::sgn(double** R,int i, int j)
{
	double result;

	result = 1.0;

	std::list<edge> edges = chk_nodes[j].get_edges();
	std::list<edge>::iterator it;

	for(it = edges.begin(); it != edges.end(); it++){

		int l = (*it).get_dest().get_idx();

		if(l != i){
			if(R[l][j] < 0){
				result = -result;
			}
		}
	}

	return result;
}

/******************************************************************
* It computes the phi function of a single value, depicted in     *
* figure 11.15 of the "Inside Solid State Drives" book.           *
******************************************************************/
double log_bp_error_corrector::phi(double x)
{
	if(x > 0){
		return ( -log(tanh(x/2)) );
	}
	else{
		return 0.0;
	}
}

/******************************************************************
* It computes a sum of a series of values, as needed by formula   *
* 11.25 of "Inside Solid State Drives" book. It helps performing  *
* Horizontal step of Log-Domain Belief Propagation algorithm.     *
******************************************************************/
double log_bp_error_corrector::phi(double** R, int i, int j)
{
	double result;

	result = 0;

	std::list<edge> edges = chk_nodes[j].get_edges();
	std::list<edge>::iterator it;

	for(it = edges.begin(); it != edges.end(); it++){

		int l = (*it).get_dest().get_idx();

		if(l != i){
			result += phi(abs(R[l][j]));
		}
	}

	return result;
}

