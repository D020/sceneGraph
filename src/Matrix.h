#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#ifndef __Matrix_h_
#define __Matrix_h_

class Matrix {
	int rows;
	int cols;
	float* entries;
public:
	Matrix();
	Matrix(const Matrix& other); //Copy constructor
	Matrix& operator=(const Matrix& other); //Assignment operator
	Matrix(int row, int col);
	Matrix(float,float,float); //3-Vector constructor
	Matrix(float,float,float,float); //4-Vector constructor
	~Matrix ();

	//setters and getters
	void setEntry(int row, int col, float value);
	float getEntry(int row,int col) const;

	int getRows();
	int getCols();

	Matrix getRow(int row) const;
	Matrix getCol(int col) const;
	void setRow(int row, Matrix singleRowMatrix);
	void setCol(int col, Matrix singleColMatrix);
	float x();
	float y();
	float z();
	
	const float* const getEntries();
	bool isSize(int row, int col);
	Matrix resize(int row, int col);

	//Arithmetics
	Matrix transpose() const;
	Matrix trans_invert() const;
	friend Matrix operator+(const Matrix& a, const Matrix& b);
	friend Matrix operator-(const Matrix& a, const Matrix& b);
	friend Matrix operator*(const Matrix& a, const Matrix& b);
	
	friend Matrix operator*(const Matrix& a, const float s);
	friend Matrix operator*(const float s, const Matrix& a);
	
	//Vector arithmetics
	friend float operator%(const Matrix& a, const Matrix& b);
	Matrix normalized() const;
	float length() const;
	Matrix clamped(float value);
	
	//Debug
	void print() const;
};

Matrix XMatrix(float theta); //4x4 Rotation matrix
Matrix YMatrix(float theta); //4x4 Rotation matrix
Matrix ZMatrix(float theta); //4x4 Rotation matrix
Matrix SMatrix(float x, float y, float z);
Matrix TMatrix(float x, float y, float z);
Matrix PMatrix(float n, float f, float l, float r, float t, float b);

#endif
