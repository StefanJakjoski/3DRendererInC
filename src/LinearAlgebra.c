#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../include/LinearAlgebra.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define ZERO_APPROX 1e-13

// CreateVector FUNCTION
// DEFINE VECTOR ARRAY WITH POSITIVE DIMENSION AT SPECIFIED POINTER
//
// Vector* newVector;
// if(!CreateVector(&newVector, (int) vectorDimension))
//   errorHandler(); 
//
int CreateVector(Vector** res, int vectorSize){
    if(vectorSize <= 0){
        fprintf(stderr, "Improper vector dimension.\n");
        return 0;
    }

    Vector* buffer = (Vector *) malloc(sizeof(Vector));
    if(buffer == NULL){
        fprintf(stderr, "Memory allocation failed.\n");
        return 0;
    }

    buffer->vectorDimension = vectorSize;
    if(vectorSize > SMALL_OBJECT_AMOUNT)
        buffer->vectorArray = (double *) calloc(vectorSize, sizeof(double));
    else{
        for(int i = 0; i < vectorSize; i++){
            buffer->smallVectorArray[i] = 0.0;
        }
    }
    
    *res = buffer;
    return 1;
}

// CreateVector2 FUNCTION
// RETURNS POINTER TO VECTOR ARRAY WITH SPECIFIED DIMENSION
// RETURNS NULL IF DIMENSION <= 0
//
// Vector* newVector = CreateVector2((int) vectorDimension);
//
Vector* CreateVector2(int vectorSize){
    Vector* v;
    if(!CreateVector(&v, vectorSize))
        return NULL;

    return v;
}

// FreeVector FUNCTION
// FREES ALL POINTERS ASSOCIATED WITH SPECIFIED VECTOR
//
// FreeVector((Vector *) v);
//
void FreeVector(Vector* v){
    if(v->vectorDimension > SMALL_OBJECT_AMOUNT)
        free(v->vectorArray);
    free(v);
}


double vGet(Vector* v, int index){
    if(v->vectorDimension <= index){
        fprintf(stderr, "Index out of range.\n");
        return NAN;
    }

    if(v->vectorDimension <= SMALL_OBJECT_AMOUNT){
        return v->smallVectorArray[index];
    }

    return v->vectorArray[index];
}

int vSet(Vector* v, int index, double val){
    if(v->vectorDimension <= index){
        fprintf(stderr, "Index out of range.\n");
        return 0;
    }

    if(v->vectorDimension <= SMALL_OBJECT_AMOUNT){
        v->smallVectorArray[index] = val;
        return 1;
    }

    v->vectorArray[index] = val;
    return 1;
}

// CompareVectors FUNCTION
// COMPARES DIMENSIONS AND INDIVIDUAL ELEMENTS OF 2 SPECIFIED VECTORS
// RETURNS 0 IF VECTORS ARE NOT IDENTICAL, RETURNS 1 OTHERWISE
//
// if(!CompareVectors((Vector *) a, (Vector *) b))
//   ErrorHandler();
//
int CompareVectors(Vector* a, Vector* b){
    if(a->vectorDimension != b->vectorDimension){
        fprintf(stderr, "Improper vector dimensions.\n");
        return 0;
    }

    for(int i = 0; i < a->vectorDimension; i++){
        if(vGet(a, i) != vGet(b, i)){
            return 0;
        }
    }

    return 1;
}

// VectorToString FUNCTION
// PRINT CONTENTS OF VECTOR a TO STANDARD OUTPUT
//
// if(!VectorToString((Vector *) a))
//   errorHandler(); 
//
int VectorToString(Vector* a){
    if(a->vectorDimension <= 0){
        fprintf(stderr, "Improper vector dimension.\n");
        return 0;
    }

    printf("{");
    for(int i = 0; i < a->vectorDimension; i++){
        if(i >= 1){ printf(", "); }
        printf("%f", vGet(a, i));
    }
    printf("}\n");

    return 1;
}

// ArrayToVector FUNCTION
// DEFINE VECTOR ARRAY WITH POSITIVE DIMENSION AT SPECIFIED POINTER
// FROM ARRAY OF TYPE DOUBLE
//
// Vector* newVector;
// if(!ArrayToVector(&newVector, (double *) array, (int) dimensionSize))
//   errorHandler(); 
//
int ArrayToVector(Vector** res, double* array, int arrayDimension){
    if(arrayDimension <= 0){
        fprintf(stderr, "Improper vector dimension.\n");
        return 0;
    }

    Vector *buffer = NULL;
    if(!CreateVector(&buffer, arrayDimension))
        return 0;

    
    if(buffer->vectorDimension <= SMALL_OBJECT_AMOUNT)
        memcpy(buffer->smallVectorArray, array, arrayDimension*sizeof(double));
    else
        memcpy(buffer->vectorArray, array, arrayDimension*sizeof(double));

    *res = buffer;
    return 1;
}

// DuplicateVector FUNCTION
// DUPLICATE VECTOR ARRAY TO SPECIFIED POINTER
//
// Vector* newVector;
// if(!DuplicateVector(&newVector, (Vector *) original))
//   errorHandler(); 
//
int DuplicateVector(Vector** res, Vector* original){
    if(original->vectorDimension <= 0){
        fprintf(stderr, "Improper vector dimension.\n");
        return 0;
    }

    Vector* buffer = CreateVector2(original->vectorDimension);
    if(original->vectorDimension <= SMALL_OBJECT_AMOUNT)
        memcpy(buffer->smallVectorArray, original->smallVectorArray, original->vectorDimension * sizeof(double));
    else
        memcpy(buffer->vectorArray, original->vectorArray, original->vectorDimension * sizeof(double));
    *res = buffer;

    return 1;
}

// DuplicateVector2 FUNCTION
// RETURN DUPLICATE OF SPECIFIED VECTOR ARRAY
// RETURNS NULL IF DIMENSION <= 0
//
// Vector* newVector = DuplicateVector2((Vector *) original);
//
Vector* DuplicateVector2(Vector* original){
    Vector* v;
    if(!DuplicateVector(&v, original))
        return NULL;
    
    return v;
}

