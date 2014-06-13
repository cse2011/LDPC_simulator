/******************************************************************
* This abstract class models a generic LDPC encoder. It takes as  *
* input a a word and generates the corresponding codeword.        *
******************************************************************/
#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <iostream>
#include <stdlib.h>
#include "word.h"

using namespace std;

class encoder{

	/* interface */
	public:
	virtual word encode(word& in) = 0;
	virtual ~encoder(void){};

};


#endif
