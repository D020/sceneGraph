#include "Matrix.h"
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

//Constructors and assignment operators

Matrix::Matrix(){ //Default constructor, no entries.
								rows = 0;
								cols = 0;
								entries = NULL;
}

Matrix::Matrix(const Matrix& other){ //Copy constructor
								rows = other.rows;
								cols = other.cols;
								entries = (float*) calloc(rows*cols,sizeof(float));
								memcpy(entries,other.entries,rows*cols*sizeof(float));
}


Matrix& Matrix::operator=(const Matrix& other){ //Assignment operator
								rows = other.rows;
								cols = other.cols;
								entries = (float*) realloc(entries,rows*cols*sizeof(float));
								memcpy(entries,other.entries,rows*cols*sizeof(float));
								return *this;
}

Matrix::Matrix (int row, int col) { //Regular constructor all entries 0
								rows = row;
								cols = col;
								entries = (float*) calloc(rows*cols,sizeof(float));
};

Matrix::Matrix (float x, float y, float z) { //3 Vector constructor
								rows = 3;
								cols = 1;
								entries = (float*) calloc(rows*cols,sizeof(float));
								entries[0] = x;
								entries[1] = y;
								entries[2] = z;
};

Matrix::Matrix (float x, float y, float z, float q) { //4 Vector constructor
								rows = 4;
								cols = 1;
								entries = (float*) calloc(rows*cols,sizeof(float));
								entries[0] = x;
								entries[1] = y;
								entries[2] = z;
								entries[3] = q;
};

//Getters and setters
void Matrix::setEntry(int row, int col, float value){
								if(0<=row && 0<=col && row<rows && col<cols)
																entries[row+col*rows] = value;
								else
																printf("setEntry:Out of bounds\n");

}

float Matrix::getEntry(int row,int col) const {
								if(0<=row && 0<= col && row<rows && col<cols)
																return entries[row+col*rows];
								else{
																printf("getEntry:Out of bounds\n");
																return 0;
								}

}

int Matrix::getRows(){
								return rows;
}

int Matrix::getCols(){
								return rows;
}


Matrix Matrix::getRow(int row) const {
								Matrix result;
								if(0<=row && row<rows) {
																result = Matrix(1,cols);
																for(int i=0; i<cols; i++)
																								result.entries[i] = entries[row+i*rows];
								}
								else{
																printf("getRow:Out of bounds\n");
								}
								return result;
}

Matrix Matrix::getCol(int col) const {
								Matrix result;
								if(0<=col && col<cols) {
																result = Matrix(rows,1);
																for(int i=0; i<rows; i++)
																								result.entries[i] = entries[i+col*rows];
								}
								else{
																printf("getCol:Out of bounds\n");
								}
								return result;
}

void Matrix::setRow(int row, Matrix singleRowMatrix){
								if(singleRowMatrix.rows==1 && singleRowMatrix.cols == cols && 0<=row && row<rows) {
																for(int i=0; i<cols; i++)
																								entries[i*rows + row] = singleRowMatrix.entries[i];
								}
								else{
																printf("setRow Matrix dimensions mismatch or out of bounds rownumber\n");
								}
}

void Matrix::setCol(int col, Matrix singleColMatrix){
								if(singleColMatrix.cols==1 && singleColMatrix.rows == rows && 0<=col && col<cols) {
																for(int i=0; i<rows; i++)
																								entries[i + col*rows] = singleColMatrix.entries[i];
								}
								else{
																printf("setCol Matrix dimensions mismatch or out of bounds colnumber\n");
								}
}

const float* const Matrix::getEntries(){
								return entries;
}

bool Matrix::isSize(int row, int col){
								return (row==rows && col==cols);
}

Matrix Matrix::resize(int row, int col){
								Matrix result(row,col);
								for(int i=0; i<MIN(row,rows); i++) {
																for(int j=0; j<MIN(col,cols); j++) {
																								result.setEntry(i,j,getEntry(i,j));
																}
								}
								return result;
}

float Matrix::x(){
								return getEntry(0,0);
}
float Matrix::y(){
								return getEntry(1,0);
}
float Matrix::z(){
								return getEntry(2,0);
}
//Arithmetics
Matrix Matrix::transpose() const {
								Matrix result(cols,rows);
								for(int col=0; col<cols; col++) {
																for(int row=0; row<rows; row++) {
																								result.entries[col+row*cols] = entries[row+col*rows];
																}
								}
								return result;
}

