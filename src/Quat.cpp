#include "Quat.h"

Quat::Quat(){
	entries[0] = 1;
	entries[1] = 0;
	entries[2] = 0;
	entries[3] = 0;
}
Quat::Quat(float one, float i, float j, float k){
	entries[0] = one;
	entries[1] = i;
	entries[2] = j;
	entries[3] = k;
}

Quat::Quat(Matrix axis, float angle){
	axis = axis.normalized();
	entries[0] = cos(angle/2);
	entries[1] = axis.x() * sin(angle/2);
	entries[2] = axis.y() * sin(angle/2);
	entries[3] = axis.z() * sin(angle/2);
}

Quat::Quat(const Quat& other){ //Copy constructor
	entries[0] = other.entries[0];
	entries[1] = other.entries[1];
	entries[2] = other.entries[2];
	entries[3] = other.entries[3];
}


Quat& Quat::operator=(const Quat& other){	//Assignment operator
	entries[0] = other.entries[0];
	entries[1] = other.entries[1];
	entries[2] = other.entries[2];
	entries[3] = other.entries[3];
	return *this;
}

Quat operator+(const Quat& a, const Quat& b){
	Quat result;
	result.entries[0] = a.entries[0] + b.entries[0];
	result.entries[1] = a.entries[1] + b.entries[1];
	result.entries[2] = a.entries[2] + b.entries[2];
	result.entries[3] = a.entries[3] + b.entries[3];
	return result;
}
Quat operator-(const Quat& a, const Quat& b){
	Quat result;
	result.entries[0] = a.entries[0] - b.entries[0];
	result.entries[1] = a.entries[1] - b.entries[1];
	result.entries[2] = a.entries[2] - b.entries[2];
	result.entries[3] = a.entries[3] - b.entries[3];
	return result;
}

Quat operator*(const Quat& a, const Quat& b){
	Quat result;
	float a1 = a.entries[0];	float a2 = b.entries[0];
	float b1 = a.entries[1];	float b2 = b.entries[1];
	float c1 = a.entries[2];	float c2 = b.entries[2];
	float d1 = a.entries[3];	float d2 = b.entries[3];
	
	result.entries[0] = a1*a2 - b1*b2 - c1*c2 - d1*d2;
	result.entries[1] = a1*b2 + b1*a2 + c1*d2 - d1*c2;
	result.entries[2] = a1*c2 - b1*d2 + c1*a2 + d1*b2;
	result.entries[3] = a1*d2 + b1*c2 - c1*b2 + d1*a2;
	return result;
}

Quat Quat::conjugate(){
	Quat result;
	result.entries[0] = entries[0];
	result.entries[1] = -entries[1];
	result.entries[2] = -entries[2];
	result.entries[3] = -entries[3];
	return result;
	
}

float Quat::norm(){
	float result = 0;
	result +=entries[0]*entries[0];
	result +=entries[1]*entries[1];
	result +=entries[2]*entries[2];
	result +=entries[3]*entries[3];
	return sqrt(result);
}
Quat Quat::normalize(){
	Quat result;
	float length = norm();
	result.entries[0] = entries[0]/length;
	result.entries[1] = entries[1]/length;
	result.entries[2] = entries[2]/length;
	result.entries[3] = entries[3]/length;
	return result;
}

Matrix Quat::toMatrix4(){
	float w = entries[0];
	float x = entries[1];
	float y = entries[2];
	float z = entries[3];
	Matrix result(4,4);
	result.setEntry(0,0,1-2*y*y - 2*z*z); 	result.setEntry(0,1,2*x*y-2*z*w); 		result.setEntry(0,2,2*x*z + 2*y*w);	result.setEntry(0,3,0.0);
	result.setEntry(1,0,2*x*y + 2*z*w); 	result.setEntry(1,1,1 - 2*x*x - 2*z*z); 	result.setEntry(1,2,2*y*z - 2*x*w);	result.setEntry(1,3,0);
	result.setEntry(2,0,2*x*z - 2*y*w); 	result.setEntry(2,1,2*y*z + 2*x*w); 	result.setEntry(2,2,1 - 2*x*x - 2*y*y);	result.setEntry(2,3,0);
	result.setEntry(3,0,0.0);				result.setEntry(3,1,0.0);				result.setEntry(3,2,0.0);			result.setEntry(3,3,1.0);
	return result;
}

Matrix Quat::toMatrix3(){
	float w = entries[0];
	float x = entries[1];
	float y = entries[2];
	float z = entries[3];
	Matrix result(3,3);
	result.setEntry(0,0,1-2*y*y - 2*z*z); 	result.setEntry(0,1,2*x*y-2*z*w); 		result.setEntry(0,2,2*x*z + 2*y*w);
	result.setEntry(1,0,2*x*y + 2*z*w); 	result.setEntry(1,1,1 - 2*x*x - 2*z*z); 	result.setEntry(1,2,2*y*z - 2*x*w);
	result.setEntry(2,0,2*x*z - 2*y*w); 	result.setEntry(2,1,2*y*z + 2*x*w); 	result.setEntry(2,2,1 - 2*x*x - 2*y*y);
	return result;
}
void Quat::print() const{
	for(int row=0;row<4;row++){
		printf("%.2f ",entries[row]);
	}
}
