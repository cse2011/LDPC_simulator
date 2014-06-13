/******************************************************************
* This abstract class models a generic communication channel.     *
* It is used to keep the error correction algorithm agnostic of   *
* the channel model used for the memory.                          *
******************************************************************/
#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include "word.h" 
#include "channel_data.h"

class channel
{

	/* interface */
	public:
	virtual double like_ratio(double x) = 0;
	virtual channel_data transport(word input) = 0;
	virtual ~channel(void){};

};

#endif
