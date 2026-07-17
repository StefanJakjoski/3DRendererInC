#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SMALL_OBJECT_AMOUNT 5                           // modified for optimization

typedef struct {
    double smallVectorArray[SMALL_OBJECT_AMOUNT];       // smaller vectors to use stack
    double* vectorArray;                                // larger vectors to use heap
    int vectorDimension;
} Vector;

typedef struct {
    double smallMatrixArray[SMALL_OBJECT_AMOUNT][SMALL_OBJECT_AMOUNT];      // smaller matrices to use stack
    double** matrixArray;                                                   // larger matrices to use heap
    int matrixRows;
    int matrixColumns;
} Matrix;

enum VectorType {
    ROW = 0, COLUMN = 1
};

// CreateVector FUNCTION
// DEFINE VECTOR ARRAY WITH POSITIVE DIMENSION AT SPECIFIED POINTER
//
// Vector* newVector;
// if(!CreateVector(&newVector, (int) vectorDimension))
//   errorHandler(); 
//
int CreateVector(Vector** res, int vectorSize);

// CreateVector2 FUNCTION
// RETURNS POINTER TO VECTOR ARRAY WITH SPECIFIED DIMENSION
// RETURNS NULL IF DIMENSION <= 0
//
// Vector* newVector = CreateVector2((int) vectorDimension);
//
Vector* CreateVector2(int vectorSize);

// FreeVector FUNCTION
// FREES ALL POINTERS ASSOCIATED WITH SPECIFIED VECTOR
//
// FreeVector((Vector *) v);
//
void FreeVector(Vector* v);

double vGet(Vector* v, int index);

int vSet(Vector* v, int index, double val);

// CompareVectors FUNCTION
// COMPARES DIMENSIONS AND INDIVIDUAL ELEMENTS OF 2 SPECIFIED VECTORS
// RETURNS 0 IF VECTORS ARE NOT IDENTICAL, RETURNS 1 OTHERWISE
//
// if(!CompareVectors((Vector *) a, (Vector *) b))
//   ErrorHandler();
//
int CompareVectors(Vector* a, Vector* b);

// VectorToString FUNCTION
// PRINT CONTENTS OF VECTOR a TO STANDARD OUTPUT
//
// if(!VectorToString((Vector *) a))
//   errorHandler(); 
//
int VectorToString(Vector* a);

// ArrayToVector FUNCTION
// DEFINE VECTOR ARRAY WITH POSITIVE DIMENSION AT SPECIFIED POINTER
// FROM ARRAY OF TYPE DOUBLE
//
// Vector* newVector;
// if(!ArrayToVector(&newVector, (double *) array, (int) dimensionSize))
//   errorHandler(); 
//
int ArrayToVector(Vector** res, double* array, int arrayDimension);

// DuplicateVector FUNCTION
// DUPLICATE VECTOR ARRAY TO SPECIFIED POINTER
//
// Vector* newVector;
// if(!DuplicateVector(&newVector, (Vector *) original))
//   errorHandler(); 
//
int DuplicateVector(Vector** res, Vector* original);

// DuplicateVector2 FUNCTION
// RETURN DUPLICATE OF SPECIFIED VECTOR ARRAY
// RETURNS NULL IF DIMENSION <= 0
//
// Vector* newVector = DuplicateVector2((Vector *) original);
//
Vector* DuplicateVector2(Vector* original);

// SumVectors FUNCTION 
// MATHEMATICAL SUMMATION OF VECTORS a AND b WRITTEN OVER VECTOR a
// DOES NOT PRESERVE ORIGINAL VECTOR a
//
// (Effectively: a += b)
// if(!SumVectors((Vector *) a, (Vector *) b))
//   errorHandler(); 
//
int SumVectors(Vector* a, Vector* b);

// SumVectors2 FUNCTION 
// RETURNS MATHEMATICAL SUMMATION OF VECTORS a AND b AS NEW VECTOR
// RETURNS NULL IF DIMENSIONS <= 0
//
// Vector* c = SumVectors2((Vector *) a, (Vector *) b); 
//
Vector* SumVectors2(Vector* a, Vector* b);

