#ifndef Map_h
#define Map_h
#include "SGraph.h"
typedef enum axis_direction {
        NONE,X_AXIS,Y_AXIS,Z_AXIS
} axis_direction;
class Map {
private:
int selected;
int editSelected;

vector<SGraph*> all_children;
string inputFloat;
bool moving;
bool editing;
bool snap;
float movx;
float movy;
axis_direction movingAxis;
Matrix pushTransform;
Object pushObject;
Matrix movement;
SGraph frozenCamera;
public:
    SGraph origin;
Map();
void addChild(SGraph* in_child);
void select(int x, int y);
void toggleMove(bool quit);
void toggleEdit(bool quit);
void setSnap(bool snappy);
void setMovingAxis(axis_direction ax);
void passChar(char c);
void passMotion(float x, float y);
void passCamera(SGraph cam);
void render();
void renderClick();
};
#endif
