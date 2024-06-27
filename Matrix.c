/****************************************************************************************
*  Andrew Dell'Aringa 2082835
*  PA4 02/07/24
*  Matrix.c contains more ADT's and declarations of fucntions
*****************************************************************************************/
#include "Matrix.h"

typedef struct EntryObj{
   int column;
   double value; 
}EntryObj;

typedef EntryObj* Entry;


typedef struct MatrixObj{
    List * Entries;
    int size;
    int nze;
} MatrixObj;

// Returns a reference to a new nXn Matrix object in the zero state.
Matrix newMatrix(int n){
	Matrix new = malloc(sizeof(MatrixObj));
	new->Entries = (List *) malloc((n+1) * sizeof(List));
	for(int i =1; i <= n; i++){
		new->Entries[i] = newList();
	}
	new->size = n;
	new->nze =0;
	return new;
}
// freeMatrix()
// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix* pM){
	if ( (*pM) == NULL) return;
	//free the elements in the list
	for(int i = 1; i <= (*pM)->size; i ++){
	
		moveFront((*pM)->Entries[i]);
		while(index((*pM)->Entries[i]) != -1){
			free(get((*pM)->Entries[i]));
			moveNext((*pM)->Entries[i]);
			deleteFront((*pM)->Entries[i]);
		}
		freeList(&(*pM)->Entries[i]);
		
	}
	//free the pointer to the list
	free((*pM)->Entries);
	//free the pointer to the matrix
	free((*pM));
	(*pM) = NULL;
	
}

// Access functions 
// size()
// Return the size of square Matrix M.
int size(Matrix M){
	if(M!=NULL){
		
		return M->size;
	}
	
	return -1;
}
// NNZ()
// Return the number of non-zero elements in M.
int NNZ(Matrix M){
	if(M != NULL){
		return M->nze;
	}
	return -1;
}



// equals()
// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B){
	//if either are null
	if(A == NULL || B == NULL) return 0; 
	//if the size is not the same
	if(size(A) != size(B)) return 0;
	//if the number of nonzero elements are not the same
	if(NNZ(A) != NNZ(B)) return 0;
	//traversing the rows
	for(int i = 1; i < size(A); i++){
		//if the lengths of the rows are not the same return false
		if(length(A->Entries[i]) != length(B->Entries[i])){
			return 0; 
		}
		//if the rows are nonempty
		if(length(A->Entries[i]) != 0){
			//move both A and B to the front of the rows
			moveFront(A->Entries[i]);
			moveFront(B->Entries[i]);
			//traverse the columns
			while(index(A->Entries[i]) != -1){
				Entry a = get(A->Entries[i]);
				Entry b = get(B->Entries[i]);
				//if either the value or the column are not the same return false
				if(a->column != b->column || a->value != b->value){
					return 0;
				}
				//look at the next element in the list
				moveNext(A->Entries[i]);
				moveNext(B->Entries[i]);
			}
		}
	}
	//at this point they are the same
	return 1;
	
}
// Manipulation procedures 
// makeZero()
// Re-sets M to the zero Matrix state.
void makeZero(Matrix M){
	for(int i = 1; i <= M->size; i ++){
		moveFront(M->Entries[i]);
		while(index(M->Entries[i]) != -1){
			free(get(M->Entries[i]));
			moveNext(M->Entries[i]);
			deleteFront(M->Entries[i]);
		}
		
	}
	M->nze = 0;
}



// changeEntry()
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x){
	//preconditions
	if(M == NULL) return;
	
	if(1 <= i && i <= size(M) && 1 <= j && j <= size(M)){
		Entry newEntry = malloc(sizeof(Entry));
		newEntry->column = j;
		newEntry->value = x;
		//if the list is empty just append
		if(length(M->Entries[i]) == 0 && x != 0){
			
			append(M->Entries[i], newEntry);
			M->nze += 1;
			return;
		}
		//-----------------the list is not empty
		
		//move the cursor to the front of the list
		moveFront(M->Entries[i]);
		//traverse the columns
		while(index(M->Entries[i]) != -1){
			Entry temp = get(M->Entries[i]);
			if(j == temp->column){
				if(x == 0){
					free(get(M->Entries[i]));
					M->nze-=1;
					return;
				}
				temp->value = x;
				return;
			}
			if(j < temp->column && x != 0.0){
				//j is less than the column we are looking at but larger than the previous
				//insert before the cursor and return
				
				insertBefore(M->Entries[i], newEntry);
				M->nze += 1;
				return;
			}
			//j is larger than the column we are looking at go to the next column
			moveNext(M->Entries[i]);
		}
		//j is the larges nz column so append it to the end of the list
		
		if(x != 0){
			append(M->Entries[i], newEntry);
			M->nze += 1;
		}
	}
}



