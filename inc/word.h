/******************************************************************
* This class models a binary word, and implements all functions   *
* for accessing and setting it.                                   *
******************************************************************/
#ifndef __WORD_H__
#define __WORD_H__

#include <iostream>
#include <string>

#include "channel_data.h"

#define ZERO false
#define ONE true

class word
{
	bool *data;
	bool err;
	int len;

	public:
	
	/* constructors */
	word(int len);
	word(bool* data, int len);
	word(long long int val, int len);
	word(std::string str);
	word(channel_data data);

	/* copy constructor */
	word (const word& rhs);

	/* destructor */
	~word();

	/* assignment operator */
    word& operator = (const word rhs);
	
	/*members*/
	bool get(int i);
	void set(bool v, int i);
	int length(void) const;
	bool get_err(void) const;
	void set_err(void);

	/* hamming distance */
	friend int hamming_distance(word& w1, word& w2);

	/* stream print operator */
	friend std::ostream& operator << (std::ostream& os, const word& v);

};


#endif
