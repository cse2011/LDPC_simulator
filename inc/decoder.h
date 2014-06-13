/******************************************************************
* This abstract class models a generic LDPC decoder. It takes as  *
* input a codeword (after error correction) and generates a word. *
******************************************************************/
#ifndef __DECODER_H__
#define __DECODER_H__

#include <iostream>
#include <stdlib.h>
#include "word.h"
#include "pcheck_mat.h"

class decoder {

	/* interface */
	public:
	virtual word decode(word& in) = 0;
	virtual ~decoder(void){};

};


#endif
