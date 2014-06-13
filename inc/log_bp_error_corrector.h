/******************************************************************
* Error corrector that uses the logarithmic Belief Propagation    *
* algorithm.                                                      *
******************************************************************/

#ifndef __LOG_BP_ERROR_CORRECTOR_H_
#define __LOG_BP_ERROR_CORRECTOR_H_


#include "bp_error_corrector.h"
#include <cmath>

/* inherits from bp_error_corrector because they share most
 * of the code */
class log_bp_error_corrector : public bp_error_corrector {

	protected:

	/* redefined and new private functions */
	void compute_res(double& res, double& m);
	double sgn(double** R,int i,int j);
	double phi(double x);
	double phi(double** R, int i, int j);

	public:

	/* constructors and destructor */
	log_bp_error_corrector(channel& chn, pcheck_mat& H, int imax);
	~log_bp_error_corrector(void);

	/* redefined correction function */
	word correct(channel_data& in);

};

#endif
