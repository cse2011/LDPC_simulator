/******************************************************************
* This class models the Binary Symmetric communication channel    *
* which represents the process of storing data into a SLC memory  *
******************************************************************/

#ifndef __BSC_H__
#define __BSC_H__

/* Binary symmetric channel */

#include <stdlib.h>
#include <time.h>
#include "channel.h"

#define DEFAULT_P 0.0001


class bsc : public channel
{
	private:

	/* crossover probability */
	double p;

	public:

	/* const/destr */
	bsc(void);
	bsc(double p);
	~bsc(void);

	/* methods */
	double like_ratio(double x);
	channel_data transport(word input);

};


#endif
