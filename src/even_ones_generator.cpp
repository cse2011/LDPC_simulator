#include "even_ones_generator.h"

/******************************************************************
* Constructor of even_ones_generator class.                          *
******************************************************************/
even_ones_generator::even_ones_generator(int n, int k, int ones){

	/* Initializing data structures */
	this->n = n;
	this->k = k;
	this->ones = ones;
}

/******************************************************************
* Destructor of even_ones_generator class.                           *
******************************************************************/
even_ones_generator::~even_ones_generator(void){}


/******************************************************************
* Generate matrix H.                                              *
******************************************************************/
int even_ones_generator::generate(pcheck_mat& H){

	if((H.get_n() != n) || (H.get_k() != k)){

		cout<<"Error: wrong dimensions."<<endl;
		return -1;
	}

	/* FIRST STEP: inserting ones randomly into H */
	ones_insertion(H);

	/* SECOND STEP: checking no rows without ones inside are
	 * present
	 */
	rows_without_ones_check(H);

	/* THIRD STEP: checking H is not composed by all columns with
	 * even number of ones.
	 */
	even_columns_check(H);

	/* FOURTH STEP: try to avoid situations where a pair of columns both have
	 * ones in a particular pair of rows. (Cycles of length four)
	 */
	cycles_detection(H);

	return 0;

}

/******************************************************************
* Functions to provide support for matrix H generation.           *
******************************************************************/
void even_ones_generator::ones_insertion(pcheck_mat& H)
{
	for(int i=0; i<n; i++){
		/* Select how many ones insert in each column */
		/* XXX: How do we select number of ones for each column? */
		int num_ones = ones;
		int j = 0;
		while(j < num_ones){
			/* Select position inside column where insert one */
			int index = rand() % (n-k);

			/* Check if a one is already present */
			if(H.get(index,i) == false){
				H.set(true,index,i);
				j++;
			}
		}
	}
}

void even_ones_generator::rows_without_ones_check(pcheck_mat& H)
{
	int cnt,index;

	for(int i=0; i<n-k; i++){
		cnt = 0;

		/* Count ones in each rows */
		for(int j=0; j<n; j++){
			if(H.get(i,j) == true){
				cnt++;
			}
		}

		/* Fill in row until at least two ones are present */
		while(cnt < 2){
			/* Select position inside row where insert one */
			index = rand() % n;

			/* Check if a one is already present */
			if(H.get(i,index) == false){
				H.set(true,i,index);
				cnt++;
			}
		}
	}
}

void even_ones_generator::even_columns_check(pcheck_mat& H)
{
	int cnt;
	bool even = true;

	for(int j=0; j<n && even == true; j++){
		cnt = 0;

		/* Count number of ones in each column */
		for(int i=0; i<n-k; i++){
			if(H.get(i,j) == true){
				cnt++;
			}

		}

		/* If number of ones is not even, exit */
		if( (cnt % 2) != 0){
			even = false;
		}
	}

	/* If H contains just columns with even number of ones add one 1 randomly */
	if(even){
		bool added = false;
		do{
			int i = rand() % (n-k);
			int j = rand() % n;

			if(H.get(i,j) == false){
				H.set(true,i,j);
				added = true;
			}
		}while(added == false);
	}
}

void even_ones_generator::cycles_detection(pcheck_mat& H)
{
	int cnt, inner_cnt, row_index, col_index, move;
	bool out, added;
	cnt = 0;
	out = false;
	while((cnt < CYCLE_REM_ITERATIONS) && (out == false)){
		out = true;
		/* Pair columns comparison loop */
		for(int i=0; i<n-1; i++){
			for(int j=i+1; j<n; j++){

				/* Pair rows comparison loop */
				for(int c=0; c<n-k-1;c++){
					for(int d=c+1; d<n-k; d++){

						/* If cycle of length four detected, move one of 1s involved */
						if(H.get(c,i) == true && H.get(c,j) == true
						&& H.get(d,i) == true && H.get(d,j) == true){

							out = false;
							added = false;
							inner_cnt = 0;
							do{
								/* Choosing which one to move */
								row_index = ((rand() % 2) == 0) ? c : d;
								col_index = ((rand() % 2) == 0) ? i : j;

								/* Choosing position into column where to move one */
								move = rand() % (n-k);
								if(H.get(move,col_index) == false){
									H.set(true,move,col_index);
									H.set(false,row_index,col_index);
									added = true;
								}

								/* If all positions are full skip */
								inner_cnt++;

							}while(added == false && inner_cnt < CYCLE_REM_ITERATIONS);

						}
					}
				}
			}
		}
		cnt++;
	}
}
