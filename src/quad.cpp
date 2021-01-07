#include <stdio.h>
#include <stdlib.h>
#ifdef linux
#include <X11/X.h>
#include <X11/Xlib.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
//#include <GL/glx.h>
#include <GL/glu.h>
#include "Object.h"
#include "SGraph.h"
#include "Matrix.h"
#include "Quat.h"
#include "Map.h"
#include <string>
#include <vector>
#ifdef linux
#include "SDL.h"
#endif
#ifdef windows
#include <SDL/SDL.h>
#endif
Uint32 last_time;
Uint32 delta_time;
int width;
int height;
float zoom;
Map map;
SGraph kamera(EMPTY, string("Kameraet"));
Matrix krotation(0, 0, 0);
SGraph lookat(EMPTY, string("kig herhen noob"));
SGraph up(EMPTY, string("Kameraets up-vektor"));

typedef enum input_state_type {
    SELECT, ROTATING, ROTATING_AXIS, ROTATING_FLOAT, MOVING, MOVING_AXIS, MOVING_FLOAT, EDIT, DUPE
} input_state_type;

input_state_type inputState;

static void quit_tutorial(int code) {
    SDL_Quit();
    exit(code);
}

static void handle_key(SDL_keysym *keysym, bool up) {
    if (up) {
        switch (keysym->sym) {
            case SDLK_g:
                switch (inputState) {
                    case SELECT:
                        map.toggleMove(false);
                        inputState = MOVING;
                        break;
                }
                break;
            case SDLK_BACKSPACE:
                switch (inputState) {
                    case MOVING_FLOAT:
                        map.passChar('B');
                        break;
                }
                break;
            case SDLK_MINUS:
                switch (inputState) {
                    case MOVING_FLOAT:
                        map.passChar('-');
                        break;
                    case MOVING_AXIS:
                        inputState = MOVING_FLOAT;
                        map.passChar('-');
                        break;
                }
                break;
            case SDLK_0:
                switch (inputState) {
                    case MOVING_FLOAT:
                        map.passChar('0');
                        break;
                    case MOVING_AXIS:
                        inputState = MOVING_FLOAT;
                        map.passChar('0');
                        break;
                }
                break;
            case SDLK_1:
                switch (inputState) {
                    case MOVING_FLOAT:
                        map.passChar('1');
                        break;
                    case MOVING_AXIS:
                        inputState = MOVING_FLOAT;
                        map.passChar('1');
                        break;
                }
                break;
            case SDLK_2:
                switch (inputState) {
                    case MOVING_FLOAT:
                        map.passChar('2');
                        break;
                    case MOVING_AXIS:
                        inputState = MOVING_FLOAT;
                        map.passChar('2');
                        break;
                }
                break;
            case SDLK_3:
                switch (inputState) {
                    case MOVING_FLOAT:
                        map.passChar('3');
                        break;
                    case MOVING_AXIS:
                        inputState = MOVING_FLOAT;
                        map.passChar('3');
                        break;
                }
                break;
            case SDLK_4:
                switch (inputState) {
                    case MOVING_FLOAT:
                        map.passChar('4');
                        break;
                    case MOVING_AXIS:
                        inputState = MOVING_FLOAT;
                        map.passChar('4');
                        break;
                }
                break;
            case SDLK_5:
                switch (inputState) {
                    case MOVING_FLOAT:
                        map.passChar('5');
                        break;
                    case MOVING_AXIS:
                        inputState = MOVING_FLOAT;
                        map.passChar('5');
                        break;
                }
                break;
            case SDLK_6:
                switch (inputState) {
                    case MOVING_FLOAT:
                        map.passChar('6');
                        break;
                    case MOVING_AXIS:
                        inputState = MOVING_FLOAT;
                        map.passChar('6');
                        break;
                }
                break;
            case SDLK_7:
                switch (inputState) {
                    case MOVING_FLOAT:
                        map.passChar('7');
                        break;
                    case MOVING_AXIS:
                        inputState = MOVING_FLOAT;
                        map.passChar('7');
                        break;
                }
                break;
            case SDLK_8:
                switch (inputState) {
                    case MOVING_FLOAT:
                        map.passChar('8');
                        break;
                    case MOVING_AXIS:
                        inputState = MOVING_FLOAT;
                        map.passChar('8');
                        break;
                }
                break;
            case SDLK_9:
                switch (inputState) {
                    case MOVING_FLOAT:
                        map.passChar('9');
                        break;
                    case MOVING_AXIS:
                        inputState = MOVING_FLOAT;
                        map.passChar('9');
                        break;
                }
                break;
            case SDLK_PERIOD:
                switch (inputState) {
                    case MOVING_FLOAT:
                        map.passChar('.');
                        break;
                    case MOVING_AXIS:
                        inputState = MOVING_FLOAT;
                        map.passChar('.');
                        break;
                }
                break;

            default:
                break;
        }
    } else {
        switch (keysym->sym) {
            case SDLK_TAB:
                switch (inputState) {
                    case SELECT:
                        inputState = EDIT;
                        map.toggleEdit(false);
                        break;
                    case EDIT:
                        inputState = SELECT;
                        map.toggleEdit(false);
                        break;
                }
                break;
            case SDLK_ESCAPE:
                switch (inputState) {
                    case SELECT:
                        quit_tutorial(0);
                        break;
                    case EDIT:
                        inputState = SELECT;
                        map.toggleEdit(true);
                        break;
                    case MOVING:
                        map.toggleMove(true);
                        inputState = SELECT;
                        break;
                    case MOVING_AXIS:
                        map.toggleMove(true);
                        inputState = SELECT;
                        break;
                    case MOVING_FLOAT:
                        map.toggleMove(true);
                        inputState = SELECT;
                        break;
                }
                break;
            case SDLK_RETURN:
                switch (inputState) {
                    case MOVING_FLOAT:
                        map.toggleMove(false);
                        inputState = SELECT;
                        break;
                }
                break;
            case SDLK_x:
                switch (inputState) {
                    case MOVING:
                        inputState = MOVING_AXIS;
                        map.setMovingAxis(X_AXIS);
                        break;
                    case MOVING_AXIS:
                        map.setMovingAxis(X_AXIS);
                        break;
                    case MOVING_FLOAT:
                        map.passChar('e');
                        map.setMovingAxis(X_AXIS);
                        map.passChar('B');
                        break;
                }
                break;
            case SDLK_y:
                switch (inputState) {
                    case MOVING:
                        inputState = MOVING_AXIS;
                        map.setMovingAxis(Y_AXIS);
                        break;
                    case MOVING_AXIS:
                        map.setMovingAxis(Y_AXIS);
                        break;
                    case MOVING_FLOAT:
                        map.passChar('e');
                        map.setMovingAxis(Y_AXIS);
                        map.passChar('B');
                        break;
                }
                break;
            case SDLK_z:
                switch (inputState) {
                    case MOVING:
                        inputState = MOVING_AXIS;
                        map.setMovingAxis(Z_AXIS);
                        break;
                    case MOVING_AXIS:
                        map.setMovingAxis(Z_AXIS);
                        break;
                    case MOVING_FLOAT:
                        map.passChar('e');
                        map.setMovingAxis(Z_AXIS);
                        map.passChar('B');
                        break;
                }
                break;
            case SDLK_m:
                switch (inputState) {
                    case MOVING_AXIS:
                        inputState = MOVING_FLOAT;
                        break;
                }
                break;
            default:
                break;
        }
    }
}

