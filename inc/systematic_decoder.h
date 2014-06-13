/******************************************************************
* This class provides the functions to decode any linear block    *
* code (with matrix H in systematic form) simply extracting some  *
* of the bits from the word (equivalent to matrix multiplication  *
* between the codeword and matrix R).                             *
******************************************************************/
#ifndef __SYSTEMATIC_DECODER_H_
#define __SYSTEMATIC_DECODER_H_

#include "decoder.h"

class systematic_decoder: public decoder {

	private:

	int n;
	int k;

	public:

	/* constructors */
	systematic_decoder(pcheck_mat& H);

	/* destructor */
	~systematic_decoder(void);

	/* member functions */
	word decode(word& in);

};


#endif