// SubtractVectors FUNCTION 
// MATHEMATICAL SUBTRACTION OF VECTORS a AND b WRITTEN OVER VECTOR a
// DOES NOT PRESERVE ORIGINAL VECTOR a
//
// if(!SubtractVectors((Vector *) a, (Vector *) b))
//   errorHandler(); 
//
int SubtractVectors(Vector* a, Vector* b);

// SubtractVectors2 FUNCTION 
// RETURNS MATHEMATICAL SUBTRACTION OF VECTORS a AND b AS NEW VECTOR
// RETURNS NULL IF DIMENSIONS <= 0
//
// Vector* c = SubtractVectors2((Vector *) a, (Vector *) b); 
//
Vector* SubtractVectors2(Vector* a, Vector* b);

// MultiplyVector FUNCTION 
// MATHEMATICAL MULTIPLICATION OF VECTOR v AND SCALAR COEFFICIENT c WRITTEN OVER VECTOR a
// DOES NOT PRESERVE ORIGINAL VECTOR v
//
// if(!MultiplyVector((Vector *) v, (int) b))
//   errorHandler(); 
//
int MultiplyVector(Vector* a, const double coefficient);

// MultiplyVector2 FUNCTION 
// OUTPUTS MATHEMATICAL MULTIPLICATION OF VECTOR v AND SCALAR COEFFICIENT c
// PRESERVES ORIGINAL VECTOR v
// 
// Vector* product = MultiplyVector2((Vector *) v, (double) c);
//
Vector* MultiplyVector2(Vector* a, const double coefficient);

// CrossProduct FUNCTION
// CALCULATE CROSS PRODUCT OF 2 VECTORS AND SAVE TO POINTER
//
// Vector* crossProductVector;
// if(!CrossProduct(&crossProductVector, (Vector *) a, (Vector *) b))
//   errorHandler(); 
//
int CrossProduct(Vector** newVector, Vector* a, Vector* b);

// DotProduct FUNCTION
// CALCULATE DOT PRODUCT OF 2 VECTORS TO SPECIFIED POINTER.
//
// double dotProduct;
// if(!DotProduct(&dotProduct, (Vector *) a, (Vector *) b))
//   errorHandler(); 
//
int DotProduct(double* res, Vector* a, Vector* b);

// DotProduct2 FUNCTION
// RETURN DOT PRODUCT OF 2 VECTORS AS DOUBLE
//
// double dotProduct = DotProduct2((Vector *) a, (Vector *) b);
// if(isnan(dotProduct))
//   errorHandler();
//
double DotProduct2(Vector* a, Vector* b);

// VectorLength FUNCTION
// CALCULATE LENGTH (OR NORM) OF SPECIFIED VECTOR AND RETURN VALUE.
// RETURNS -1 IF VECTOR DIMENSION <= 0
//
// double norm;
// if(norm = VectorLength((Vector *) v) < 0)
//   errorHandler(); 
//
double VectorLength(Vector* v);

// UnitVector FUNCTION
// CALCULATE UNIT VECTOR FROM SPECIFIED VECTOR AND OVERWRITE ORIGINAL
//
// Vector* v;
// if(!UnitVector((Vector *) v))
//   errorHandler(); 
//
int UnitVector(Vector* original);

// UnitVector2 FUNCTION
// CALCULATE UNIT VECTOR FROM SPECIFIED VECTOR RETURN VECTOR OUTPUT
// RETURNS NULL IF DIMENSION <= 0
//
// Vector* unitVector = unitVector2((Vector *) v);
//
Vector* UnitVector2(Vector* original);

// ProjectVector FUNCTION
// CALCULATES PROJECTION OF VECTOR ONTO BASE VECTOR OF 
// SAME DIMENSION
//
// Vector* projab = NULL;
// if(!ProjectVector(&projab, (Vector *) a, (Vector *) b))
//   ErrorHandler();
//
int ProjectVector(Vector** res, Vector* base, Vector* v);

