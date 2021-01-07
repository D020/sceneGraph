#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"
#include <math.h>
#ifndef Quat_h
#define Quat_h
class Quat{
private:
	float entries[4];
public:
	Quat();
	Quat(float one, float i, float j, float k);
	Quat(Matrix axis, float angle);
	Quat(const Quat& other); //Copy constructor
	Quat& operator=(const Quat& other); //Assignment operator

	friend Quat operator+(const Quat& a, const Quat& b);
	friend Quat operator-(const Quat& a, const Quat& b);
	friend Quat operator*(const Quat& a, const Quat& b);

	Quat conjugate();
	float norm();
	Quat normalize();
	
	void print() const;
	Matrix toMatrix4();
	Matrix toMatrix3();
};



#endif