// SumVectors FUNCTION 
// MATHEMATICAL SUMMATION OF VECTORS a AND b WRITTEN OVER VECTOR a
// DOES NOT PRESERVE ORIGINAL VECTOR a
//
// (Effectively: a += b)
// if(!SumVectors((Vector *) a, (Vector *) b))
//   errorHandler(); 
//
int SumVectors(Vector* a, Vector* b){
    if(a->vectorDimension != b->vectorDimension || a->vectorDimension <= 0){
        fprintf(stderr, "Improper vector dimensions.\n");
        return 0;
    }

    for(int i = 0; i < a->vectorDimension; i++){
        double newRes = vGet(a, i) + vGet(b, i);
        vSet(a, i, newRes);
    }

    return 1;
}

// SumVectors2 FUNCTION 
// RETURNS MATHEMATICAL SUMMATION OF VECTORS a AND b AS NEW VECTOR
// RETURNS NULL IF DIMENSIONS <= 0
//
// Vector* c = SumVectors2((Vector *) a, (Vector *) b); 
//
Vector* SumVectors2(Vector* a, Vector* b){
    if(a->vectorDimension != b->vectorDimension || a->vectorDimension <= 0){
        fprintf(stderr, "Improper vector dimensions.\n");
        return NULL;
    }

    Vector* v = DuplicateVector2(a);
    if(!SumVectors(v, b))
        return NULL;
    
    return v;
}

// SubtractVectors FUNCTION 
// MATHEMATICAL SUBTRACTION OF VECTORS a AND b WRITTEN OVER VECTOR a
// DOES NOT PRESERVE ORIGINAL VECTOR a
//
// if(!SubtractVectors((Vector *) a, (Vector *) b))
//   errorHandler(); 
//
int SubtractVectors(Vector* a, Vector* b){
    if(a->vectorDimension != b->vectorDimension || a->vectorDimension <= 0){
        fprintf(stderr, "Improper vector dimensions.\n");
        return 0;
    }

    for(int i = 0; i < a->vectorDimension; i++){
        double newRes = vGet(a, i) - vGet(b, i);
        vSet(a, i, newRes);
    }

    return 1;
}

// SubtractVectors2 FUNCTION 
// RETURNS MATHEMATICAL SUBTRACTION OF VECTORS a AND b AS NEW VECTOR
// RETURNS NULL IF DIMENSIONS <= 0
//
// Vector* c = SubtractVectors2((Vector *) a, (Vector *) b); 
//
Vector* SubtractVectors2(Vector* a, Vector* b){
    if(a->vectorDimension != b->vectorDimension || a->vectorDimension <= 0){
        fprintf(stderr, "Improper vector dimensions.\n");
        return NULL;
    }

    Vector* v = DuplicateVector2(a);
    if(!SubtractVectors(v, b))
        return NULL;
    
    return v;
}

// MultiplyVector FUNCTION 
// MATHEMATICAL MULTIPLICATION OF VECTOR v AND SCALAR COEFFICIENT c WRITTEN OVER VECTOR a
// DOES NOT PRESERVE ORIGINAL VECTOR v
//
// if(!MultiplyVector((Vector *) v, (int) b))
//   errorHandler(); 
//
int MultiplyVector(Vector* a, const double coefficient){
    if(a->vectorDimension <= 0){
        fprintf(stderr, "Improper vector dimension.\n");
        return 0;
    }

    for(int i = 0; i < a->vectorDimension; i++){
        double newRes = vGet(a, i) * coefficient;
        vSet(a, i, newRes);
    }

    return 1;
}

// MultiplyVector2 FUNCTION 
// OUTPUTS MATHEMATICAL MULTIPLICATION OF VECTOR v AND SCALAR COEFFICIENT c
// PRESERVES ORIGINAL VECTOR v
// 
// Vector* product = MultiplyVector2((Vector *) v, (double) c);
//
Vector* MultiplyVector2(Vector* a, const double coefficient){
    if(a->vectorDimension <= 0){
        fprintf(stderr, "Improper vector dimension.\n");
        return NULL;
    }

    Vector* v = DuplicateVector2(a);
    if(!MultiplyVector(v, coefficient))
        return NULL;
    
    return v;
}

// ProjectVector FUNCTION
// CALCULATES PROJECTION OF VECTOR ONTO BASE VECTOR OF 
// SAME DIMENSION
//
// Vector* projab = NULL;
// if(!ProjectVector(&projab, (Vector *) a, (Vector *) b))
//   ErrorHandler();
//
int ProjectVector(Vector** res, Vector* base, Vector* v){
    if(base->vectorDimension != v->vectorDimension || base->vectorDimension <= 0){
        fprintf(stderr, "ProjectVector: Improper vector dimension.\n");
        return 0;
    }

    double bv = DotProduct2(base, v);
    double bb = DotProduct2(base, base);
    Vector* sb = MultiplyVector2(base, bv/bb);
    if(sb == NULL)
        return 0;
    
    *res = sb;
    
    return 1;
}

Vector* projectVector2(Vector* base, Vector* v){
    Vector* res = NULL;
    if(!ProjectVector(&res, base, v))
        return NULL;
    
    return res;
}

// CrossProduct FUNCTION
// CALCULATE CROSS PRODUCT OF 2 VECTORS AND SAVE TO POINTER
//
// Vector* crossProductVector;
// if(!CrossProduct(&crossProductVector, (Vector *) a, (Vector *) b))
//   errorHandler(); 
//
int CrossProduct(Vector** newVector, Vector* a, Vector* b){
    int properVectorDimension = 3;
    if(a->vectorDimension != properVectorDimension || b->vectorDimension != properVectorDimension){
        fprintf(stderr, "Improper vector dimensions.\n");
        return 0;
    }

    if(!CreateVector(newVector, properVectorDimension)){
        fprintf(stderr, "Error during new vector creation.\n");
        return 0;
    }

    Vector *buffer = *newVector;
    double new0 = vGet(a, 1)*vGet(b, 2) - vGet(a, 2)*vGet(b, 1);
    double new1 = vGet(a, 2)*vGet(b, 0) - vGet(a, 0)*vGet(b, 2);
    double new2 = vGet(a, 0)*vGet(b, 1) - vGet(a, 1)*vGet(b, 0);

    vSet(buffer, 0, new0); vSet(buffer, 1, new1); vSet(buffer, 2, new2);

    return 1;
}

