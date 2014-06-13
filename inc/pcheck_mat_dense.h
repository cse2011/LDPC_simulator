/******************************************************************
* Simple implementation of the parity check matrix interface      *
* that uses a dense representation (stores both 0s and 1s)        *
******************************************************************/
#ifndef __PCHECK_MAT_DENSE_H__
#define __PCHECK_MAT_DENSE_H__

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "pcheck_mat.h"


class pcheck_mat_dense: public pcheck_mat
{
	private:
	
	bool **H;
	int n;
	int k;

	/* private auxiliary methods */
	void alloc_space(void);
	void free_space(void);

	public:
	
	/* constructors */
	pcheck_mat_dense(void);
	pcheck_mat_dense(int n, int k);

	/* copy constructor */
	pcheck_mat_dense(pcheck_mat& rhs);

	/* assignment operator */
    pcheck_mat_dense& operator = (pcheck_mat& rhs);

	/* destructor */
	~pcheck_mat_dense(void);

	/* methods */
	int load(char* filename);
	int save(char* filename);
	void set(bool v, int row, int col);
	bool get(int row, int col);
	int get_n(void);
	int get_k(void);
	bool is_codeword(word& w);
	void print(ostream& os) const;

	/* stream print operator */
	friend std::ostream& operator << (std::ostream& os, const pcheck_mat_dense& v);

};


#endif