static void process_events(void) {
    //First keystates then events.
    Uint8 *keystate = SDL_GetKeyState(NULL);
    float mf = 0.003 * delta_time; //MoveFactor
    float rf = 0.001 * delta_time; //RotateFactor
    if (inputState == SELECT || inputState == EDIT) {
        if (keystate[SDLK_a])
            kamera.translate(-mf, 0, 0, REL_LOCAL);
        if (keystate[SDLK_d])
            kamera.translate(mf, 0, 0, REL_LOCAL);
        if (keystate[SDLK_w])
            kamera.translate(0, 0, -mf, REL_LOCAL);
        if (keystate[SDLK_s])
            kamera.translate(0, 0, mf, REL_LOCAL);
        if (keystate[SDLK_SPACE])
            kamera.translate(0, mf, 0, REL_LOCAL);
        if (keystate[SDLK_LCTRL])
            kamera.translate(0, -mf, 0, REL_LOCAL);
        if (keystate[SDLK_RIGHT])
            kamera.rotate(0, -rf, 0, REL_LOCAL);
        if (keystate[SDLK_LEFT])
            kamera.rotate(0, rf, 0, REL_LOCAL);
        if (keystate[SDLK_UP])
            kamera.rotate(rf, 0, 0, REL_LOCAL);
        if (keystate[SDLK_DOWN])
            kamera.rotate(-rf, 0, 0, REL_LOCAL);

        if (keystate[SDLK_q])
            kamera.rotate(0, 0, rf, REL_LOCAL);
        if (keystate[SDLK_e])
            kamera.rotate(0, 0, -rf, REL_LOCAL);
    }
    if (inputState == MOVING || inputState == MOVING_AXIS) {
        if (keystate[SDLK_LCTRL])
            map.setSnap(true);
        else
            map.setSnap(false);
    }
    //NOW EVENTS
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        //Always handle keydowns or ups, they are split into inputState cases in handle_key
        switch (event.type) {
            case SDL_KEYDOWN:
                handle_key(&event.key.keysym, false);
                break;

            case SDL_KEYUP:
                handle_key(&event.key.keysym, true);
                break;
        }


        if (inputState == SELECT || inputState == EDIT) {
            switch (event.type) {
                case SDL_QUIT:
                    quit_tutorial(0);
                    break;
                case SDL_MOUSEMOTION:
                    if (event.motion.state == SDL_BUTTON_MMASK) {
                        krotation = krotation + Matrix(float(event.motion.yrel) / float(height)*3.14 / 2, float(event.motion.xrel) / float(width)*3.14 / 2, 0);
                        kamera.setRotation(krotation.x(), krotation.y(), 0, REL_WORLD);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        map.select(event.button.x, height - event.button.y);
                    }
                    break;
            }
        }
        if (inputState == MOVING || inputState == MOVING_AXIS) {
            switch (event.type) {
                case SDL_MOUSEMOTION:
                    map.passCamera(kamera);
                    map.passMotion(float(event.motion.xrel) / float(width)*2, float(event.motion.yrel) / float(height)*2);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        map.toggleMove(false);
                        inputState = SELECT;
                    }
                    break;
            }
        }

    }
}