Matrix Matrix::trans_invert() const {
								Matrix result(4,4);
								if(result.rows == rows && result.cols == cols) {
																result = *this;
																result.setEntry(0,3,-result.getEntry(0,3));
																result.setEntry(1,3,-result.getEntry(1,3));
																result.setEntry(2,3,-result.getEntry(2,3));
																result.setEntry(3,3,1.0);

								}
								else{
																printf("tr_inv Matrix dimensions mismatch\n");
								}
								return result;
}

Matrix operator+(const Matrix& a, const Matrix& b){
								Matrix result;
								if(a.rows == b.rows && a.cols == b.cols) {
																result = Matrix(a.rows,a.cols);
																for(int col=0; col<a.cols; col++) {
																								for(int row=0; row<a.rows; row++) {
																																int index = row+col*a.rows;
																																result.entries[index] = a.entries[index] + b.entries[index];
																								}
																}
								}
								else{
																printf("+Matrix dimensions mismatch\n");
								}
								return result;
}

Matrix operator-(const Matrix& a, const Matrix& b){
								Matrix result;
								if(a.rows == b.rows && a.cols == b.cols) {
																result = Matrix(a.rows,a.cols);
																for(int col=0; col<a.cols; col++) {
																								for(int row=0; row<a.rows; row++) {
																																int index = row+col*a.rows;
																																result.entries[index] = a.entries[index] - b.entries[index];
																								}
																}
								}
								else{
																printf("-Matrix dimensions mismatch\n");
								}
								return result;
}

Matrix operator*(const Matrix& a, const Matrix& b){
								Matrix result;
								if(a.cols == b.rows) {
																result = Matrix(a.rows,b.cols);
																for(int col=0; col<b.cols; col++) {
																								for(int row=0; row<a.rows; row++) {
																																float entry = 0;
																																for(int i=0; i<b.rows; i++)
																																								entry += a.entries[row+i*a.rows] * b.entries[i+col*b.rows];
																																result.entries[row+col*a.rows] = entry;
																								}
																}
								}
								else{
																printf("*Matrix dimensions mismatch\n");
								}
								return result;
}

Matrix operator*(const Matrix& a, const float s){
								Matrix result(a.rows,a.cols);
								for(int col=0; col<a.cols; col++) {
																for(int row=0; row<a.rows; row++) {
																								int index = row+col*a.rows;
																								result.entries[index] = a.entries[index] * s;
																}
								}
								return result;
}

Matrix operator*(const float s,const Matrix& a){
								Matrix result(a.rows,a.cols);
								for(int col=0; col<a.cols; col++) {
																for(int row=0; row<a.rows; row++) {
																								int index = row+col*a.rows;
																								result.entries[index] = a.entries[index] * s;
																}
								}
								return result;
}

//Vector arithmetics

float operator%(const Matrix& a, const Matrix& b){
								float result = 0;
								if(a.cols==1 && b.cols==1 && a.rows == b.rows) {
																for(int i=0; i<a.rows; i++) {
																								result += a.entries[i] * b.entries[i];
																}
								}
								else{
																printf("%%Matrix dimensions mismatch\n");
								}
								return result;
}

float Matrix::length() const {
								float result = 0;
								if(cols==1) {
																for(int i=0; i<rows; i++)
																								result += entries[i]*entries[i];
																result = sqrt(result);
								}
								else
																printf("Matrix is not a vector, can't find length\n");
								return result;
}

Matrix Matrix::normalized() const {
								Matrix result;
								if(cols==1)
																return *this * (1 / this->length());
								else
																printf("Matrix is not a vector, can't normalize\n");
								return result;
}

Matrix Matrix::clamped(float value){
								Matrix result;
								if(cols==1) {
																result = *this;
																for(int i=0; i<rows; i++)
																								result.entries[i] = MIN(value,(*this).entries[i]);
								}
								else
																printf("Matrix is not a vector, can't clamp\n");
								return result;
}

//Not in class
Matrix XMatrix(float theta){ //4x4 Rotation matrix
								Matrix result(4,4);
								result.setEntry(0,0,1.0);  result.setEntry(0,1,0.0);    result.setEntry(0,2,0.0);   result.setEntry(0,3,0.0);
								result.setEntry(1,0,0.0);  result.setEntry(1,1,cos(theta));  result.setEntry(1,2,-sin(theta)); result.setEntry(1,3,0.0);
								result.setEntry(2,0,0.0);  result.setEntry(2,1,sin(theta));  result.setEntry(2,2,cos(theta)); result.setEntry(2,3,0.0);
								result.setEntry(3,0,0.0); result.setEntry(3,1,0.0);   result.setEntry(3,2,0.0);   result.setEntry(3,3,1.0);
								return result;
}