// Matrix Arithmetic operations 
// copy()
// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A){
	
	if(A == NULL){
		return NULL;
	}
	//Allocate the new matrix
	Matrix new = newMatrix(size(A));
	//traverse the rows
	
	for(int i = 1; i <= new->size; i++){
		//check to see if the row is empty
		if(length(A->Entries[i]) != 0){
			
			moveFront(A->Entries[i]);
			//copy the columns
			
			while(index(A->Entries[i]) != -1){
		
				Entry temp = get(A->Entries[i]);
				//we use change entry rather than append so it also updates the value
				
				changeEntry(new, i, temp->column, temp->value);
				//look at the next element in the list
				
				moveNext(A->Entries[i]);
			}
		}
	}
	return new;
}
// transpose()
// Returns a reference to a new Matrix object representing the transpose
// of A.
Matrix transpose(Matrix A){
	Matrix Transpose = newMatrix(size(A));
	for(int i = 1; i <= size(A); i++){
		if(length(A->Entries[i]) != 0){
			moveFront(A->Entries[i]);
			while(index(A->Entries[i]) != -1){
				Entry Temp = get(A->Entries[i]);
				changeEntry(Transpose, Temp->column, i, Temp->value);
				moveNext(A->Entries[i]);
			}
		}
	}
	return Transpose;
}

// scalarMult()
// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A){
	//check to see if A is null
	if(A == NULL) return NULL;
	//create a new matrix to store xA
	Matrix cM = newMatrix(size(A));
	for(int i = 1; i <= cM->size; i++){
		if(length(A->Entries[i]) != 0){
			moveFront(A->Entries[i]);
			while(index(A->Entries[i]) != -1){
				Entry temp = get(A->Entries[i]);
				changeEntry(cM,i,temp->column, (temp->value * x ) );
				moveNext(A->Entries[i]);
			}
		}
	}
	return cM;
}
//helper function add
//adds to the ith row jth column x to the Matrix A
void add(Matrix * A, int i, int j, double x){
	if((*A) != NULL && i <= (*A)->size && j <= (*A)->size){
	
		if(length((*A)->Entries[i]) != 0){
			moveFront((*A)->Entries[i]);
		
			while(index((*A)->Entries[i]) != -1){
				
				//grab the column entry we are looking at
				Entry a = get((*A)->Entries[i]);
				
				
				//if j column exist change the value of a
				if(a->column == j){
					if(x!=0){
						a->value += x;
						return;
					}
					else{
						free(a);
						delete((*A)->Entries[i]);
					}
				}
				//if a column is less then look at next column
				if(a->column < j){
					
					moveNext((*A)->Entries[i]);
					
				}
				//if j column doesnt already exist
				if(a->column > j && x != 0){
					
					Entry new = malloc(sizeof(Entry));
					new->column = j;
					new->value = x;
					insertBefore((*A)->Entries[i], new);
					(*A)->nze += 1;
					return;
				}
				
			}
			//j is the largest column just append
			if(x!=0){
				Entry new = malloc(sizeof(Entry));
				new->column = j;
				new->value = x;
				append((*A)->Entries[i], new);
				(*A)->nze+=1;
				return;
			}
			
		}
		else{
			if(x!= 0){
				//the list is empty just append it
				Entry new = malloc(sizeof(Entry));
				new->column = j;
				new->value = x;
				append((*A)->Entries[i], new);
				(*A)->nze+=1;
				return;
			}
		}
	}
}

// sum()
// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B){
	//check they are both valid matrices and they are the same size
	if( A == NULL || B == NULL || A->size != B->size ) {
		return NULL;
	}
	
	Matrix addition = newMatrix(A->size);
	for(int i = 1; i <= A->size; i++){
		moveFront(A->Entries[i]);
		moveFront(B->Entries[i]);
		while(index(A->Entries[i]) != -1 && index(B->Entries[i]) != -1){
			Entry a = get(A->Entries[i]);
			Entry b = get(B->Entries[i]);
			
			if(a->column == b->column){
				
				changeEntry(addition, i, b->column, (a->value + b->value) );
				moveNext(A->Entries[i]);
				if(index(B->Entries[i]) != index(A->Entries[i])){
					moveNext(B->Entries[i]);
				}
				
				
			}
			if(a->column > b->column){
				
				changeEntry(addition, i, b->column, b->value);
				moveNext(B->Entries[i]);
			}
			if(a->column < b->column){
				
				changeEntry(addition, i, a->column, a->value);
				moveNext(A->Entries[i]);
			}
		}
		if(index(A->Entries[i]) != -1 && index(B->Entries[i]) == -1){
			while(index(A->Entries[i]) != -1){
				Entry temp = get(A->Entries[i]);
				changeEntry(addition,i,temp->column, temp->value);
				moveNext(A->Entries[i]);
			}
		}
		if(index(B->Entries[i]) != -1 && index(A->Entries[i]) == -1){
			while(index(B->Entries[i]) != -1){
				Entry temp = get(B->Entries[i]);
				changeEntry(addition,i,temp->column, temp->value);
				moveNext(B->Entries[i]);
			}
		}
		
	}
	return addition;
}

