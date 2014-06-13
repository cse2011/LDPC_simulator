/******************************************************************
* This class stores the output of the memory (channel). It is     *
* used as input to the decoding algorithm. Since some memory      *
* equivalent channel models can use more than two values (e.g.    *
* those for MLC flashes) the data stored by this class is of type *
* double.                                                         *
******************************************************************/
#ifndef __CHANNEL_DATA_H__
#define __CHANNEL_DATA_H__

#include <iostream>

class channel_data
{
	double* data;
	int len;

	public:

	/* constructors */
	channel_data(int len);
	channel_data(double* data, int len);

	/* copy constructor */
	channel_data(const channel_data& rhs);

	/* destructor */
	~channel_data();

	/* assignment operator */
	channel_data& operator = (const channel_data rhs);
	
	/* stream print operator */
	friend std::ostream& operator << (std::ostream& os, const channel_data& v);

	/* methods */
	void set(double v, int i);
	double get(int i) const;
	int length(void) const;

};

#endif
