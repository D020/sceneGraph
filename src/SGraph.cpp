#include "SGraph.h"

SGraph::SGraph(Object* in_object, string in_name) : position(4, 4), rotation(), scalation(4, 4), transform(4, 4), finaltransform(4, 4), wireframe_color(0, 1, 0) {
    parent = NULL;
    position = TMatrix(0, 0, 0);
    scalation = SMatrix(1.0, 1.0, 1.0);
    name = in_name;
    object = in_object;
    transform = position * rotation.toMatrix4();
    finaltransform = transform;
    render_wireframe = false;
    render_normals = false;
    render_filled = true;
    render_click = false;
    render_vertices = false;
    render_editClick = false;
    selected_verts = -1;
}

SGraph::SGraph(obj_type ot, string in_name) : SGraph(new Object(ot), in_name) {
}

SGraph::SGraph(const char* path, string in_name) : SGraph(new Object(path), in_name) {
}

SGraph::SGraph() {
}

void SGraph::setRenderFilled(bool in) {
    render_filled = in;
}

void SGraph::setRenderWireframe(bool in) {
    render_wireframe = in;
}

void SGraph::setRenderNormals(bool in) {
    render_normals = in;
}

void SGraph::setRenderClick(bool in) {
    render_click = in;
}

void SGraph::setRenderEditClick(bool in) {
    render_editClick = in;
}

void SGraph::setRenderVertices(bool in) {
    render_vertices = in;
}

bool SGraph::getRenderFilled() {
    return render_filled;
}

bool SGraph::getRenderWireframe() {
    return render_wireframe;
}

bool SGraph::getRenderNormals() {
    return render_normals;
}

bool SGraph::getRenderClick() {
    return render_click;
}

bool SGraph::getRenderEditClick() {
    return render_editClick;
}

bool SGraph::getRenderVertices() {
    return render_vertices;
}

void SGraph::addChild(SGraph* in_child) {
    in_child->parent = this;
    children.push_back(in_child);
}

void SGraph::scale(float x, float y, float z) {
    scalation = scalation * SMatrix(x, y, z);
    updateLocal();
}

void SGraph::setScale(float x, float y, float z) {
    scalation = SMatrix(x, y, z);
    updateLocal();
}

void SGraph::translate(float x, float y, float z, int relative) {
    if (relative == REL_WORLD) {
        Quat p_rot;
        if (parent != NULL)
            p_rot = parent->getRotation()[0];
        position = p_rot.conjugate().toMatrix4() * TMatrix(x, y, z) * p_rot.toMatrix4() * position;
    }
    if (relative == REL_LOCAL)
        position = rotation.toMatrix4() * TMatrix(x, y, z) * rotation.conjugate().toMatrix4() * position;
    if (relative == REL_PARENT)
        position = TMatrix(x, y, z) * position;
    updateAll();
}

void SGraph::translateAlong(Matrix axis, float x, float y, float z, int relative) {
    axis = axis.normalized();
    Quat axisRotation = Quat(Matrix(0, 0, 1), axis.x()) * Quat(Matrix(0, 1, 0), axis.y()) * Quat(Matrix(1, 0, 0), axis.z());
    if (relative == REL_WORLD) {
        Matrix converted = axisRotation.toMatrix4() * TMatrix(x, y, z) * axisRotation.conjugate().toMatrix4() * Matrix(1, 0, 0);
        translate(converted.x(), converted.y(), converted.z(), REL_WORLD);
    }
}

void SGraph::setTranslation(float x, float y, float z, int relative) {
    if (relative == REL_WORLD) {
        Quat p_rot;
        Matrix p_tran = TMatrix(0, 0, 0);
        if (parent != NULL) {
            p_tran = parent->getTranslation()[0];
            p_rot = parent->getRotation()[0];
        }
        position = p_rot.conjugate().toMatrix4() * TMatrix(x, y, z) * p_tran.trans_invert() * p_rot.toMatrix4();
    }
    if (relative == REL_LOCAL || relative == REL_PARENT)
        position = TMatrix(x, y, z);
    updateAll();
}