// diff()
// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B){
	//check they are both valid matrices and they are the same size
	if( A == NULL || B == NULL || A->size != B->size ){
		 return NULL;
	 }
	Matrix subtraction = newMatrix(A->size);
	for(int i = 1; i <= A->size; i++){
		moveFront(A->Entries[i]);
		moveFront(B->Entries[i]);
		while(index(A->Entries[i]) != -1 && index(B->Entries[i]) != -1){
			Entry a = get(A->Entries[i]);
			Entry b = get(B->Entries[i]);
			if(a->column == b->column){

				add(&subtraction, i, b->column, (a->value - b->value) );
				moveNext(A->Entries[i]);
				moveNext(B->Entries[i]);
			}
			if(a->column > b->column){
				
				add(&subtraction, i, b->column, (0 - b->value));
				moveNext(B->Entries[i]);
			}
			if(a->column < b->column){
				
				add(&subtraction, i, a->column,  a->value );
				moveNext(A->Entries[i]);
			}
		}
		if(index(A->Entries[i]) != -1 && index(B->Entries[i]) == -1){
			while(index(A->Entries[i]) != -1){
				Entry temp = get(A->Entries[i]);
				
				add(&subtraction,i,temp->column, temp->value);
				moveNext(A->Entries[i]);
			}
		}
		if(index(B->Entries[i]) != -1 && index(A->Entries[i]) == -1){
			while(index(B->Entries[i]) != -1){
				Entry temp = get(B->Entries[i]);
			
				add(&subtraction,i,temp->column, (0 - temp->value) );
				moveNext(B->Entries[i]);
			}
		}
		
	}
	return subtraction;
}


// product()
// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B){
	//Make the new product Matrix we are returning
	Matrix AB = newMatrix(A->size);
	//iterate through the a rows
	for(int i = 1; i <= A->size;i++){
		
		//check to see if the A row is empty
		if(length(A->Entries[i]) != 0){
		
			//check to see if the B row is Emtpy
			if(length(B->Entries[i]) != 0){
			
				//iterate through the b rows
				for(int k = 1; k<=B->size;k++){
					//move the cursor to the front of A list
					moveFront(A->Entries[i]);
					
					//iterate through the i columns
					while(index(A->Entries[i]) != -1){
					
						//store A entry in a
						Entry a = get(A->Entries[i]);
						
						//move to the front of the B list
						moveFront(B->Entries[k]);
						
						//iterate through the k columns
						while(index(B->Entries[k]) != -1){
							Entry b = get(B->Entries[k]);
							if(a->column == k ){
								double result = (a->value * b->value);
								add(&AB, i, b->column, result );
							}
							moveNext(B->Entries[k]);
						}
						
						moveNext(A->Entries[i]);	
					}
				}	
			}
				
			
		}
		
	
	}
	
	return AB;
}




// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows 
// are not printed. Each non-zero row is represented as one line consisting 
// of the row number, followed by a colon, a space, then a space separated 
// list of pairs "(col, val)" giving the column numbers and non-zero values 
// in that row. The double val will be rounded to 1 decimal point.


void printMatrix(FILE* out, Matrix M){
	//check to make sure that the graph is not null or else you will get seg fault
	if(M == NULL) return;
	//check that the graph is not empty 
	if(M->nze > 0 && M->size > 0 ){
		bool flag = false;
		//loop through the row
		for(int i = 1; i<= M->size; i++){
			moveFront(M->Entries[i]);
			//check to see if the rows 
			if(index(M->Entries[i]) != -1){
				flag = true;
				fprintf(out, "%d: ",i);
			}
			//loop through the columns
			while(index(M->Entries[i])!=-1){
				Entry temp = get(M->Entries[i]);
				fprintf(out, "(%d, %.1f) ", temp->column, temp->value);
				moveNext(M->Entries[i]);
			}
			if(flag) fprintf(out, "\n");
			flag = false;
		}
	}
}