// DotProduct FUNCTION
// CALCULATE DOT PRODUCT OF 2 VECTORS TO SPECIFIED POINTER.
//
// double dotProduct;
// if(!DotProduct(&dotProduct, (Vector *) a, (Vector *) b))
//   errorHandler(); 
//
int DotProduct(double* res, Vector* a, Vector* b){
    if(a->vectorDimension != b->vectorDimension || a->vectorDimension <= 0){
        fprintf(stderr, "Improper vector dimensions.\n");
        return 0;
    }

    double buffer = 0;
    for(int i = 0; i < a->vectorDimension; i++){
        buffer += vGet(a, i)*vGet(b, i);
    }
    *res = buffer;

    return 1;
}

// DotProduct2 FUNCTION
// RETURN DOT PRODUCT OF 2 VECTORS AS DOUBLE
//
// double dotProduct = DotProduct2((Vector *) a, (Vector *) b);
// if(isnan(dotProduct))
//   errorHandler();
//
double DotProduct2(Vector* a, Vector* b){
    double dotProduct;
    if(!DotProduct(&dotProduct, a, b))
        return NAN;
    
    return dotProduct;
}

// VectorLength FUNCTION
// CALCULATE LENGTH (OR NORM) OF SPECIFIED VECTOR AND RETURN VALUE.
// RETURNS -1 IF VECTOR DIMENSION <= 0
//
// double norm;
// if(norm = VectorLength((Vector *) v) < 0)
//   errorHandler(); 
//
double VectorLength(Vector* v){
    if(v->vectorDimension <= 0){
        fprintf(stderr, "Improper vector dimension.\n");
        return -1;
    }

    double res = 0;
    for(int i = 0; i < v->vectorDimension; i++){
        res += pow(vGet(v, i), 2);
    }

    return sqrt(res);
}

// UnitVector FUNCTION
// CALCULATE UNIT VECTOR FROM SPECIFIED VECTOR AND OVERWRITE ORIGINAL
//
// Vector* v;
// if(!UnitVector((Vector *) v))
//   errorHandler(); 
//
int UnitVector(Vector* original){
    double norm = VectorLength(original);
    if(norm < 0)
        return 0;
    
    if(!MultiplyVector(original, 1.0/norm))
        return 0;

    return 1;
}

// UnitVector2 FUNCTION
// CALCULATE UNIT VECTOR FROM SPECIFIED VECTOR RETURN VECTOR OUTPUT
// RETURNS NULL IF DIMENSION <= 0
//
// Vector* unitVector = unitVector2((Vector *) v);
//
Vector* UnitVector2(Vector* original){
    Vector* v;
    if(!DuplicateVector(&v, original))
        return NULL;

    if(!UnitVector(v))
        return NULL;

    return v;
}


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
int CreateMatrix(Matrix** res, int rows, int columns){
    if(rows <= 0 || columns <= 0){
        fprintf(stderr, "Improper matrix dimensions.\n");
        return 0;
    }

    Matrix* buffer = (Matrix *) malloc(sizeof(Matrix));
    if(buffer == NULL){
        fprintf(stderr, "Memory allocation failed.\n");
        return 0;
    }
    buffer->matrixRows = rows; buffer->matrixColumns = columns;

    if(buffer->matrixRows <= SMALL_OBJECT_AMOUNT && buffer->matrixColumns <= SMALL_OBJECT_AMOUNT){
        for(int i = 0; i < buffer->matrixRows; i++){
            for(int j = 0; j < buffer->matrixColumns; j++){
                buffer->smallMatrixArray[i][j] = 0;
            }
        }

        *res = buffer;
        return 1;
    }

    double** array = (double **) malloc(rows*sizeof(double *));
    if(array == NULL){
        fprintf(stderr, "Memory allocation failed.\n");
        return 0;
    }
    
    for(int i = 0; i < rows; i++){
        array[i] = (double *) calloc(columns, sizeof(double));
    }

    buffer->matrixArray = array;
    *res = buffer;

    return 1;
}

// CreateMatrix2 FUNCTION
// RETURNS MATRIX POINTER WITH SPECIFIED DIMENSIONS
// RETURNS NULL IF EITHER DIMENSION <= 0
//
// Matrix* m = CreateMatrix((int) rowDimension, (int) colDimension);
//
Matrix* CreateMatrix2(int rows, int columns){
    Matrix* m;
    if(!CreateMatrix(&m, rows, columns))
        return NULL;

    return m;
}

void FreeMatrix(Matrix* m){
    if(m->matrixRows > SMALL_OBJECT_AMOUNT || m->matrixColumns > SMALL_OBJECT_AMOUNT){
        for(int i = 0; i < m->matrixRows; i++)
            free(m->matrixArray[i]);
        free(m->matrixArray);
    }
    free(m);
}

double mGet(Matrix* m, int row, int col){
    if(m->matrixRows <= row || m->matrixColumns <= col){
        fprintf(stderr, "Index out of range.\n");
        return NAN;
    }

    if(m->matrixRows <= SMALL_OBJECT_AMOUNT && m->matrixColumns <= SMALL_OBJECT_AMOUNT){
        return m->smallMatrixArray[row][col];
    }

    return m->matrixArray[row][col];
}

int mSet(Matrix* m, int row, int col, double val){
    if(m->matrixRows <= row || m->matrixColumns <= col){
        fprintf(stderr, "Index out of range.\n");
        return 0;
    }

    if(m->matrixRows <= SMALL_OBJECT_AMOUNT && m->matrixColumns <= SMALL_OBJECT_AMOUNT){
        m->smallMatrixArray[row][col] = val;
        return 1;
    }

    m->matrixArray[row][col] = val;
    return 1;
}

// CreateIdentityMatrix FUNCTION
// DEFINE IDENTITY MATRIX AT SPECIFIED POINTER
//
// Matrix* identity;
// if(!CreateIdentityMatrix(&identity, (int) dimensions))
//   ErrorHandler();
//
int CreateIdentityMatrix(Matrix** res, int dimensions){
    if(dimensions <= 0){
        fprintf(stderr, "Improper matrix dimensions.\n");
        return 0;
    }

    Matrix* buffer;
    CreateMatrix(&buffer, dimensions, dimensions);
    for(int i = 0; i < dimensions; i++){
        //buffer->matrixArray[i][i] = 1;
        mSet(buffer, i, i, 1.0);
    }
    *res = buffer;

    return 1;
}

