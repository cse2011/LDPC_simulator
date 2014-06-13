#include "systematic_encoder.h"

/******************************************************************
* Constructor                                                     *
******************************************************************/
systematic_encoder::systematic_encoder(pcheck_mat& H)
{
	n = H.get_n();
	k = H.get_k();
	alloc_space();

	/* make a copy matrix H for gauss reduce algorithm */
	pcheck_mat_dense H2(H);

	gauss_reduce(H,H2);

	/* copy matrix P in the correct position and fill the rest with I(k) */
	for(int i = 0; i < k; i++){
		/* copy P */
		for(int j = 0; j < n - k; j++){
			G[i][j] = H2.get(j, i + (n - k));
		}

		/* Identity */
		for(int j = n - k; j < n; j++){
			if(j - (n - k) == i) G[i][j] = true;
			else G[i][j] = false;
		}
	}

	/* Check that product H * G^t is zero */
	/*
	cout<<"Product matrix: "<<endl;
	bool prod[n-k][k];
	for(int i = 0; i < n - k; i++){
		for(int j = 0; j < k; j++){
			prod[i][j] = false;
			for(int l = 0; l < n; l++){
				prod[i][j] = prod[i][j] != (H.get(i,l) && G[j][l]);
			}
			cout<<(prod[i][j] ? "1 " : "0 ");
		}
		cout<<endl;
	}
	*/
}

/******************************************************************
* Destructor                                                      *
******************************************************************/
systematic_encoder::~systematic_encoder(void)
{
	free_space();
}

/******************************************************************
* Encoding function                                               *
******************************************************************/
word systematic_encoder::encode(word& in){
	word code(n);

	if(in.length() != k ) {
		code.set_err();
		return code;
	}

	for(int i = 0; i < n; i++){
		bool elm = false;
		for(int j = 0; j < k; j++){
			/* next line is a matrix product in boolean algebra. != is the
			XOR operator for bool variables, while && is the AND operator
			(multiplication). The product performed is (in * G) */
			elm = (elm != (in.get(j) && G[j][i]));
		}
		code.set(elm, i);
	}

	return code;
}

/******************************************************************
* Function to print matrix G (specific to this implementation of  *
* the encoder)                                                    *
******************************************************************/
void systematic_encoder::print_g(ostream& os){
	for(int i = 0; i < k; i++){
		for(int j = 0; j < n; j++){
			os<<G[i][j]<<" ";
		}
		os<<std::endl;
	}
}

/******************************************************************
* Auxiliary functions                                             *
******************************************************************/
void systematic_encoder::alloc_space(void){

	/* G is a matrix k*n */
	G = new bool*[k];

	for(int i = 0; i < k; i++ ){
		G[i] = new bool[n];
	}

}

void systematic_encoder::free_space(void){

	for(int i = 0; i < k; i++ ){
		delete [] G[i];
	}

	delete [] G;

}

/******************************************************************
* This function performs gauss elimination to reduce matrix H to  *
* the form [I(n-k) | P] and saves in the input parameter H the    *
* original parity check matrix with the columns reordered as      *
* required by gaussian elimination. It is a preliminary step to   *
* the generation of matrix G.
******************************************************************/
void systematic_encoder::gauss_reduce(pcheck_mat& H, pcheck_mat& H2)
{
	int i,j,r, last;
	bool tmp_elem;

	/* gaussian elimination performed on H2 but exchanging also
	 * rows of H.
	 */

	r = 0;
	last = n - k;

	/* iterate over rows */
	while(1){

		while(r < last) {

			/* find the first "1" in this row after position r */
			for(i = r; i < n; i++){
				if(H2.get(r,i) == true){
					break;
				}
			}

			/* row has no 1s after position r: exchange with another one */
			if(i == n){

				last= last - 1;

				/* swap rows */
				for(j = 0; j < n; j++){
					tmp_elem = H2.get(last,j);
					H2.set(H2.get(r,j), last,j);
					H2.set(tmp_elem, r, j);
				}

			/* row has at least a 1: go on */
			}else{
				break;
			}
		}

		/* break if we reached the last row */
		if( r == last) break;

		/* swap column r and i to put a pivot in row r.
		 * This must be done both on H2 and H */
		for(j = 0; j < n - k; j++){

			tmp_elem = H2.get(j,i);
			H2.set(H2.get(j,r),j,i);
			H2.set(tmp_elem, j, r);

			tmp_elem = H.get(j,i);
			H.set(H.get(j,r),j,i);
			H.set(tmp_elem, j, r);
		}

		/* for all rows (except pivot) */
		for(i = 0; i < n - k; i++){

			/* if the r-th column contains "1": subtract */
			if( (i != r) && (H2.get(i,r) == true)){
				/* for all remaining elements in the current row */
				for(j = r; j < n; j++){
					/* note: bitwise subtraction is a XOR, like sum */
					tmp_elem = H2.get(i,j) ^H2.get(r,j);
					H2.set(tmp_elem, i, j);
				}
			}
		}

		r++;
	}
}