void SGraph::setTranslation(Matrix intrans) {
    position = intrans;
    updateAll();
}

void SGraph::rotate(float x, float y, float z, int relative) {
    if (relative == REL_WORLD) {
        Quat p_rot;
        if (parent != NULL)
            p_rot = parent->getRotation()[0];
        rotation = p_rot.conjugate() * Quat(Matrix(0, 0, 1), z) * Quat(Matrix(0, 1, 0), y) * Quat(Matrix(1, 0, 0), x) * p_rot*rotation;
    }
    if (relative == REL_PARENT)
        rotation = Quat(Matrix(0, 0, 1), z) * Quat(Matrix(0, 1, 0), y) * Quat(Matrix(1, 0, 0), x) * rotation;
    if (relative == REL_LOCAL) {
        /*
           Matrix rot = rotation.toMatrix3();
           Matrix localx = rot*Matrix(1,0,0);
           Matrix localy = rot*Matrix(0,1,0);
           Matrix localz = rot*Matrix(0,0,1);
           rotation = Quat(localz,z)*Quat(localy,y)*Quat(localx,x)*rotation;
         */
        rotation = rotation * Quat(Matrix(0, 0, 1), z) * Quat(Matrix(0, 1, 0), y) * Quat(Matrix(1, 0, 0), x) * rotation.conjugate() * rotation;
    }
    updateAll();
}

void SGraph::setRotation(float x, float y, float z, int relative) {
    if (relative == REL_WORLD) {
        Quat p_rot;
        if (parent != NULL)
            p_rot = parent->getRotation()[0];
        rotation = p_rot.conjugate() * Quat(Matrix(0, 0, 1), z) * Quat(Matrix(0, 1, 0), y) * Quat(Matrix(1, 0, 0), x);
    }
    if (relative == REL_PARENT || relative == REL_LOCAL)
        rotation = Quat(Matrix(0, 0, 1), z) * Quat(Matrix(0, 1, 0), y) * Quat(Matrix(1, 0, 0), x);
    updateAll();
}

void SGraph::rotateTransAround(Matrix org, Matrix axis, float deg) {
    if (org.isSize(3, 1) && axis.isSize(3, 1)) {
        org = org.resize(4, 1);
        Matrix pos = finaltransform * Matrix(0, 0, 0, 1);
        Quat rot = Quat(axis, deg);
        Matrix dir = pos - org;
        dir = rot.toMatrix4() * dir;
        Matrix np = org + dir;
        setTranslation(np.x(), np.y(), np.z(), REL_WORLD);
        //setRotation(+
    } else
        printf("Origin or axis is not a Matrix(3,1)\n");

}

void SGraph::rotateAround(Matrix org, Matrix axis, float deg) {
    if (org.isSize(3, 1) && axis.isSize(3, 1)) {
        org = org.resize(4, 1);
        Matrix pos = finaltransform * Matrix(0, 0, 0, 1);
        Quat rot = Quat(axis, deg);
        Matrix dir = pos - org;
        dir = rot.toMatrix4() * dir;
        Matrix np = org + dir;
        setTranslation(np.x(), np.y(), np.z(), REL_WORLD);


        Quat p_rot;
        if (parent != NULL)
            p_rot = parent->getRotation()[0];
        rotation = p_rot.conjugate() * rot * p_rot*rotation;



    } else
        printf("Origin or axis is not a Matrix(3,1)\n");

}

Matrix* SGraph::getTransform() {
    return &finaltransform;
}

Quat* SGraph::getRotation() {
    return &rotation;
}

Matrix SGraph::getPosition() {
    return finaltransform * Matrix(0, 0, 0, 1);
}

Matrix* SGraph::getTranslation() {
    return &position;
}

Matrix* SGraph::getScale() {
    return &scalation;
}

Object* SGraph::getObject() {
    return object;
}

