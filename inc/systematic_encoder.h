/******************************************************************
* This class provides functions to encode a word into a codeword  *
* for any linear block code using multiplication between the word *
* and the generator matrix G. The code will be in systematic form *
******************************************************************/

#ifndef __SYSTEMATIC_ENCODER_H_
#define __SYSTEMATIC_ENCODER_H_

#include "encoder.h"
#include "pcheck_mat.h"
#include "pcheck_mat_dense.h"

class systematic_encoder: public encoder {

	private:

	bool** G;
	int n;
	int k;
	int* pos;

	/* private auxiliary functions */
	void alloc_space(void);
	void free_space(void);
	void gauss_reduce(pcheck_mat& H, pcheck_mat& H2);

	public:

	/* constructors */
	systematic_encoder(pcheck_mat& H);

	/* destructor */
	~systematic_encoder(void);

	/* member functions */
	word encode(word& in);
	void print_g(ostream& os);

};



#endif