static void setup_opengl(int width, int height);
static void setup_sdl(int width, int height);

int i;
void drawGrid();
#ifdef linux

int main(int argc, char *argv[])
#endif
#ifdef windows
int WinMain(int argc, char *argv[])
#endif
{
    inputState = SELECT;
    width = 1024;
    height = 768;
    setup_sdl(width, height);
    map = Map();
    SGraph sg_def(PLANE, string("Default item"));
    SGraph sg_car("res/Avent.obj", string("Lambo Avent"));
    SGraph sg_car2("res/crown_victoria.obj", string("Policeeee"));
    SGraph sg_man("res/man.obj", string("maayn"));
    map.addChild(&sg_car);
    map.addChild(&sg_car2);
    map.addChild(&sg_man);
    map.addChild(&sg_def);
    sg_def.translate(1, 0, 0, REL_WORLD);
    sg_car2.translate(-2, 0, 0, REL_WORLD);
    sg_man.translate(0, 2, 0, REL_WORLD);
    last_time = SDL_GetTicks();
    while (1) {
        delta_time = SDL_GetTicks() - last_time;
        last_time = SDL_GetTicks();
        //Move camera
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        Matrix ncamera = kamera.getPosition();
        Matrix nlookat = lookat.getPosition();
        Matrix nup = up.getPosition() - kamera.getPosition();
        gluLookAt(ncamera.x(), ncamera.y(), ncamera.z(), nlookat.x(), nlookat.y(), nlookat.z(), nup.x(), nup.y(), nup.z());
        //Render pass 1 for click detection
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Input handling
        process_events();
        //Render pass 2 for actual game
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        map.origin.renderClick(0);
        SDL_GL_SwapBuffers();
    }
    return 0;
}

void drawGrid() {

}

static void setup_sdl(int width, int height) {
    const SDL_VideoInfo *info = NULL;
    int bpp = 0;
    int flags = 0;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
        quit_tutorial(1);
    }

    /* Let's get some video information. */
    info = SDL_GetVideoInfo();

    if (!info) {
        fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
        quit_tutorial(1);
    }
    bpp = info->vfmt->BitsPerPixel;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    flags = SDL_OPENGL; //| SDL_FULLSCREEN;
    if (SDL_SetVideoMode(width, height, bpp, flags) == 0) {
        fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
        quit_tutorial(1);
    }
    setup_opengl(width, height);
}

static void setup_opengl(int width, int height) {
    kamera.addChild(&lookat);
    kamera.addChild(&up);
    lookat.setTranslation(0, 0, -5, REL_PARENT);
    up.setTranslation(0, 1, 0, REL_PARENT);
    kamera.translate(0, 1, 0, REL_WORLD);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float n = 5;
    float f = 20;
    float l = -1;
    float r = 1;
    float t = 1;
    float b = -1;
    gluPerspective(45.0f, (GLfloat) 1 / (GLfloat) 1, 0.1f, 100.0f);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glShadeModel(GL_SMOOTH);

    GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] = {0.0, 2.0, 10.0, 1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_NORMALIZE);
    glEnable(GL_RESCALE_NORMAL);
    glEnable(GL_DEPTH_TEST);
}
