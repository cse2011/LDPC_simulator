/******************************************************************
* This abstract class provides functions for the generation of    *
* parity check matrix H for an LDPC code                          *
******************************************************************/
#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include <iostream>
#include <stdlib.h>
#include "pcheck_mat.h"

class generator {

	/* interface */
	public:
	virtual int generate(pcheck_mat& H) = 0;
	virtual ~generator(void){};

};


#endif