// ProjectVector2 FUNCTION
// RETURNS VECTOR POINTER OF PROJECTION FROM ONE VECTOR
// TO SPECIFIED BASE VECTOR
// RETURNS NULL IF ERROR
//
// Vector* projab = ProjectVector2((Vector *) a, (Vector *) b);
//
Vector* ProjectVector2(Vector* base, Vector* v);


//
// START OF MATRIX FUNCTIONS
//


// CreateMatrix FUNCTION
// DEFINE MATRIX WITH POSITIVE DIMENSIONS AT SPECIFIED POINTER
//
// Matrix* newMatrix;
// if(!CreateMatrix(&newMatrix, (int) rowDimension, (int) columnDimension))
//   errorHandler(); 
//
int CreateMatrix(Matrix** res, int rows, int columns);

// CreateMatrix2 FUNCTION
// RETURNS MATRIX POINTER WITH SPECIFIED DIMENSIONS
// RETURNS NULL IF EITHER DIMENSION <= 0
//
// Matrix* m = CreateMatrix((int) rowDimension, (int) colDimension);
//
Matrix* CreateMatrix2(int rows, int columns);

// FreeMatrix FUNCTION
// FREES POINTERS ASSOCIATED WITH MATRIX
//
// FreeMatrix((Matrix *) m);
//
void FreeMatrix(Matrix* m);

double mGet(Matrix* m, int row, int col);

int mSet(Matrix* m, int row, int col, double val);

// CreateIdentityMatrix FUNCTION
// DEFINE IDENTITY MATRIX AT SPECIFIED POINTER
//
// Matrix* identity;
// if(!CreateIdentityMatrix(&identity, (int) dimensions))
//   ErrorHandler();
//
int CreateIdentityMatrix(Matrix** res, int dimensions);

// CreateIdentityMatrix2 FUNCTION
// RETURN IDENTITY MATRIX FROM SPECIFIED DIMENSIONS
// RETURNS NULL IF DIMENSIONS ARE IMPROPER
//
// Matrix* identity = CreateIdentityMatrix2((int) dimensions);
//
Matrix* CreateIdentityMatrix2(int dimensions);

// CompareMatrices FUNCTION
// COMPARES 2 SPECIFIED MATRICES
// RETURNS 1 IF IDENTICAL, 0 OTHERWISE
//
// if(!CompareMatrices((Matrix *) a, (Matrix *) b))
//   ErrorHandler();
//
int CompareMatrices(Matrix* a, Matrix* b);

// TransposeMatrix FUNCTION
// CALCULATE TRANSPOSE OF MATRIX AND SAVE TO SPECIFIED POINTER
//
// Matrix* m;
// if(!TransposeMatrix(&m, (Matrix *) a))
//   ErrorHandler();
// 
int TransposeMatrix(Matrix** res, Matrix* a);

// TransposeMatrix2 FUNCTION
// RETURN TRANSPOSE OF MATRIX
// RETURNS NULL IF EITHER DIMENSION <= 0
//
// Matrix* m = TransposeMatrix2((Matrix *) a);
//
Matrix* TransposeMatrix2(Matrix* a);

// MatrixToString FUNCTION
// PRINT CONTENTS OF MATRIX TO STANDARD OUTPUT 
//
// if(!MatrixToString((Matrix *) m))
//   ErrorHandler();
//
int MatrixToString(Matrix* m);

// ArrayToMatrix FUNCTION
// DEFINE MATRIX WITH POSITIVE DIMENSIONS AT SPECIFIED POINTER
// FROM 2D ARRAY OF TYPE DOUBLE
//
// Matrix* newMatrix;
// if(!ArrayToMatrix(&newMatrix, (double **) array, (int) arrayRows, (int) arrayColumns))
//   errorHandler(); 
//
int ArrayToMatrix(Matrix** res, double** array, int arrayRows, int arrayColumns);

