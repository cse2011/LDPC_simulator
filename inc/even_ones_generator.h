/******************************************************************
* This class provides functions for generation of a parity check  *
* matrix H for LDPC codes fixing the number of 1s in each column. *
******************************************************************/
#ifndef __EVEN_ONES_GENERATOR_H__
#define __EVEN_ONES_GENERATOR_H__

#include <iostream>
#include <stdlib.h>
#include "pcheck_mat.h"
#include "generator.h"

using namespace std;

#define CYCLE_REM_ITERATIONS 10

class even_ones_generator : public generator
{

	/* member variables */
	int n;
	int k;
	int ones;

	/* private auxiliary functions */
	void insert_identity(pcheck_mat& H);
	void ones_insertion(pcheck_mat& H);
	void even_columns_check(pcheck_mat& H);
	void rows_without_ones_check(pcheck_mat& H);
	void cycles_detection(pcheck_mat& H);

	public:
	
	/* constructors */
	even_ones_generator(int n, int k, int ones);

	/* destructor */
	~even_ones_generator(void);
	
	/* methods */
	int generate(pcheck_mat& H);

};


#endif
