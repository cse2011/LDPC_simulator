#include "channel_data.h"

/******************************************************************
* Constructors of the channel_data class.                         *
******************************************************************/
channel_data::channel_data(int len){	
	this->len = len;
	data = new double[len];
}

channel_data::channel_data(double* data, int len){
	this->len = len;
	this->data = new double[len];
	for(int i = 0; i < len; i++){
		this->data[i] = data[i];
	}
}

/******************************************************************
* Copy constructor of the channel_data class.                     *
******************************************************************/
channel_data::channel_data(const channel_data& rhs){
	len = rhs.length();
	data = new double[len];            
	for(int i = 0; i < len; i++){
		data[i] = rhs.get(i);
	}
}

/******************************************************************
* Destructor of the channel_data class.                           *
******************************************************************/
channel_data::~channel_data(){
	delete [] data;
}

/******************************************************************
* Assignment operator of the channel_data class.                  *
******************************************************************/
channel_data& channel_data::operator = (const channel_data rhs) {
        len = rhs.length();
        data = new double[len];
        for(int i = 0; i < len; i++){
                data[i] = rhs.get(i);
        }
        return *this;
}

/******************************************************************
* Stream print operator of the channel_data class.                *
******************************************************************/
std::ostream& operator << (std::ostream& os, const channel_data& v) {
	
	for(int i =0; i < v.len; i++){
		os <<v.data[i];
	}

	return os;
}

/******************************************************************
* Methods of the channel_data class.                              *
******************************************************************/
void channel_data::set(double v, int i){
	if(i < len) data[i] = v;
}

double channel_data::get(int i) const{
	if(i < len) return data[i];
	return 0;
}

int channel_data::length(void) const{
	return len;	
}

