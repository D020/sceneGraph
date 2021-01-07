#include <stdio.h>
#include <stdlib.h>
#include "Object.h"
#include "Matrix.h"
#include "Quat.h"
#include <vector>
#ifndef SGraph_h
#define SGraph_h
#define REL_WORLD 0
#define REL_PARENT 1
#define REL_LOCAL 2

class SGraph {
private:
    int selected;
    string name;
    SGraph* parent;
    Matrix position;
    Quat rotation;
    Matrix scalation;
    Matrix transform;
    Matrix finaltransform;
    Matrix wireframe_color;
    bool render_wireframe;
    bool render_normals;
    bool render_filled;
    bool render_vertices;
    bool render_click;
    bool render_editClick;
    bool moving;

    Object* object;
    vector<SGraph*> children;
public:
    int selected_verts;
    SGraph();
    SGraph(Object* in_object, string in_name);
    SGraph(obj_type ot, string in_name);
    SGraph(const char* path, string in_name);
    void addChild(SGraph* in_child);
    void print();

    void scale(float x, float y, float z);
    void setScale(float x, float y, float z);

    void translate(float x, float y, float z, int relative);
    void translateAlong(Matrix axis, float x, float y, float z, int relative);
    void setTranslation(float x, float y, float z, int relative);
    void setTranslation(Matrix intrans);

    void rotate(float x, float y, float z, int relative); //Relative is of one defined in top.
    void setRotation(float x, float y, float z, int relative);
    void rotateTransAround(Matrix org, Matrix axis, float deg);
    void rotateAround(Matrix org, Matrix axis, float deg);

    void setRenderFilled(bool in);
    void setRenderWireframe(bool in);
    void setRenderNormals(bool in);
    void setRenderClick(bool in);
    void setRenderEditClick(bool in);
    void setRenderVertices(bool in);
    bool getRenderFilled();
    bool getRenderWireframe();
    bool getRenderNormals();
    bool getRenderClick();
    bool getRenderEditClick();
    bool getRenderVertices();
    void passMotion(float x, float y);

    Matrix* getTransform();
    Quat* getRotation();
    Matrix* getTranslation();
    Matrix getPosition();
    Matrix* getScale();
    Object* getObject();
    vector<SGraph*>* getChildren();

    void updateLocal();
    void updateAll();
    void render();
    int renderClick(int availableID);

    void select(int x, int y);
};
#endif