vector<SGraph*>* SGraph::getChildren() {
    return &children;
}

void SGraph::updateLocal() {
    rotation = rotation.normalize();
    if (parent == NULL)
        finaltransform = position * rotation.toMatrix4();
    else
        finaltransform = parent->getTransform()[0] * position * rotation.toMatrix4();
}

void SGraph::updateAll() {
    updateLocal();
    for (int i = 0; i < children.size(); i++)
        children[i]->updateAll();
}

int SGraph::renderClick(int availableID) {
    Object *obj = getObject();

    glNormalPointer(GL_FLOAT, 0, obj->getNormals());
    glColorPointer(3, GL_FLOAT, 0, obj->getColors());
    glVertexPointer(3, GL_FLOAT, 0, obj->getVertices());

    vector<SGraph *> *children = getChildren();
    for (int i = 0; i < children->size(); i++)
        availableID = availableID + children[0][i]->renderClick(availableID);

    glPushMatrix();
    glPushAttrib(GL_LIGHTING_BIT);

    glMatrixMode(GL_MODELVIEW);
    glMultMatrixf((getTransform()[0] * getScale()[0]).getEntries());

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glDisable(GL_LIGHTING);
    glShadeModel(GL_FLAT);

    int i = availableID;
    unsigned int lowest_byte = i & 0xff;
    unsigned int second_byte = (i >> 8) & 0xff;
    unsigned int third_byte = (i >> 16) & 0xff;
    glColor3f(1 - float(lowest_byte) / 255, float(second_byte) / 255, float(third_byte) / 255);

    glDrawElements(GL_TRIANGLES, obj->getNoFaces() * 3, GL_UNSIGNED_INT, obj->getFaces());

    glPopAttrib();
    glPopMatrix();
    return availableID + 1;
}