// CreateIdentityMatrix2 FUNCTION
// RETURN IDENTITY MATRIX FROM SPECIFIED DIMENSIONS
// RETURNS NULL IF DIMENSIONS ARE IMPROPER
//
// Matrix* identity = CreateIdentityMatrix2((int) dimensions);
//
Matrix* CreateIdentityMatrix2(int dimensions){
    Matrix* res;
    if(!CreateIdentityMatrix(&res, dimensions))
        return NULL;
    
    return res;
}

double absDouble(double a){
    if(a >= 0)
        return a;
    
    return -a;
}

// CompareMatrices FUNCTION
// COMPARES 2 SPECIFIED MATRICES
// RETURNS 1 IF IDENTICAL, 0 OTHERWISE
//
// if(!CompareMatrices((Matrix *) a, (Matrix *) b))
//   ErrorHandler();
//
int CompareMatrices(Matrix* a, Matrix* b){
    if(a->matrixRows != b->matrixRows || a->matrixColumns != b->matrixColumns){
        fprintf(stderr, "Improper matrix dimensions.\n");
        return 0;
    }

    for(int i = 0; i < a->matrixRows; i++){
        for(int j = 0; j < a->matrixColumns; j++){
            if(absDouble(mGet(a, i, j) - mGet(b, i, j)) > ZERO_APPROX){
                //printf("%d %d\n", i, j);
                return 0;
            }
        }
    }

    return 1;
}

// TransposeMatrix FUNCTION
// CALCULATE TRANSPOSE OF MATRIX AND SAVE TO SPECIFIED POINTER
//
// Matrix* m;
// if(!TransposeMatrix(&m, (Matrix *) a))
//   ErrorHandler();
// 
int TransposeMatrix(Matrix** res, Matrix* a){
    if(a->matrixRows <= 0 || a->matrixColumns <= 0){
        fprintf(stderr, "Improper matrix dimensions.\n");
        return 0;
    }

    Matrix* buffer = CreateMatrix2(a->matrixColumns, a->matrixRows);
    for(int i = 0; i < a->matrixRows; i++){
        for(int j = 0; j < a->matrixColumns; j++){
            //buffer->matrixArray[j][i] = a->matrixArray[i][j];
            mSet(buffer, j, i, mGet(a, i, j));
        }
    }
    *res = buffer;

    return 1;
}

// TransposeMatrix2 FUNCTION
// RETURN TRANSPOSE OF MATRIX
// RETURNS NULL IF EITHER DIMENSION <= 0
//
// Matrix* m = TransposeMatrix2((Matrix *) a);
//
Matrix* TransposeMatrix2(Matrix* a){
    Matrix* res;
    if(!TransposeMatrix(&res, a))
        return NULL;

    return res;
}

// MatrixToString FUNCTION
// PRINT CONTENTS OF MATRIX TO STANDARD OUTPUT 
//
// if(!MatrixToString((Matrix *) m))
//   ErrorHandler();
//
int MatrixToString(Matrix* m){
    if(m->matrixRows <= 0 || m->matrixColumns <= 0){
        fprintf(stderr, "Improper matrix dimensions.\n");
        return 0;
    }

    int digitsToDisplay = 10;
    int decimalPlaces = 3;
    for(int i = 0; i < m->matrixRows; i++){
        printf("| ");
        for(int j = 0; j < m->matrixColumns; j++){
            if(j != 0)
                printf(", ");
            
            //printf("%*.*G", digitsToDisplay, decimalPlaces, m->matrixArray[i][j]);
            printf("%*.*G", digitsToDisplay, decimalPlaces, mGet(m, i, j));
        }
        printf(" |\n");
    }

    return 1;
}

// ArrayToMatrix FUNCTION
// DEFINE MATRIX WITH POSITIVE DIMENSIONS AT SPECIFIED POINTER
// FROM 2D ARRAY OF TYPE DOUBLE
//
// Matrix* newMatrix;
// if(!ArrayToMatrix(&newMatrix, (double **) array, (int) arrayRows, (int) arrayColumns))
//   errorHandler(); 
//
int ArrayToMatrix(Matrix** res, double** array, int arrayRows, int arrayColumns){
    Matrix* buffer;
    if(!CreateMatrix(&buffer, arrayRows, arrayColumns)){
        fprintf(stderr, "Error during new matrix creation.\n");
        return 0;
    }

    if(buffer->matrixRows <= SMALL_OBJECT_AMOUNT && buffer->matrixColumns <= SMALL_OBJECT_AMOUNT)
        for(int i = 0; i < arrayRows; i++){
            for(int j = 0; j < arrayColumns; j++){
                mSet(buffer, i, j, array[i][j]);
            }
        }
    else
        memcpy(buffer->matrixArray, array, arrayColumns*arrayRows*sizeof(double));
    *res = buffer;

    return 1;
}

// DuplicateMatrix FUNCTION
// RETURNS POINTER TO DUPLICATE OF GIVEN MATRIX
// NULL IF ERROR
//
// Matrix* duplicate = DuplicateMatrix((Matrix *) m);
//
Matrix* DuplicateMatrix(Matrix* m){
    Matrix* res;
    if(!CreateMatrix(&res, m->matrixRows, m->matrixColumns))
        return NULL;
    
    for(int i = 0; i < m->matrixRows; i++){
        for(int j = 0; j < m->matrixColumns; j++){
            //res->matrixArray[i][j] = m->matrixArray[i][j];
            mSet(res, i, j, mGet(m, i, j));
        }
    }

    return res;
}

