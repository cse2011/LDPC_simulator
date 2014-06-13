/******************************************************************
* This abstract class provides an interface to access parity      *
* check data of an LDCP code. It is kept abstract because         *
* different implementations of this storage class can be          *
* coded, taking into account the fact that H is by definition a   *
* sparse matrix in an LDPC code.                                  *
******************************************************************/
#ifndef __PCHECK_MAT_H__
#define __PCHECK_MAT_H__

#include <iostream>
#include <stdlib.h>
#include <string>
#include "word.h"

using namespace std;

class pcheck_mat
{

	/* interface */
	public:
	virtual int load(char* filename) = 0;
	virtual int save(char* filename) = 0;
	virtual void set(bool v, int row, int col) = 0;
	virtual bool get(int row, int col) = 0;
	virtual int get_n(void) = 0;
	virtual int get_k(void) = 0;
	virtual bool is_codeword(word& w) = 0;
	virtual void print(ostream& os) const = 0;
	virtual ~pcheck_mat(void){};

	/* dummy print operator */
	friend ostream& operator << (ostream& os, const pcheck_mat& v) {
		/* calls print function */
		v.print(os);
		return os;
	}

};




#endif
