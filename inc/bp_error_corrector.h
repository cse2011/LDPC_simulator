/******************************************************************
* Error corrector that uses the standard Belief Propagation       *
* algorithm.                                                      *
******************************************************************/

#ifndef __BP_ERROR_CORRECTOR_H_
#define __BP_ERROR_CORRECTOR_H_

#include "error_corrector.h"
#include "pcheck_mat.h"
#include "channel.h"
#include "graph.h"

class bp_error_corrector : public error_corrector {

	protected:
	int imax;
	int n;
	int k;
	pcheck_mat& H;
	channel& chn;
	node* var_nodes;
	node* chk_nodes;

	/* private auxiliary functions */
	void build_graph(void);
	double horizontal(double** r, int i, int j);
	double vertical(double** m, double w, int i, int j);
	double like_ratio(double** m, double w, int i);
	virtual void compute_res(double& res, double& m);


	public:

	/* constructors/destructors */
	bp_error_corrector(channel& chn, pcheck_mat& H, int imax);
	~bp_error_corrector(void);

	/* methods */
	virtual word correct(channel_data& in);
	void print_graph(node_type t);
};



#endif