// SumMatrices FUNCTION
// CALCULATES SUM OF 2 MATRICES TO A SPECIFIED POINTER
//
// Matrix* sum;
// if(!SumMatrices(&sum, (Matrix *) a, (Matrix *) b))
//   ErrorHandler();
//
int SumMatrices(Matrix** res, Matrix* a, Matrix* b){
    if(a->matrixRows != b->matrixRows || a->matrixColumns != b->matrixColumns || a->matrixRows <= 0 || a->matrixColumns <= 0){
        fprintf(stderr, "Improper matrix dimensions.\n");
        return 0;
    }

    Matrix* buffer;
    if(!CreateMatrix(&buffer, a->matrixRows, a->matrixColumns))
        return 0;
    
    
    for(int i = 0; i < a->matrixRows; i++){
        for(int j = 0; j < a->matrixColumns; j++){
            //buffer->matrixArray[i][j] = a->matrixArray[i][j] + b->matrixArray[i][j];
            mSet(buffer, i, j, mGet(a, i, j) + mGet(b, i, j));
        }
    }
    *res = buffer;

    return 1;
}

// SumMatrices2 FUNCTION
// RETURNS SUM OF 2 MATRICES
//
// Matrix* sum = SumMatrices((Matrix *) a, (Matrix *) b);
//
Matrix* SumMatrices2(Matrix* a, Matrix* b){
    Matrix* sum;
    if(!SumMatrices(&sum, a, b))
        return NULL;
    
    return sum;
}

// MultiplyMatrices FUNCTION
// TRADITIONAL FUNCTION MULTIPLICATION FOR 2 MATRICES
//
// Matrix* product;
// if(!MultiplyMatrices(&product, (Matrix *) a, (Matrix *) b))
//   ErrorHandler();
//
int MultiplyMatrices(Matrix** res, Matrix* a, Matrix* b){
    if(a->matrixColumns != b->matrixRows || a->matrixRows <= 0 || a->matrixColumns <= 0 || b->matrixColumns <= 0){
        fprintf(stderr, "Improper matrix dimensions.\n");
        return 0;
    }

    Matrix* buffer;
    if(!CreateMatrix(&buffer, a->matrixRows, b->matrixColumns))
        return 0;
    
    for (int i = 0; i < a->matrixRows; i++) {
        for (int j = 0; j < b->matrixColumns; j++) {
            //buffer->matrixArray[i][j] = 0;
            double new = 0.0;
            for (int k = 0; k < a->matrixColumns; k++) {
                //buffer->matrixArray[i][j] += a->matrixArray[i][k] * b->matrixArray[k][j];
                new += mGet(a, i, k) * mGet(b, k, j);
            }
            mSet(buffer, i, j, new);
        }
    }
    *res = buffer;

    return 1;
}

// MultiplyMatrices2 FUNCTION
// RETURNS PRODUCT OF TRADITIONAL MULTIPLICATION FOR 2 MATRICES
//
// Matrix* product = MultiplyMatrices2((Matrix *) a, (Matrix *) b);
//
Matrix* MultiplyMatrices2(Matrix* a, Matrix* b){
    Matrix* res;
    if(!MultiplyMatrices(&res, a, b))
        return NULL;
    
    return res;
}

// MatrixScalarMultiplication FUNCTION
// CALCULATES SCALAR MATRIX MULTIPLICATION SAVED TO SPECIFIED MATRIX
//
// Matrix* scalarProduct;
// if(!MatrixScalarMultiplication(&scalarProduct, (Matrix *) a, (double) scalar))
//   ErrorHandler();
//
int MatrixScalarMultiplication(Matrix** res, Matrix* a, const double scalar){
    if(a->matrixColumns <= 0 || a->matrixRows <= 0){
        fprintf(stderr, "Improper matrix dimensions.\n");
        return 0;
    }

    Matrix* buffer = CreateMatrix2(a->matrixRows, a->matrixColumns);
    for (int i = 0; i < a->matrixRows; i++){
        for (int j = 0; j < a->matrixColumns; j++){
            //buffer->matrixArray[i][j] = a->matrixArray[i][j]*scalar;
            mSet(buffer, i, j, mGet(a, i, j)*scalar);
        }
    }
    *res = buffer;

    return 1;
}

// MatrixScalarMultiplication2 FUNCTION
// RETURNS SCALAR MATRIX MULTIPLICATION
//
// Matrix* scalarProduct = MatrixScalarMultiplication2((Matrix *) a, (double) scalar);
//
Matrix* MatrixScalarMultiplication2(Matrix* a, const double scalar){
    Matrix* res;
    if(!MatrixScalarMultiplication(&res, a, scalar))
        return NULL;

    return res;
}

// SwapMatrixRows FUNCTION
// SWAPS SPECIFIED ROWS IN GIVEN MATRIX
//
// if(!SwapMatrixRows((Matrix *) m, (int) row1, (int) row2))
//   ErrorHandler();
//
int SwapMatrixRows(Matrix* a, int row1, int row2){
    if(a->matrixColumns <= 0 || a->matrixRows <= 0 || a->matrixRows <= MAX(row1, row2)){
        fprintf(stderr, "Improper matrix dimensions.\n");
        return 0;
    }

    for(int i = 0; i < a->matrixColumns; i++){
        //int buffer = a->matrixArray[row1][i];
        //a->matrixArray[row1][i] = a->matrixArray[row2][i];
        //a->matrixArray[row2][i] = buffer;

        double buffer = mGet(a, row1, i);
        mSet(a, row1, i, mGet(a, row2, i));
        mSet(a, row2, i, buffer);
    }

    return 1;
}

// MultiplyRow MATRIX
// MULTIPLY ROW OF MATRIX BY SCALAR VALUE
//
// if(!MultiplyRow((Matrix *) a, (int) row, (int) scalar))
//   ErrorHandler();
//
int MultiplyRow(Matrix* a, int row, double scalar){
    if(isnan(scalar)){
        fprintf(stderr, "Improper scalar value.\n");
        return 0;
    }
    if(a->matrixColumns <= 0 || a->matrixRows <= MAX(row, 0) || row < 0){
        fprintf(stderr, "Improper matrix dimensions.\n");
        return 0;
    }

    for(int i = 0; i < a->matrixColumns; i++){
        //a->matrixArray[row][i] *= scalar;
        mSet(a, row, i, mGet(a, row, i)*scalar);
    }

    return 1;
}

