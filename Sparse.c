/****************************************************************************************
*  Andrew Dell'Aringa 2082835
*  PA4 02/07/24
*  Sparse.c Reads file and manipulates and prints
*****************************************************************************************/
#include "Matrix.h"
#include "assert.h"

int main (int argc, char ** argv){
	FILE *in, *out;
 	// check command line for correct number of arguments
	if( argc != 3 ){
		printf("Error incorrect arguments\n");
		exit(1);
	}

	//store and open the inputfile
	in = fopen(argv[1], "r");
	if( in==NULL ){
		printf("Unable to open file %s for reading\n", argv[1]);
		exit(1);
	}
	//store and open the output file
	out = fopen(argv[2], "w");
	if( out==NULL ){
		printf("Unable to open file %s for writing\n", argv[2]);
		exit(1);
	}
	
	//The input file will begin with a single line containing three integer n, a, and b seperated by spaces
	int n, a, b;
	fscanf(in, "%d %d %d", &n, &a, &b); 
	Matrix A = newMatrix(n);
	Matrix B = newMatrix(n);
	
	//the line after is blank skip it
	char temporary;
	fscanf(in, "%c", &temporary);
	
	//the following a lines will specify the non-zero entries of an nxn matrix A.
	//each line will contain a space seperated list of three numbers
	//two integers and a double fiving the row column and value
	int r , c;
	double v;
	
	for(int i = 0; i < a; i++){
		fscanf(in, "%d %d %lf", &r, &c, &v) ;
	
		//handle the data
		changeEntry(A, r, c ,v);
		
    	}
    	assert(NNZ(A) == a);
    	//the line after is blank skip it
    	
	fscanf(in, "%c", &temporary);
	
	for(int i = 0; i < b; i++){
		fscanf(in, "%d %d %lf", &r, &c, &v);
	
		//handle the data
		changeEntry(B, r ,c ,v);
		
    	}
    	assert(NNZ(B) == b);
    	
    //***************************************************************************************
    	//matrix modification
    //***************************************************************************************
    	//print A
    	fprintf(out, "A has %d non-zero entries:\n", a);
    	printMatrix(out,A);
    	fprintf(out, "\n");
    	
    	
    	//print B
    	fprintf(out, "B has %d non-zero entries:\n", b);
    	printMatrix(out,B);
    	fprintf(out, "\n");
    	
    	
    	//(1.5)*A
    	fprintf(out, "(1.5)*A =\n");
    	Matrix A1 = scalarMult(1.5, A);
    	printMatrix(out,A1);
    	fprintf(out, "\n");
    	
    	//A+B
    	fprintf(out, "A+B =\n");
    	Matrix A2 = sum(A,B);
    	printMatrix(out,A2);
    	fprintf(out, "\n");
    	
    	//A+A
    	fprintf(out, "A+A =\n");
    	Matrix A3 = sum(A,A);
    	printMatrix(out,A3);
    	fprintf(out, "\n");
    	
    	//B-A
    	fprintf(out, "B-A =\n");
    	Matrix A4 = diff(B,A);
    	printMatrix(out,A4);
    	fprintf(out, "\n");
    	
    	//A-A
    	fprintf(out, "A-A =\n");
    	Matrix A5 = diff(A,A);
    	printMatrix(out,A5);
    	fprintf(out, "\n");
    	
    	//Transpose(A)
    	fprintf(out,"Transpose(A) =\n");
    	Matrix A6 = transpose(A);
    	printMatrix(out,A6);
    	fprintf(out, "\n");
    	
    	//A*B
    	fprintf(out, "A*B =\n");
    	Matrix A7 = product(A,B);
    	printMatrix(out,A7);
    	fprintf(out, "\n");
    	//B*B
    	fprintf(out, "B*B =\n");
    	Matrix A8 = product(B,B);
    	printMatrix(out,A8);
    	fprintf(out, "\n");
    	
    	//FREE THE DATA 
    	freeMatrix(&A);
    	freeMatrix(&B);
    	freeMatrix(&A1);
    	freeMatrix(&A2);
    	freeMatrix(&A3);
    	freeMatrix(&A4);
    	freeMatrix(&A5);
    	freeMatrix(&A6);
    	freeMatrix(&A7);
    	freeMatrix(&A8);
	return 0;
}
