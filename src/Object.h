#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h> //sudo apt-get install libglew-dev and compile-flag -I/usr/include/GL
//#include "glew.h"
#include <string>
#include <vector>
#include <string.h>
#include <algorithm>
#ifndef Object_h
#define Object_h
using namespace std;
typedef enum obj_type {
								EMPTY,PLANE,CUBE
}obj_type;
class Object {
private:
GLfloat* vertices;
GLfloat* colors;
GLfloat* normals;
GLuint* faces;
int no_faces;
int no_vertices;
public:
Object(const char* path);
Object(obj_type ot);
Object(const Object &copy);
~Object();
int getNoFaces();
int getNoVertices();
GLfloat* getVertices();
GLfloat* getColors();
GLfloat* getNormals();
GLuint* getFaces();
};
#endif