// AddRow FUNCTION
// ADD THE CONTENTS OF ONE MATRIX ROW ONTO ANOTHER
// (Effectively: matrix[row1] += matrix[row2])
//
// if(!AddRow((Matrix *) a, (int) rowBase, (int) rowSecondary))
//   ErrorHandler();
//
int AddRow(Matrix* a, int rowBase, int rowSecondary){
    if(a->matrixColumns <= 0 || a->matrixRows <= 0 || a->matrixRows <= rowBase || a->matrixRows <= rowSecondary || rowBase < 0 || rowSecondary < 0){
        fprintf(stderr, "Improper matrix dimensions.\n");
        return 0;
    }

    for(int i = 0; i < a->matrixColumns; i++){
        //a->matrixArray[rowBase][i] += a->matrixArray[rowSecondary][i];
        double new = mGet(a, rowBase, i) + mGet(a, rowSecondary, i);
        mSet(a, rowBase, i, new);
    }

    return 1;
}

// DeterminantRecursive FUNCTION 
// RECURSIVELY CALCULATES DETERMINANT OF GIVEN MATRIX 
// RETURNS DOUBLE AND PRINTS ERROR IF IMPROPER DIMENSIONS
//
// double determinant = DeterminantRecursive((Matrix *) m);
// 
double DeterminantRecursive(Matrix* m){
    if(m->matrixRows != m->matrixColumns || m->matrixColumns <= 0){
        fprintf(stderr, "Improper matrix dimensions.\n");
        return 0.0;
    }
    if(m->matrixColumns == 1){
        //return m->matrixArray[0][0];
        return mGet(m, 0, 0);
    }
    if(m->matrixColumns == 2){
        //return m->matrixArray[0][0]*m->matrixArray[1][1] - m->matrixArray[1][0]*m->matrixArray[0][1];
        return mGet(m, 0, 0)*mGet(m, 1, 1) - mGet(m, 1, 0)*mGet(m, 0, 1);
    }

    double base = 0;
    for(int i = 0; i < m->matrixColumns; i++){
        //if(m->matrixArray[0][i] == 0)
        if(mGet(m, 0, i) == 0)
            continue;
        
        Matrix* subM = CreateMatrix2(m->matrixRows-1, m->matrixRows-1);
        int x = 0; int y = 0;
        for(int j = 1; j < m->matrixRows; j++){
            for(int k = 0; k < m->matrixColumns; k++){
                if(k == i)
                    continue;
                
                //subM->matrixArray[y][x] = m->matrixArray[j][k];
                mSet(subM, y, x, mGet(m, j, k));
                x++;
            }
            y++; x = 0;
        }

        //base += pow(-1,i) * m->matrixArray[0][i] * DeterminantRecursive(subM);
        base += pow(-1,i) * mGet(m, 0, i) * DeterminantRecursive(subM);
    }

    return base;
}

// FindNonZeroInColumn() FUNCTION
// FINDS FIRST NON ZERO ENTRY IN SPECIFIED MATRIX COLUMN (TOP TO BOTTOM)
// STARTS SEARCH AT SPECIFIED STARTING ROW
// RETURNS -1 IF ERROR OR ALL ENTRIES ARE 0
//
int FindNonZeroInColumn(Matrix* m, int column, int startingRow){
    if(m->matrixColumns <= 0 || m->matrixRows <= 0 || column < 0 || column >= m->matrixColumns || startingRow < 0 ||startingRow >= m->matrixRows){
        fprintf(stderr, "Improper matrix dimensions.\n");
        return -1;
    }

    for(int i = startingRow; i < m->matrixRows; i++){
        //if(m->matrixArray[i][column] != 0)
        if(mGet(m, i, column) != 0)
            return i;
    }

    return -1;
}

// InvertSquareMarix FUNCTION
// INVERTS SQUARE MATRIX AND ASSIGNS RESULT TO SPECIFIED POINTER
// 
// Matrix* inverse;
// if(!InvertSquareMatrix(&inv, (Matrix *) m))
//   ErrorHandler();
//
int InvertSquareMatrix(Matrix** res, Matrix* m){
    if(m->matrixColumns <= 0 || m->matrixRows <= 0 || m->matrixColumns != m->matrixRows){
        fprintf(stderr, "Improper matrix dimensions.\n");
        return 0;
    }

    Matrix* identity = CreateIdentityMatrix2(m->matrixRows);
    Matrix* original = DuplicateMatrix(m);
    if(original == NULL){
        fprintf(stderr, "Error during duplication.\n");
        return 0;
    }    

    for(int i = 0; i < m->matrixColumns; i++){
        // Ensure diagonal entry is non zero
        //if(original->matrixArray[i][i] == 0){
        if(mGet(original, i, i) == 0){
            int RowToSwap = FindNonZeroInColumn(original, i, i);
            if(RowToSwap == -1){
                fprintf(stderr, "Matrix is noninvertible.\n");
                return 0;
            }

            SwapMatrixRows(original, i, RowToSwap);
            SwapMatrixRows(identity, i, RowToSwap);
        }

        // Reduce all other values at given row
        for(int y = 0; y < m->matrixRows; y++){
            //if(y == i || original->matrixArray[y][i] == 0)
            if(y == i || mGet(original, y, i) == 0)
                continue;
            
            //double scalarMultiple = -1.0*original->matrixArray[y][i]/original->matrixArray[i][i];
            double scalarMultiple = -1.0*mGet(original, y, i)/mGet(original, i, i);
            MultiplyRow(original, i, scalarMultiple);
            MultiplyRow(identity, i, scalarMultiple);
            AddRow(original, y, i);
            AddRow(identity, y, i);
        }
    }

    // Reduce diagonal entries to 1
    for(int i = 0; i < m->matrixRows; i++){
        //double scalarMultiple = 1.0/original->matrixArray[i][i];
        double scalarMultiple = 1.0/mGet(original, i, i);
        MultiplyRow(original, i, scalarMultiple);
        MultiplyRow(identity, i, scalarMultiple);
    }

    *res = identity;
    FreeMatrix(original);

    return 1;
}

// InvertSquareMatrix2 FUNCTION
// RETURNS INVERTED MATRIX FROM SPECIFIED MATRIX
// RETURNS NULL IF ERROR
// 
// Matrix* inverse = InvertSquareMatrix2((Matrix *) m);
//
Matrix* InvertSquareMatrix2(Matrix* m){
    Matrix* res;
    if(!InvertSquareMatrix(&res, m))
        return NULL;

    return res;
}