// DuplicateMatrix FUNCTION
// RETURNS POINTER TO DUPLICATE OF GIVEN MATRIX
// NULL IF ERROR
//
// Matrix* duplicate = DuplicateMatrix((Matrix *) m);
//
Matrix* DuplicateMatrix(Matrix* m);

// SumMatrices FUNCTION
// CALCULATES SUM OF 2 MATRICES TO A SPECIFIED POINTER
//
// Matrix* sum;
// if(!SumMatrices(&sum, (Matrix *) a, (Matrix *) b))
//   ErrorHandler();
//
int SumMatrices(Matrix** res, Matrix* a, Matrix* b);

// SumMatrices2 FUNCTION
// RETURNS SUM OF 2 MATRICES
//
// Matrix* sum = SumMatrices((Matrix *) a, (Matrix *) b);
//
Matrix* SumMatrices2(Matrix* a, Matrix* b);

// MultiplyMatrices FUNCTION
// TRADITIONAL FUNCTION MULTIPLICATION FOR 2 MATRICES
//
// Matrix* product;
// if(!MultiplyMatrices(&product, (Matrix *) a, (Matrix *) b))
//   ErrorHandler();
//
int MultiplyMatrices(Matrix** res, Matrix* a, Matrix* b);

// MultiplyMatrices2 FUNCTION
// RETURNS PRODUCT OF TRADITIONAL MULTIPLICATION FOR 2 MATRICES
//
// Matrix* product = MultiplyMatrices2((Matrix *) a, (Matrix *) b);
//
Matrix* MultiplyMatrices2(Matrix* a, Matrix* b);

// MatrixScalarMultiplication FUNCTION
// CALCULATES SCALAR MATRIX MULTIPLICATION SAVED TO SPECIFIED MATRIX
//
// Matrix* scalarProduct;
// if(!MatrixScalarMultiplication(&scalarProduct, (Matrix *) a, (double) scalar))
//   ErrorHandler();
//
int MatrixScalarMultiplication(Matrix** res, Matrix* a, const double scalar);

// MatrixScalarMultiplication2 FUNCTION
// RETURNS SCALAR MATRIX MULTIPLICATION
//
// Matrix* scalarProduct = MatrixScalarMultiplication2((Matrix *) a, (double) scalar);
//
Matrix* MatrixScalarMultiplication2(Matrix* a, const double scalar);

// SwapMatrixRows FUNCTION
// SWAPS SPECIFIED ROWS IN GIVEN MATRIX
//
// if(!SwapMatrixRows((Matrix *) m, (int) row1, (int) row2))
//   ErrorHandler();
//
int SwapMatrixRows(Matrix* a, int row1, int row2);

// MultiplyRow MATRIX
// MULTIPLY ROW OF MATRIX BY SCALAR VALUE
//
// if(!MultiplyRow((Matrix *) a, (int) row, (int) scalar))
//   ErrorHandler();
//
int MultiplyRow(Matrix* a, int row, double scalar);

// AddRow FUNCTION
// ADD THE CONTENTS OF ONE MATRIX ROW ONTO ANOTHER
// (Effectively: matrix[row1] += matrix[row2])
//
// if(!AddRow((Matrix *) a, (int) rowBase, (int) rowSecondary))
//   ErrorHandler();
//
int AddRow(Matrix* a, int rowBase, int rowSecondary);

// DeterminantRecursive FUNCTION 
// RECURSIVELY CALCULATES DETERMINANT OF GIVEN MATRIX 
// RETURNS DOUBLE AND PRINTS ERROR IF IMPROPER DIMENSIONS
//
// double determinant = DeterminantRecursive((Matrix *) m);
// 
double DeterminantRecursive(Matrix* m);