void SGraph::render() {

    Object *obj = getObject();

    glNormalPointer(GL_FLOAT, 0, obj->getNormals());
    glColorPointer(3, GL_FLOAT, 0, obj->getColors());
    glVertexPointer(3, GL_FLOAT, 0, obj->getVertices());

    if (render_editClick) {
        GLfloat* verts = obj->getVertices();
        glPushMatrix();
        glPushAttrib(GL_LIGHTING_BIT);

        glMatrixMode(GL_MODELVIEW);
        glMultMatrixf((getTransform()[0] * getScale()[0]).getEntries());

        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);

        glDisable(GL_LIGHTING);
        glShadeModel(GL_FLAT);

        glDisable(GL_LIGHTING);
        glPointSize(10);
        glBegin(GL_POINTS);
        int u;
        u = 0;
        for (int i = 0; i < obj->getNoVertices()*3; i += 3) {
            u++;
            unsigned int lowest_byte = u & 0xff;
            unsigned int second_byte = (u >> 8) & 0xff;
            unsigned int third_byte = (u >> 16) & 0xff;
            glColor3f(float(lowest_byte) / 255, float(second_byte) / 255, float(third_byte) / 255);
            glVertex3f(verts[i], verts[i + 1], verts[i + 2]);
        }
        glEnd();

        glPopAttrib();
        glPopMatrix();
    } else {
        glPushMatrix();
        glPushAttrib(GL_LIGHTING_BIT);
        glMatrixMode(GL_MODELVIEW);
        glMultMatrixf((getTransform()[0] * getScale()[0]).getEntries());


        glShadeModel(GL_SMOOTH);

        glEnable(GL_LIGHTING);
        //glEnable(GL_NORMALIZE);
        glEnable(GL_RESCALE_NORMAL);
        if (render_filled) {
            GLfloat waterAmbient[] = {0.2, 0.2, 0.2, 1.0};
            GLfloat waterColor[] = {0.8, 0.8, 0.8, 1.0};
            GLfloat waterShiny[] = {0.1, 0.0, 0.0, 1.0};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, waterColor);
            glMaterialfv(GL_FRONT, GL_AMBIENT, waterAmbient);
            glMaterialfv(GL_FRONT, GL_SPECULAR, waterShiny);
            glMaterialf(GL_FRONT, GL_SHININESS, 5.0);


            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            glEnableClientState(GL_VERTEX_ARRAY);
            glNormalPointer(GL_FLOAT, 0, obj->getNormals());
            glColorPointer(3, GL_FLOAT, 0, obj->getColors());
            glVertexPointer(3, GL_FLOAT, 0, obj->getVertices());
            glDrawElements(GL_TRIANGLES, obj->getNoFaces() * 3, GL_UNSIGNED_INT, obj->getFaces());
        }
        GLuint* faces = obj->getFaces();
        GLfloat* verts = obj->getVertices();
        GLfloat* norms = obj->getNormals();
        //Wireframe
        if (render_wireframe) {
            glDisable(GL_LIGHTING);
            glColor3f(wireframe_color.x(), wireframe_color.y(), wireframe_color.z());
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i < obj->getNoFaces()*3; i += 3) {
                GLuint p1i = faces[i]*3;
                GLuint p2i = faces[i + 1]*3;
                GLuint p3i = faces[i + 2]*3;
                glVertex3f(verts[p1i], verts[p1i + 1], verts[p1i + 2]);
                glVertex3f(verts[p2i], verts[p2i + 1], verts[p2i + 2]);
                glVertex3f(verts[p3i], verts[p3i + 1], verts[p3i + 2]);
                glVertex3f(verts[p1i], verts[p1i + 1], verts[p1i + 2]);
            }
            glEnd();
        }
        if (render_vertices) {
            glDisable(GL_LIGHTING);
            //glColor3f(1, 0.2, 1);
            glPointSize(6);
            glBegin(GL_POINTS);
            int u = 0;
            for (int i = 0; i < obj->getNoVertices()*3; i += 3) {
                if (u == selected_verts)
                    glColor3f(1, 1, 0);
                else
                    glColor3f(1, 0.2, 1);
                glVertex3f(verts[i], verts[i + 1], verts[i + 2]);
                u++;
            }
            glEnd();
        }

        //Normals
        if (render_normals) {
            glColor3f(0, 0, 1);
            glBegin(GL_LINES);
            /*
               for(int i=0; i<obj->getNoVertices()*3; i+=3) {
                glVertex3f(verts[i],verts[i+1],verts[i+2]);
                glVertex3f(verts[i]+norms[i]/5,verts[i+1]+norms[i+1]/5,verts[i+2]+norms[i+2]/5);

               }
             */
            for (int i = 0; i < obj->getNoFaces()*3; i += 3) {
                GLuint p1i = faces[i]*3;
                GLuint p2i = faces[i + 1]*3;
                GLuint p3i = faces[i + 2]*3;
                GLfloat x = (verts[p1i] + verts[p2i] + verts[p3i]) / 3;
                GLfloat y = (verts[p1i + 1] + verts[p2i + 1] + verts[p3i + 1]) / 3;
                GLfloat z = (verts[p1i + 2] + verts[p2i + 2] + verts[p3i + 2]) / 3;
                GLfloat nx = (norms[p1i] + norms[p2i] + norms[p3i]) / 3;
                GLfloat ny = (norms[p1i + 1] + norms[p2i + 1] + norms[p3i + 1]) / 3;
                GLfloat nz = (norms[p1i + 2] + norms[p2i + 2] + norms[p3i + 2]) / 3;
                glVertex3f(x, y, z);
                glVertex3f(x + nx / 2, y + ny / 2, z + nz / 2);

            }

            glEnd();
        }

        vector<SGraph *> *children = getChildren();
        glPopAttrib();
        glPopMatrix();
        for (int i = 0; i < children->size(); i++)
            children[0][i]->render();
    }
}

void SGraph::print() {
    printf("%s\n", name.c_str());
    for (int i = 0; i < children.size(); i++) {
        printf("--");
        children[i]->print();
    }
}
