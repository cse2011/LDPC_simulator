#include "word.h"

/******************************************************************
* Constructors of word class.                                     *
******************************************************************/

/* create an empty word */
word::word(int len){
	this->len = len;
	data = new bool[len];
	err = false;
}

/* create a word from an array of boolean values */
word::word(bool* data, int len){
	this->len = len;
	this->data = new bool[len];

	for(int i = 0; i < len; i++){
		this->data[i] = data[i];
	}

	err = false;
}

/* create a word from an integer */
word::word(long long int val, int len){
	this->len = len;
	this->data = new bool[len];
	this->err = false;

	long long int mask = 0x1;
	for(int i = 0; i < len; i++){
		/* extract corresponding bit */
		unsigned long long int bit = val & mask;
		if(bit != 0) this->data[len - 1 - i] = true;
		else this->data[len - 1 - i] = false;
		mask = mask<<1;
	}
}

/* create word from a string of "bits" (e.g. "1001010") */
word::word(std::string str){
	len = str.length();
	data = new bool[len];
	err = false;	

	for(int i = 0; i < len; i++){
		char c = str.at(i);
		if(c =='1') data[i] = true;
		else if(c=='0') data[i] = false;			
		else {
			err = true;
			return;
		}
	}

}

/* create a word from a channel data rounding to integer */
word::word(channel_data data){
	this->len = data.length();
	this->data = new bool[len];
	this->err = false;

	for(int i = 0; i < len; i++){
		double d = data.get(i);
		int di = (int) d;
		if(di == 1) this->data[i] = true;
		else if(di == 0) this->data[i] = false;
		else{
			this->err = true;
			return;
		}
	}
}


/******************************************************************
* Copy constructors of word class.                                *
******************************************************************/
word::word(const word& rhs){
	len = rhs.len;
	data = new bool[len];            
	for(int i = 0; i < len; i++){
		data[i] = rhs.data[i];
	}
	err = rhs.err;
}

/******************************************************************
* Destructor of word class.                                       *
******************************************************************/
word::~word(){
	delete [] data;	
}

/******************************************************************
* Assignment operator of word class.                              *
******************************************************************/
word& word::operator = (const word rhs) {
	len = rhs.len;
	data = new bool[len];		
	for(int i = 0; i < len; i++){
		data[i] = rhs.data[i];
	}
	err = rhs.err;
	return *this;
}

/******************************************************************
* Member functions of word class.                                 *
******************************************************************/
bool word::get(int i){
	if(i < len) return data[i];
	err = true;
	return false;	
}

void word::set(bool v, int i){
	if(i < len) data[i] = v;
	else err = true;
}

int word::length(void) const{
	return len;
}

bool word::get_err(void) const{
	return err;
}

void word::set_err(void)
{
	err = true;
}

/******************************************************************
* Function to compute Hamming Distance between two words          *
******************************************************************/
int hamming_distance(word& w1, word& w2)
{
	int hd = 0;
	if(w1.length()!= w2.length()){
		return -1;
	}

	for(int i = 0; i < w1.length(); i++){
		if(w1.get(i) != w2.get(i)) hd++;
	}

	return hd;

}

/******************************************************************
* Stream Print operator of word class.                            *
******************************************************************/
std::ostream& operator << (std::ostream& os, const word& v) {

	for(int i =0; i < v.len; i++){
		if(v.data[i]) os<<"1";
		else os<<"0";
	}
	return os;
}
