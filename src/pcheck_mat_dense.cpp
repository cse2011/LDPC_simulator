#include "pcheck_mat_dense.h"

/******************************************************************
* Constructors                                                    *
******************************************************************/
pcheck_mat_dense::pcheck_mat_dense(void)
{
	H = NULL;
	n = 0;
	k = 0;
}

pcheck_mat_dense::pcheck_mat_dense(int n, int k)
{
	this->n = n;
	this->k = k;
	alloc_space();
}

/******************************************************************
* Copy constructor                                                *
******************************************************************/
pcheck_mat_dense::pcheck_mat_dense(pcheck_mat& rhs)
{
	this->n = rhs.get_n();
	this->k = rhs.get_k();
	alloc_space();

	/* copy data */
	for(int i = 0; i < (n - k); i++){
		for(int j = 0; j < n; j++){
			H[i][j] = rhs.get(i,j);
		}
	}

}

/******************************************************************
* Assignment operator                                             *
******************************************************************/
pcheck_mat_dense& pcheck_mat_dense::operator = (pcheck_mat& rhs)
{
	/* free previous content */
	if(H != NULL){
		free_space();
	}
	
	this->n = rhs.get_n();
	this->k = rhs.get_k();
	alloc_space();

	/* copy data */
	for(int i = 0; i < (n - k); i++){
		for(int j = 0; j < n; j++){
			H[i][j] = rhs.get(i,j);
		}
	}

	return *this;
}

/******************************************************************
* Destructor                                                      *
******************************************************************/
pcheck_mat_dense::~pcheck_mat_dense(void)
{
	/* free memory for matrix H */
	free_space();
	H = NULL;
}

/******************************************************************
* Stream Print operator                                           *
******************************************************************/
ostream& operator << (ostream& os, const pcheck_mat_dense& v) {

	/* calls print function */
	v.print(os);
	return os;
}

/******************************************************************
* Function to load matrix H from file                             *
******************************************************************/
int pcheck_mat_dense::load(char* filename)
{
	ifstream fh;
	int tmp;

	fh.open(filename);

	if(fh.fail()){
		cout<<"File not found."<<endl;
		return -1;
	}

	/* free previous content */
	if(H != NULL){
		free_space();
	}

	/* read k and n */
	fh >> tmp;
	if(tmp <= 0) {
		cout<<"Wrong value of k."<<endl;
		return -1;
	}
	k = tmp;

	fh >> tmp;
	if(tmp <= 0 || tmp<=k){
		cout<<"Wrong value of n."<<endl;
		return -1;
	}
	n = tmp;

	alloc_space();

	for(int i = 0; i < n - k; i++){
		for(int j = 0; j < n; j++){
			fh >> tmp;
			if(tmp==1) H[i][j] = true;
            else if(tmp==0) H[i][j] = false;
            else {
            	cout<<"Wrong matrix data."<<endl;
            	return -1;
            }
        }
    }
	fh.close();
	return 0;

}

/******************************************************************
* Function to save matrix H to file                               *
******************************************************************/
int pcheck_mat_dense::save(char* filename)
{
	ofstream fh;

	fh.open(filename);

	if(fh.fail()){
		cout<<"File not found."<<endl;
		return -1;
	}

	fh << k <<endl;
	fh << n <<endl;

	for(int i = 0; i < n - k; i++){
		for(int j = 0; j < n; j++){
			if(H[i][j] == true) fh << "1 ";
            else fh << "0 ";
        }
		fh <<endl;
    }

	fh.close();
	return 0;
}

/******************************************************************
* Setter and Getter for elements of the matrix                    *
******************************************************************/
void pcheck_mat_dense::set(bool v, int row, int col)
{
		H[row][col] = v;
}

bool pcheck_mat_dense::get(int row, int col){
	return H[row][col];
}

/******************************************************************
* Getters for size variables                                      *
******************************************************************/
int pcheck_mat_dense::get_n(void)
{
	return n;
}

int pcheck_mat_dense::get_k(void)
{
	return k;
}

/******************************************************************
* Function to check if a word is a codeword or not                *
******************************************************************/
bool pcheck_mat_dense::is_codeword(word& w)
{
	if(w.length()!= n){
		cout<<"Error: wrong word length"<<endl;
		return false;
	}

	/* perform multiplication w * H' */
	bool elm;
	for(int i = 0; i < n - k; i++){
		elm = false;
		for(int j = 0; j < n; j++){
			elm = (elm != (w.get(j) && H[i][j]));
		}
		if(elm == true) return false;
	}

	return true;
}

/******************************************************************
* Print function                                                  *
******************************************************************/
void pcheck_mat_dense::print(ostream& os) const
{
	for(int i =0; i < (n - k); i++){
		for(int j = 0; j < n; j++){
			if(H[i][j] == true) os<<"1 ";
			else os<<"0 ";
		}
		os<<endl;
	}
}

/******************************************************************
* Auxiliary functions                                             *
******************************************************************/

void pcheck_mat_dense::alloc_space(void)
{
	H = new bool*[n - k];
	for(int i = 0; i < (n - k); i++){
		H[i] = new bool[n];
	}
}


void pcheck_mat_dense::free_space(void)
{
	for(int i = 0; i < n - k; i++){
		delete [] H[i];
	}
	delete [] H;
}