Matrix YMatrix(float theta){ //4x4 Rotation matrix
								Matrix result(4,4);
								result.setEntry(0,0,cos(theta));  result.setEntry(0,1,0.0);  result.setEntry(0,2,sin(theta)); result.setEntry(0,3,0.0);
								result.setEntry(1,0,0.0);    result.setEntry(1,1,1.0);  result.setEntry(1,2,0.0);   result.setEntry(1,3,0.0);
								result.setEntry(2,0,-sin(theta));  result.setEntry(2,1,0.0);  result.setEntry(2,2,cos(theta)); result.setEntry(2,3,0.0);
								result.setEntry(3,0,0.0);   result.setEntry(3,1,0.0); result.setEntry(3,2,0.0);   result.setEntry(3,3,1.0);
								return result;
}

Matrix ZMatrix(float theta){ //4x4 Rotation matrix
								Matrix result(4,4);
								result.setEntry(0,0,cos(theta));  result.setEntry(0,1,-sin(theta));  result.setEntry(0,2,0.0); result.setEntry(0,3,0.0);
								result.setEntry(1,0,sin(theta));  result.setEntry(1,1,cos(theta));  result.setEntry(1,2,0.0); result.setEntry(1,3,0.0);
								result.setEntry(2,0,0.0);    result.setEntry(2,1,0.0);    result.setEntry(2,2,1.0); result.setEntry(2,3,0.0);
								result.setEntry(3,0,0.0);   result.setEntry(3,1,0.0);   result.setEntry(3,2,0.0); result.setEntry(3,3,1.0);
								return result;
}


Matrix SMatrix(float x, float y, float z){ //4x4 Scale matrix
								Matrix result(4,4);
								result.setEntry(0,0,x);  result.setEntry(0,1,0.0);  result.setEntry(0,2,0.0); result.setEntry(0,3,0.0);
								result.setEntry(1,0,0.0);  result.setEntry(1,1,y);  result.setEntry(1,2,0.0); result.setEntry(1,3,0.0);
								result.setEntry(2,0,0.0);  result.setEntry(2,1,0.0);  result.setEntry(2,2,z);  result.setEntry(2,3,0.0);
								result.setEntry(3,0,0.0); result.setEntry(3,1,0.0); result.setEntry(3,2,0.0); result.setEntry(3,3,1.0);
								return result;
}

Matrix TMatrix(float x, float y, float z){ //4x4 Translation matrix
								Matrix result(4,4);
								result.setEntry(0,0,1.0);  result.setEntry(0,1,0.0);  result.setEntry(0,2,0.0); result.setEntry(0,3,x);
								result.setEntry(1,0,0.0);  result.setEntry(1,1,1.0);  result.setEntry(1,2,0.0); result.setEntry(1,3,y);
								result.setEntry(2,0,0.0);  result.setEntry(2,1,0.0);  result.setEntry(2,2,1.0); result.setEntry(2,3,z);
								result.setEntry(3,0,0.0); result.setEntry(3,1,0.0); result.setEntry(3,2,0.0); result.setEntry(3,3,1.0);
								return result;
}

Matrix PMatrix(float n, float f, float l, float r, float t, float b){ //4x4 Perspective matrix
								Matrix result(4,4);
								result.setEntry(0,0,2*n/(r-l));  result.setEntry(0,1,0.0);  result.setEntry(0,2,(r+l)/(r-l)); result.setEntry(0,3,0);
								result.setEntry(1,0,0.0);  result.setEntry(1,1,2*n/(t-b));  result.setEntry(1,2,(t+b)/(t-b)); result.setEntry(1,3,0);
								result.setEntry(2,0,0.0);  result.setEntry(2,1,0.0);  result.setEntry(2,2,-(f+n)/(f-n)); result.setEntry(2,3,-2*f*n/(f-n));
								result.setEntry(3,0,0.0); result.setEntry(3,1,0.0); result.setEntry(3,2,-1.0); result.setEntry(3,3,0);
								return result;
}


void Matrix::print() const {
								for(int row=0; row<rows; row++) {
																for(int col=0; col<cols; col++)
																								printf("%.2f ",getEntry(row,col));
																printf("\n");
								}
}


Matrix::~Matrix (){
								//printf("Free\n");
								if(0<rows && 0<cols)
																free((void*)entries);
};