// MultiplyMatrixVector() FUNCTION
// CALCULATE MATRIX*VECTOR MULTIPLICATION TO SPECIFIED VECTOR POINTER
//
// Vector* product;
// if(!MultiplyMatrixVector(&product, (Matrix *) m, (Vector *) v))
//   ErrorHandler();
//
int MultiplyMatrixVector(Vector** res, Matrix* m, Vector* v){
    if(m->matrixColumns <= 0 || m->matrixRows <= 0 || m->matrixColumns != v->vectorDimension){
        fprintf(stderr, "Improper matrix/vector dimensions.\n");
        return 0;
    }

    Vector* buffer;
    if(!CreateVector(&buffer, m->matrixRows))
        return 0;
    
    for(int i = 0; i < m->matrixRows; i++){
        double resultAtIndex = 0.0;
        for(int j = 0; j < m->matrixColumns; j++){
            //resultAtIndex += m->matrixArray[i][j] * vGet(v, j);
            resultAtIndex += mGet(m, i, j) * vGet(v, j);
        }

        //buffer->vectorArray[i] = resultAtIndex;
        vSet(buffer, i, resultAtIndex);
    }
    *res = buffer;

    return 1;
}

// MultiplyMatrixVector2() FUNCTION
// RETURN VECTOR POINTER FOR MATRIX*VECTOR PRODUCT
// RETURNS NULL IF ERROR OR MISALIGNED DIMENSIONS
//
// Vector* product = MultiplyMatrixVector2((Matrix *) m, (Vector *) v);
//
Vector* MultiplyMatrixVector2(Matrix* m, Vector* v){
    Vector* res;
    if(!MultiplyMatrixVector(&res, m, v))
        return NULL;
    
    return res;
}

// MoorePenroseInverse FUNCTION
// CALCULATES MOORE PENROSE INVERSE OF MATRIX
// 
// Matrix* MPInverse;
// if(!MoorePenroseInverse(&MPInverse, (Matrix *) m))
//   ErrorHandler();
//
int MoorePenroseInverse(Matrix** res, Matrix* m){
    if(m->matrixColumns <= 0 || m->matrixRows <= 0){
        fprintf(stderr, "MoorePenroseInverse: Improper matrix dimensions.\n");
        return 0;
    }

    Matrix* mT;
    if(!TransposeMatrix(&mT, m))
        return 0;
    
    
    if(m->matrixRows <= m->matrixColumns){
        Matrix* mmT, *mmTInv, *mp;
        if(!MultiplyMatrices(&mmT, m, mT)){
            fprintf(stderr, "MP Inverse Error: Multiplying matrices.\n");
            return 0;
        }
        
        if(!InvertSquareMatrix(&mmTInv, mmT)){
            fprintf(stderr, "MP Inverse Error: Inverting matrix.\n");
            return 0;
        }

        if(!MultiplyMatrices(&mp, mT, mmTInv)){
            fprintf(stderr, "MP Inverse Error: Multiplying matrices.\n");
            return 0;
        }
        
        *res = mp;
        FreeMatrix(mT); FreeMatrix(mmT); FreeMatrix(mmTInv);

        return 1;
    }

    Matrix* mTm, *mTmInv, *mp2;
    if(!MultiplyMatrices(&mTm, mT, m)){
        fprintf(stderr, "MP Inverse Error: Multiplying matrices.\n");
        return 0;
    }
    
    if(!InvertSquareMatrix(&mTmInv, mTm)){
        fprintf(stderr, "MP Inverse Error: Inverting matrix.\n");
        return 0;
    }

    if(!MultiplyMatrices(&mp2, mTmInv, mT)){
        fprintf(stderr, "MP Inverse Error: Multiplying matrices.\n");
        return 0;
    }

    *res = mp2;
    FreeMatrix(mT); FreeMatrix(mTm); FreeMatrix(mTmInv);

    return 1;    
}

// SubstituteVectorInMatrix FUNCTION
// SUBSTITUTES ROW OR COLUMN IN MATRIX WITH THE SPECIFIED
// VECTOR AT GIVEN INDEX
//
// int MatrixSubstitutionIndex = 2;
// int vType = ROW;  (ROW = 0, COLUMN = 1)
// if(!SubstituteVectorInMatrix((Matrix *) m, (Vector *) v, MatrixSubstitutionIndex, vType))
//   ErrorHandler();
//
int SubstituteVectorInMatrix(Matrix* m, Vector* v, int it, int vectorType){
    if(m->matrixColumns <= 0 || m->matrixRows <= 0 || v->vectorDimension <= 0){
        fprintf(stderr, "SubstituteVectorInMatrix: Improper matrix/vector dimensions.\n");
        return 0;
    }

    // Vector to matrix row
    if(vectorType == ROW){
        if(m->matrixRows <= it || m->matrixColumns != v->vectorDimension){
            fprintf(stderr, "SubstituteVectorInMatrix: Misaligned dimensions.\n");
            return 0;
        }

        for(int i = 0; i < m->matrixColumns; i++){
            //m->matrixArray[it][i] = v->vectorArray[i];
            //m->matrixArray[it][i] = vGet(v, i);
            mSet(m, it, i, vGet(v, i));
        }

        return 1;
    }

    // Vector to matrix column
    if(m->matrixColumns <= it || m->matrixRows != v->vectorDimension){
        fprintf(stderr, "SubstituteVectorInMatrix: Misaligned dimensions.\n");
        return 0;
    }

    for(int i = 0; i < m->matrixRows; i++){
        //m->matrixArray[i][it] = v->vectorArray[i];
        //m->matrixArray[i][it] = vGet(v, i);
        mSet(m, i, it, vGet(v, i));
    }

    return 1;
}