// FindNonZeroInColumn() FUNCTION
// FINDS FIRST NON ZERO ENTRY IN SPECIFIED MATRIX COLUMN (TOP TO BOTTOM)
// STARTS SEARCH AT SPECIFIED STARTING ROW
// RETURNS -1 IF ERROR OR ALL ENTRIES ARE 0
//
int FindNonZeroInColumn(Matrix* m, int column, int startingRow);

// InvertSquareMarix FUNCTION
// INVERTS SQUARE MATRIX AND ASSIGNS RESULT TO SPECIFIED POINTER
// 
// Matrix* inverse;
// if(!InvertSquareMatrix(&inv, (Matrix *) m))
//   ErrorHandler();
//
int InvertSquareMatrix(Matrix** res, Matrix* m);

// InvertSquareMatrix2 FUNCTION
// RETURNS INVERTED MATRIX FROM SPECIFIED MATRIX
// RETURNS NULL IF ERROR
// 
// Matrix* inverse = InvertSquareMatrix2((Matrix *) m);
//
Matrix* InvertSquareMatrix2(Matrix* m);

// MultiplyMatrixVector() FUNCTION
// CALCULATE MATRIX*VECTOR MULTIPLICATION TO SPECIFIED VECTOR POINTER
//
// Vector* product;
// if(!MultiplyMatrixVector(&product, (Matrix *) m, (Vector *) v))
//   ErrorHandler();
//
int MultiplyMatrixVector(Vector** res, Matrix* m, Vector* v);

// MultiplyMatrixVector2() FUNCTION
// RETURN VECTOR POINTER FOR MATRIX*VECTOR PRODUCT
// RETURNS NULL IF ERROR OR MISALIGNED DIMENSIONS
//
// Vector* product = MultiplyMatrixVector2((Matrix *) m, (Vector *) v);
//
Vector* MultiplyMatrixVector2(Matrix* m, Vector* v);

// MoorePenroseInverse FUNCTION
// CALCULATES MOORE PENROSE INVERSE OF MATRIX
// 
// Matrix* MPInverse;
// if(!MoorePenroseInverse(&MPInverse, (Matrix *) m))
//   ErrorHandler();
//
int MoorePenroseInverse(Matrix** res, Matrix* m);

// SubstituteVectorInMatrix FUNCTION
// SUBSTITUTES ROW OR COLUMN IN MATRIX WITH THE SPECIFIED
// VECTOR AT GIVEN INDEX
//
// int MatrixSubstitutionIndex = 2;
// int vType = ROW;  (ROW = 0, COLUMN = 1)
// if(!SubstituteVectorInMatrix((Matrix *) m, (Vector *) v, MatrixSubstitutionIndex, vType))
//   ErrorHandler();
//
int SubstituteVectorInMatrix(Matrix* m, Vector* v, int it, int vectorType);

// MatrixToVectorArray FUNCTION
// DECOMPOSES MATRIX TO ARRAY OF SPECIFIED VECTOR TYPE
// (ROW = 0, COLUMN = 1)
//
// Vector** vectorArray;
// if(!MatrixToVectorArray(&vectorArray, (Matrix *) m, ROW))
//   ErrorHandler();
//
int MatrixToVectorArray(Vector*** array, Matrix* m, int vectorType);

// VectorArrayToMatrix FUNCTION
// RETURNS MATRIX FROM VECTOR ARRAY OF SPECIFIED VECTOR TYPE
// (ROW = 0, COLUMN = 1)
//
// Matrix* m;
// if(!VectorArrayToMatrix(&m, (Vector **) vectorArray, COLUMN))
//   ErrorHandler();
// 
int VectorArrayToMatrix(Matrix** res, Vector** vectorArray, int arrayLength, int vectorType);

// QRDecomposition FUNCTION
// CALCULATES QR DECOMPOSITION OF SPECIFIED MATRIX
//
// Matrix *Q, *R;
// if(!QRDecomposition(&Q, &R, (Matrix *) m))
//   ErrorHandler();
//
int QRDecomposition(Matrix** Q, Matrix** R, Matrix* m);

#endif // LINEAR_ALGEBRA_H