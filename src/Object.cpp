#include "Object.h"
#include "Matrix.h"

struct facepair {
    int vertex;
    int normal;

    facepair(int v, int n) {
        vertex = v;
        normal = n;
    }
};

struct vertex {
    float x;
    float y;
    float z;

    vertex(float xx, float yy, float zz) {
        x = xx;
        y = yy;
        z = zz;
    }
};

bool fp_sort(facepair i, facepair j) {
    if (i.vertex == j.vertex)
        return (i.normal < j.normal);
    else
        return (i.vertex < j.vertex);
}

typedef struct vs {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} vs;

typedef struct fc {
    GLuint v1;
    GLuint v2;
    GLuint v3;
} fc;

void setvs(vs* v, GLfloat x, GLfloat y, GLfloat z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

Object::~Object() {
    free(vertices);
    free(normals);
    free(colors);
    free(faces);
}

Object::Object(const Object &copy) {
    vertices = (GLfloat*) malloc(copy.no_vertices * 3 * sizeof (GLfloat));
    colors = (GLfloat*) malloc(copy.no_vertices * 3 * sizeof (GLfloat));
    normals = (GLfloat*) malloc(copy.no_vertices * 3 * sizeof (GLfloat));
    faces = (GLuint*) malloc(copy.no_faces * 3 * sizeof (GLuint));
    memcpy(vertices, copy.vertices, copy.no_vertices * 3 * sizeof (GLfloat));
    memcpy(colors, copy.colors, copy.no_vertices * 3 * sizeof (GLfloat));
    memcpy(normals, copy.normals, copy.no_vertices * 3 * sizeof (GLfloat));
    memcpy(faces, copy.faces, copy.no_vertices * 3 * sizeof (GLuint));

    no_faces = copy.no_faces;
    no_vertices = copy.no_vertices;

}

Object::Object(obj_type ot) {
    vertices = NULL;
    normals = NULL;
    faces = NULL;
    colors = NULL;
    no_faces = 0;
    no_vertices = 0;
    if (ot == PLANE) {
        no_vertices = 6;
        vertices = (GLfloat*) calloc(no_vertices * 3, sizeof (GLfloat));
        no_faces = 2;
        faces = (GLuint*) calloc(no_faces * 3, sizeof (GLuint));
        normals = (GLfloat*) calloc(no_vertices * 3, sizeof (GLfloat));
        colors = (GLfloat*) calloc(no_vertices * 3, sizeof (GLfloat));

        vs* vts = (vs*) vertices;
        fc* fcs = (fc*) faces;
        vs* nms = (vs*) normals;
        vs* cls = (vs*) colors;

        setvs(&vts[0], 1, 0, 1);
        setvs(&vts[1], 1, 0, -1);
        setvs(&vts[2], -1, 0, -1);
        setvs(&vts[3], -1, 0, -1);
        setvs(&vts[4], -1, 0, 1);
        setvs(&vts[5], 1, 0, 1);

        fcs[0].v1 = 0;
        fcs[0].v2 = 1;
        fcs[0].v3 = 2;
        fcs[1].v1 = 3;
        fcs[1].v2 = 4;
        fcs[1].v3 = 5;
        setvs(&nms[0], 0, 1, 0);
        setvs(&nms[1], 0, 1, 0);
        setvs(&nms[2], 0, 1, 0);
        setvs(&nms[3], 0, 1, 0);
        setvs(&nms[4], 0, 1, 0);
        setvs(&nms[5], 0, 1, 0);

        memset(colors, 12, sizeof (GLfloat) * no_vertices * 3);

    }
    printf("Verts - Normals\n");
    for (int i = 0; i < no_vertices; i++)
        printf("%f\t%f\n%f\t%f\n%f\t%f\n\n", vertices[i * 3 + 0], normals[i * 3 + 0], vertices[i * 3 + 1], normals[i * 3 + 1], vertices[i * 3 + 2], normals[i * 3 + 2]);
}

Object::Object(const char* path) {
    vector<string> lines;
    vector<string> vertlines;
    vector<string> facelines;
    vector<string> normlines;
    vector<facepair> facepairs;
    vector<vertex> vertexs;
    vector<vertex> norms;
#ifdef linux
    FILE* fp = fopen(path, "r");
#endif
#ifdef windows
    string fixedfp(path);
    std::replace(fixedfp.begin(), fixedfp.end(), '/', '\\');
    FILE* fp = fopen(fixedfp.c_str(), "rb");
#endif
    printf("%s\n", path);
    if (fp == NULL)
        printf("File error\n");
    else {
        size_t lSize;
        char* buffer;
        size_t bytes_read;
        fseek(fp, 0, SEEK_END);
        lSize = ftell(fp);
        rewind(fp);
        printf("rewind\n");
        buffer = (char*) calloc(sizeof (char), lSize);
        if (buffer == NULL) {
            printf("Memory allocation error\n");
        } else {
            bytes_read = fread(buffer, 1, lSize, fp);
            printf("read\n");
            if (bytes_read != lSize)
                printf("Reading error\n");
            else {
                printf("enter strtok\n");
                char* pch;
                pch = strtok(buffer, "\n");
                while (pch != NULL) {
                    //printf("%s\n",pch);
                    if (pch[0] == 'v' && pch[1] == ' ')
                        vertlines.push_back(pch);
                    else if (pch[0] == 'v' && pch[1] == 'n')
                        normlines.push_back(pch);
                    else if (pch[0] == 'f')
                        facelines.push_back(pch);
                    pch = strtok(NULL, "\n");
                }
                printf("exit strtok\n");

            }
        }
        free(buffer);
    }
    for (int i = 0; i < vertlines.size(); i++) {
        if (i % (vertlines.size()/5) == 0)
            printf("v%i/%i\n", i, (int) vertlines.size());
        string line = vertlines[i];
        size_t number_1_begin = line.find_first_of("1234567890.-+e");
        size_t number_1_end = line.find_first_not_of("1234567890.-+e", number_1_begin + 1);

        size_t number_2_begin = line.find_first_of("1234567890.-+e", number_1_end + 1);
        size_t number_2_end = line.find_first_not_of("1234567890.-+e", number_2_begin + 1);

        size_t number_3_begin = line.find_first_of("1234567890.-+e", number_2_end + 1);
        size_t number_3_end = line.find_first_not_of("1234567890.-+e", number_3_begin + 1);

        string number_1 = line.substr(number_1_begin, number_1_end - number_1_begin);
        string number_2 = line.substr(number_2_begin, number_2_end - number_2_begin);
        string number_3 = line.substr(number_3_begin, number_3_end - number_3_begin);

        vertexs.push_back(vertex(atof(number_1.c_str()), atof(number_2.c_str()), atof(number_3.c_str())));

        //printf("%s\tEXTRACT\t '%f'\t '%f' \t '%f' \n",line.c_str(),vertexs.back().x,vertexs.back().y,vertexs.back().z);

    }
    for (int i = 0; i < normlines.size(); i++) {
        if (i % (normlines.size()/5) == 0)
            printf("n%i/%i\n", i, (int) normlines.size());
        string line = normlines[i];
        size_t number_1_begin = line.find_first_of("1234567890.-+e");
        size_t number_1_end = line.find_first_not_of("1234567890.-+e", number_1_begin + 1);

        size_t number_2_begin = line.find_first_of("1234567890.-+e", number_1_end + 1);
        size_t number_2_end = line.find_first_not_of("1234567890.-+e", number_2_begin + 1);

        size_t number_3_begin = line.find_first_of("1234567890.-+e", number_2_end + 1);
        size_t number_3_end = line.find_first_not_of("1234567890.-+e", number_3_begin + 1);

        string number_1 = line.substr(number_1_begin, number_1_end - number_1_begin);
        string number_2 = line.substr(number_2_begin, number_2_end - number_2_begin);
        string number_3 = line.substr(number_3_begin, number_3_end - number_3_begin);
        Matrix pnorm = Matrix(atof(number_1.c_str()), atof(number_2.c_str()), atof(number_3.c_str()));
        pnorm = pnorm.normalized();
        norms.push_back(vertex(pnorm.x(), pnorm.y(), pnorm.z()));
    }
    for (int i = 0; i < facelines.size(); i++) {
        if (i % (facelines.size()/5) == 0)
            printf("f%i/%i\n", i, (int) facelines.size());
        string line = facelines[i];
        //_f 15/13/15 17/14/17 20/15/20
        size_t number_1_begin = line.find_first_of("1234567890.-+");
        //f _15/13/15 17/14/17 20/15/20
        size_t number_1_end = line.find_first_of("/", number_1_begin + 1);
        //f 15_/13/15 17/14/17 20/15/20
        size_t number_1_snd = line.find_first_of("/", number_1_end + 1);
        //f 15/13_/15 17/14/17 20/15/20

        size_t number_2_begin = line.find_first_of("1234567890.-+", number_1_snd + 1);
        //f 15/13/_15 17/14/17 20/15/20
        size_t number_2_end = line.find_first_not_of("1234567890.-+", number_2_begin + 1);
        //f 15/13/15_ 17/14/17 20/15/20

        size_t number_3_begin = line.find_first_of("1234567890.-+", number_2_end + 1);
        size_t number_3_end = line.find_first_of("/", number_3_begin + 1);
        size_t number_3_snd = line.find_first_of("/", number_3_end + 1);

        size_t number_4_begin = line.find_first_of("1234567890.-+", number_3_snd + 1);
        size_t number_4_end = line.find_first_not_of("1234567890.-+", number_4_begin + 1);

        size_t number_5_begin = line.find_first_of("1234567890.-+", number_4_end + 1);
        size_t number_5_end = line.find_first_of("/", number_5_begin + 1);
        size_t number_5_snd = line.find_first_of("/", number_5_end + 1);

        size_t number_6_begin = line.find_first_of("1234567890.-+", number_5_snd + 1);
        size_t number_6_end = line.find_first_not_of("1234567890.-+", number_6_begin + 1);

        string number_1 = line.substr(number_1_begin, number_1_end - number_1_begin);
        string number_2 = line.substr(number_2_begin, number_2_end - number_2_begin);
        string number_3 = line.substr(number_3_begin, number_3_end - number_3_begin);
        string number_4 = line.substr(number_4_begin, number_4_end - number_4_begin);
        string number_5 = line.substr(number_5_begin, number_5_end - number_5_begin);
        string number_6 = line.substr(number_6_begin, number_6_end - number_6_begin);

        //printf("%s\tEXTRACT\t '%s'\t '%s' \t '%s' \t '%s'\t '%s' \t '%s' \n",line.c_str(),number_1.c_str(),number_2.c_str(),number_3.c_str(),number_4.c_str(),number_5.c_str(),number_6.c_str());

        facepairs.push_back(facepair(atoi(number_1.c_str()), atoi(number_2.c_str())));
        facepairs.push_back(facepair(atoi(number_3.c_str()), atoi(number_4.c_str())));
        facepairs.push_back(facepair(atoi(number_5.c_str()), atoi(number_6.c_str())));

        //printf("%s\tEXTRACT\t %i,%i \t %i,%i \t %i,%i \t\n",line.c_str(),facepairs[i*3].vertex,facepairs[i*3].normal,facepairs[i*3+1].vertex,facepairs[i*3+1].normal,facepairs[i*3+2].vertex,facepairs[i*3+2].normal);

    }

    vector<facepair> face_tmp;
    face_tmp = facepairs;

    std::sort(facepairs.begin(), facepairs.end(), fp_sort);
    printf("sorted\n");
    vector<facepair> face_set;
    while (!facepairs.empty()) {
        if (face_set.size() == 0) {
            face_set.push_back(facepairs.back());
            facepairs.pop_back();
        } else {
            if (face_set.back().vertex == facepairs.back().vertex && face_set.back().normal == facepairs.back().normal)
                facepairs.pop_back();
            else {
                face_set.push_back(facepairs.back());
                facepairs.pop_back();
            }
        }
    }
    printf("facepaired\n");
    std::sort(face_set.begin(), face_set.end(), fp_sort);
    facepairs = face_tmp;

    vertices = (GLfloat*) calloc(face_set.size()*3, sizeof (GLfloat));
    colors = (GLfloat*) calloc(face_set.size()*3, sizeof (GLfloat));
    normals = (GLfloat*) calloc(face_set.size()*3, sizeof (GLfloat));
    faces = (GLuint*) calloc(facelines.size()*3, sizeof (GLuint));
    no_vertices = 0;
    for (int i = 0; i < face_set.size(); i++) {
        //printf("%i,%i \t %i \n",face_set[i].vertex,face_set[i].normal,i);
        vertices[i * 3 + 0] = (GLfloat) vertexs[face_set[i].vertex - 1].x;
        vertices[i * 3 + 1] = (GLfloat) vertexs[face_set[i].vertex - 1].y;
        vertices[i * 3 + 2] = (GLfloat) vertexs[face_set[i].vertex - 1].z;
        no_vertices++;
        normals[i * 3 + 0] = (GLfloat) norms[face_set[i].normal - 1].x;
        normals[i * 3 + 1] = (GLfloat) norms[face_set[i].normal - 1].y;
        normals[i * 3 + 2] = (GLfloat) norms[face_set[i].normal - 1].z;
        colors[i * 3 + 0] = 0.7; //((float)(rand()%101))/100;
        colors[i * 3 + 1] = 0.5; //((float)(rand()%101))/100;
        colors[i * 3 + 2] = 0.5; //((float)(rand()%101))/100;
    }
    printf("finding fp\n");
    for (int i = 0; i < facepairs.size(); i++) {
        //printf("Finding '%i,%i' in face_set\n",facepairs[i].vertex,facepairs[i].normal);
        if (i % (facepairs.size()/5) == 0)
            printf("fp%i/%i\n", i, (int) facepairs.size());
        //Linear search
        /*
                        int it=0;
                           while((face_set[it].vertex != facepairs[i].vertex || face_set[it].normal != facepairs[i].normal)&& it<face_set.size()) {
                                it++;
                           }*/
        int a = 0;
        int it = face_set.size() / 2;
        int b = face_set.size();
        while ((face_set[it].vertex != facepairs[i].vertex || face_set[it].normal != facepairs[i].normal) && it < face_set.size()) {
            if (b - a < 20) {
                break;
            }
            if (fp_sort(face_set[it], facepairs[i])) {
                a = it;
                it = a + (b - a) / 2;
            } else {
                b = it;
                it = a + (b - a) / 2;
            }
            //printf("a: %i, b:%i\n",a,b);
        }
        it = a;
        while ((face_set[it].vertex != facepairs[i].vertex || face_set[it].normal != facepairs[i].normal) && it < face_set.size()) {
            it++;
        }

        //printf("Found at %i\n",it);
        faces[i] = (GLuint) it;

    }

    printf("done\n");

    /*
       printf("Verts - Normals\n");
       for(int i=0; i<face_set.size();i++)
       printf("%f\t%f\n%f\t%f\n%f\t%f\n\n",vertices[i*3+0],normals[i*3+0],vertices[i*3+1],normals[i*3+1],vertices[i*3+2],normals[i*3+2]);
       for(int i=0;i<facepairs.size()/3;i++){
       printf("%i %i %i\n",faces[i*3+0], faces[i*3+1],faces[i*3+2]);
       }
     */
    no_faces = facelines.size();

}

int Object::getNoFaces() {
    return no_faces;
}

int Object::getNoVertices() {
    return no_vertices;
}

GLfloat* Object::getVertices() {
    return vertices;
}

GLfloat* Object::getColors() {
    return colors;
}

GLfloat* Object::getNormals() {
    return normals;
}

GLuint* Object::getFaces() {
    return faces;
}
