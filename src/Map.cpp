#include "Map.h"
#include <math.h>

Map::Map() : origin(SGraph(EMPTY, "Default map")), pushTransform(Matrix(4, 4)), movement(TMatrix(0, 0, 0)), pushObject(Object(EMPTY)) {
    selected = -1;
    editSelected = -1;
    moving = false;
    editing = false;
    movingAxis = NONE;
    inputFloat.clear();
}

void Map::addChild(SGraph* in_child) {
    origin.addChild(in_child);
    all_children.push_back(in_child);
}

void Map::render() {
    origin.render();
}

void Map::renderClick() {
    if (editing) {
        SGraph child = *all_children[selected];
        child.setRenderEditClick(true);
        child.render();
        child.setRenderEditClick(false);
    } else {
        for (unsigned int i = 0; i < all_children.size(); i++) {
            SGraph child = *all_children[i];
            child.setRenderClick(true);
            bool fill = child.getRenderFilled();
            bool wire = child.getRenderWireframe();
            bool norm = child.getRenderNormals();
            child.setRenderFilled(false);
            child.setRenderWireframe(false);
            child.setRenderNormals(false);
            i++;
            unsigned int lowest_byte = i & 0xff;
            unsigned int second_byte = (i >> 8) & 0xff;
            unsigned int third_byte = (i >> 16) & 0xff;
            i--;
            glColor3f(float(lowest_byte) / 255, float(second_byte) / 255, float(third_byte) / 255);
            child.render();
            child.setRenderFilled(fill);
            child.setRenderWireframe(wire);
            child.setRenderNormals(norm);
            child.setRenderClick(false);
        }
    }
    //origin.render();
}

void Map::passChar(char c) {
    if (c == 'B') {
        if (inputFloat.length() != 0)
            inputFloat.resize(inputFloat.length() - 1);
    } else {
        inputFloat.resize(inputFloat.length() + 1, c);
    }
    printf("%s\n", inputFloat.c_str());
    movy = atof(inputFloat.c_str());
    movx = 0;
    passMotion(0, 0);
}

void Map::toggleMove(bool quit) {
    moving = !moving;
    if (moving) {
        movx = 0;
        movy = 0;
        inputFloat.clear();
        movingAxis = NONE;
    }
    if (0 <= selected) {
        if (moving) {
            pushTransform = *all_children[selected]->getTransform();

        } else if (quit) {
            all_children[selected]->setTranslation(pushTransform);
            printf("quitted\n");
        } else {
            printf("applied\n");
        }
    }
}

void Map::toggleEdit(bool quit) {
    editing = !editing;
    if (editing) {

    }
    if (0 <= selected) {
        if (editing) {
            pushObject = *all_children[selected]->getObject();
            all_children[selected]->setRenderVertices(true);
            all_children[selected]->setRenderFilled(false);
        } else if (quit) {
            *all_children[selected]->getObject() = pushObject;
            printf("quitted\n");
            all_children[selected]->setRenderVertices(false);
            all_children[selected]->setRenderFilled(true);
        } else {
            printf("applied\n");
            all_children[selected]->setRenderVertices(false);
            all_children[selected]->setRenderFilled(true);
        }
    }
}

void Map::setSnap(bool snappy) {
    snap = snappy;
}

void Map::setMovingAxis(axis_direction ax) {
    movingAxis = ax;
}

void Map::passMotion(float x, float y) {
    float pMovx;
    float pMovy;
    float movLength;
    if (moving) {
        movx += x;
        movy += y;
        if (0 <= selected) {
            if (snap) {
                pMovx = movx - fmod(movx, 0.1);
                pMovy = movy - fmod(movy, 0.1);
                movLength = pMovy;
            } else {
                pMovx = movx;
                pMovy = movy;
                movLength = pMovy;
            }
            if (movingAxis == NONE) {
                Quat axisRotation = *frozenCamera.getRotation();
                Matrix converted = axisRotation.toMatrix4() * TMatrix(pMovx, -pMovy, 0) * axisRotation.conjugate().toMatrix4();
                all_children[selected]->setTranslation(pushTransform * converted);
            } else if (movingAxis == X_AXIS)
                all_children[selected]->setTranslation(pushTransform * TMatrix(movLength, 0, 0));
            else if (movingAxis == Y_AXIS)
                all_children[selected]->setTranslation(pushTransform * TMatrix(0, movLength, 0));
            else if (movingAxis == Z_AXIS)
                all_children[selected]->setTranslation(pushTransform * TMatrix(0, 0, movLength));
        }
    }
}

void Map::passCamera(SGraph cam) {
    frozenCamera = cam;
}

void Map::select(int x, int y) {
    glFlush();
    glFinish();
    renderClick();
    glFlush();
    glFinish();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned char data[4];
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    if (selected < all_children.size())
        all_children[selected]->setRenderWireframe(false);
    if (editing) {
        editSelected = (unsigned int) data[0] + (((unsigned int) data[1]) << 8) + (((unsigned int) data[2]) << 16) - 1;
        all_children[selected]->selected_verts = editSelected;
        Object* obj = all_children[selected]->getObject();
        GLfloat* verts = obj->getVertices();
        verts[editSelected * 3] = verts[editSelected * 3] + 1;
    } else {
        selected = (unsigned int) data[0] + (((unsigned int) data[1]) << 8) + (((unsigned int) data[2]) << 16) - 1;
    }
    printf("Selected %i %i\n", selected, editSelected);
    if (selected < all_children.size())
        all_children[selected]->setRenderWireframe(true);
    glFlush();
    glFinish();
}