int MatrixToVectorArray(Vector*** res, Matrix* m, int vectorType){
    if(m->matrixColumns <= 0 || m->matrixRows <= 0){
        fprintf(stderr, "MatrixToVectorArray: Improper matrix dimensions.\n");
        return 0;
    }

    // if matrix to row vectors
    if(vectorType == ROW){
        Vector** array = (Vector **) malloc(m->matrixRows*sizeof(Vector *));
        for(int i = 0; i < m->matrixRows; i++){
            if(!CreateVector(&array[i], m->matrixColumns))
                return 0;

            for(int j = 0; j < m->matrixColumns; j++){
                //array[i]->vectorArray[j] = m->matrixArray[i][j];
                //vSet(array[i], j, m->matrixArray[i][j]);
                vSet(array[i], j, mGet(m, i, j));
            }
        }
        *res = array;

        return 1;
    }

    // if matrix to column vectors
    Vector** array = (Vector **) malloc(m->matrixColumns*sizeof(Vector *));
    for(int i = 0; i < m->matrixColumns; i++){
        if(!CreateVector(&array[i], m->matrixRows))
            return 0;

        for(int j = 0; j < m->matrixRows; j++){
            //array[i]->vectorArray[j] = m->matrixArray[j][i];
            //vSet(array[i], j, m->matrixArray[j][i]);
            vSet(array[i], j, mGet(m, j, i));
        }
    }
    *res = array;

    return 1;
}

// VectorArrayToMatrix FUNCTION
// RETURNS MATRIX FROM VECTOR ARRAY OF SPECIFIED VECTOR TYPE
// (ROW = 0, COLUMN = 1)
//
// Matrix* m;
// if(!VectorArrayToMatrix(&m, (Vector **) vectorArray, COLUMN))
//   ErrorHandler();
// 
int VectorArrayToMatrix(Matrix** res, Vector** vectorArr, int arrayLength, int vectorType){
    int vectorDims = vectorArr[0]->vectorDimension;
    if(arrayLength <= 0 || vectorDims <= 0){
        fprintf(stderr, "VectorArrayToMatrix: Improper array length.\n");
        return 0;
    }

    // if row vectors to matrix
    if(vectorType == ROW){
        Matrix* buffer = NULL;
        if(!CreateMatrix(&buffer, arrayLength, vectorDims))
            return 0;
        
        for(int i = 0; i < arrayLength; i++){
            if(vectorArr[i]->vectorDimension != vectorDims){
                fprintf(stderr, "VectorArrayToMatrix: Matrix dimensions differ.\n");
                FreeMatrix(buffer);
                return 0;
            }

            for(int j = 0; j < vectorDims; j++){
                //buffer->matrixArray[i][j] = vectorArr[i]->vectorArray[j];
                //buffer->matrixArray[i][j] = vGet(vectorArr[i], j);
                mSet(buffer, i, j, vGet(vectorArr[i], j));
            }
        }
        *res = buffer;

        return 1;
    }

    // if column vectors to matrix
    Matrix* buffer = NULL;
    if(!CreateMatrix(&buffer, vectorDims, arrayLength))
        return 0;
    
    for(int i = 0; i < arrayLength; i++){
        if(vectorArr[i]->vectorDimension != vectorDims){
            fprintf(stderr, "VectorArrayToMatrix: Matrix dimensions differ.\n");
            FreeMatrix(buffer);
            return 0;
        }

        for(int j = 0; j < vectorDims; j++){
            //buffer->matrixArray[j][i] = vectorArr[i]->vectorArray[j];
            //buffer->matrixArray[j][i] = vGet(vectorArr[i], j);
            mSet(buffer, j, i, vGet(vectorArr[i], j));
        }
    }
    *res = buffer;

    return 1;
}

// QRDecomposition FUNCTION
// CALCULATES QR DECOMPOSITION OF SPECIFIED MATRIX
//
// Matrix *Q, *R;
// if(!QRDecomposition(&Q, &R, (Matrix *) m))
//   ErrorHandler();
//
int QRDecomposition(Matrix** Q, Matrix** R, Matrix* m){
    if(m->matrixRows <= 0 || m->matrixColumns <= 0){
        fprintf(stderr, "QRDecomposition: Improper matrix dimensions.\n");
        return 0;
    }

    int cols = m->matrixColumns;

    Vector** aVectors = NULL;
    if(!MatrixToVectorArray(&aVectors, m, COLUMN))
        return 0;
    
    // determining u vectors
    Vector** uVectors = (Vector **) malloc(cols*sizeof(Vector *));    
    Vector** eVectors = (Vector **) malloc(cols*sizeof(Vector *));
    for(int i = 0; i < cols; i++){
        Vector* u = NULL;                           
        if(!DuplicateVector(&u, aVectors[i]))       // ui = ai
            return 0;

        // subtract projections
        Vector *offset = NULL;
        if(!CreateVector(&offset, u->vectorDimension))
            return 0;
        
        for(int j = 0; j < i; j++){
            Vector* projUjAi = NULL;
            if(!ProjectVector(&projUjAi, uVectors[j], aVectors[i]))     // calculate projection
                return 0;
            
            if(!SumVectors(offset, projUjAi))                           // add to offset
                return 0;

            FreeVector(projUjAi);
        }

        if(!SubtractVectors(u, offset))             // apply offset (ui - sum[proj(uj, ai)])
            return 0;
        
        FreeVector(offset);

        uVectors[i] = u;
        double uLength = VectorLength(u);
        eVectors[i] = MultiplyVector2(u, 1.0/uLength);
    }

    Matrix* QBuffer = NULL;
    if(!VectorArrayToMatrix(&QBuffer, eVectors, cols, COLUMN))      // finalize Q matrix
        return 0;
    
    *Q = QBuffer;
    //MatrixToString(QBuffer);

    Matrix* RBuffer = NULL;
    if(!CreateMatrix(&RBuffer, cols, cols))
        return 0;

    for(int i = 0; i < cols; i++){
        for(int j = i; j < cols; j++){
            //RBuffer->matrixArray[i][j] = DotProduct2(eVectors[i], aVectors[j]);
            mSet(RBuffer, i, j, DotProduct2(eVectors[i], aVectors[j]));
        }
    }
    *R = RBuffer;
    //MatrixToString(RBuffer);

    
    for(int i = 0; i < cols; i++){
        FreeVector(eVectors[i]); FreeVector(aVectors[i]); FreeVector(uVectors[i]);
    }
    free(eVectors); free(aVectors); free(uVectors);

    return 1;
}