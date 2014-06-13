/******************************************************************
* This abstract class models a generic LDPC error correction      *
* algorithm. It takes as input data coming from a channel and     *
* generates the corresponding codeword according to some algorithm*
******************************************************************/
#ifndef __ERROR_CORRECTOR_H__
#define __ERROR_CORRECTOR_H__

#include <iostream>
#include <stdlib.h>

#include "word.h"
#include "channel_data.h"

class error_corrector {

	public:

	virtual word correct(channel_data& in) = 0;
	virtual ~error_corrector(void){};

};


#endif
