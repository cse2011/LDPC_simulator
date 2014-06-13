#include "systematic_decoder.h"

/******************************************************************
* Constructor                                                     *
******************************************************************/
systematic_decoder::systematic_decoder(pcheck_mat& H)
{
	n = H.get_n();
	k = H.get_k();
}

/******************************************************************
* Destructor                                                      *
******************************************************************/
systematic_decoder::~systematic_decoder(void){}

/******************************************************************
* Member functions                                                *
******************************************************************/
word systematic_decoder::decode(word& in)
{
	/* since the code is (by construction) in systematic form, and
	 * the message bits are at the END of the codeword, this class
	 * does not really perform matrix multiplication between the
	 * codeword and matrix R, but simply extracts the first k bits
	 * from the codeword.
	 */
	word data(k);

	/* check input length */
	if(in.length()!= n){
		data.set_err();
		return data;
	}

	for(int i = n - k; i < n; i++){
		bool tmp = in.get(i);
		data.set(tmp,i - (n - k));
	}

	return data;

}

