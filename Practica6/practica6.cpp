
#if defined(__APPLE__)

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "Camera.h"

#else

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "camera.h"

#endif

#define _USE_MATH_DEFINES

#include <cmath>
#include <string>
#include <iostream>


using namespace std;

const GLint W_WIDTH = 640;
const GLint W_HEIGHT = 480;

const GLint W_WINDOW = 2;
const GLint H_WINDOW = 2;

// Sombreado
GLint sombreado = 0;

// Control de dispositivos
GLint leftMouseButton;
GLint rightMouseButton;
GLint mouseX = -1;
GLint mouseY = -1;

// Control de la proyeccion
GLfloat NEARFACE = 1.0f;
GLfloat FARFACE = 750.0f;
GLfloat FOV = 45.0f;

// Control de la camara
Camera **cameras = new Camera *[4];
GLint selectedCamera;
GLfloat const SPEED = 0.2;

//Estado de las luces
GLboolean statusLight0 = true;
GLboolean statusLight1 = true;
GLboolean statusLight2 = true;
GLboolean statusLight3 = true;
GLboolean statusNormal = true;

//Configurar luces
GLfloat lightPosition1[] = { 1, 0, 0, 1 };
GLfloat lightColor1[] = { 1.0, 1.0, 0.4, 1.0 };

GLfloat lightPosition2[] = { 0, 1, 0, 1 };
GLfloat lightColor2[] = { 1.0, 0.4, 1, 1.0 };

GLfloat lightPosition3[] = { 0, 0, 0, 1 };
GLfloat lightColor3[] = { 0.4, 1, 1, 1.0 };

GLfloat lightPosition4[] = { 0, 0, 1, 1 };
GLfloat lightColor4[] = { 1.0, 1, 1, 1.0 };

void init(void) {

    cameras[TOP_VIEW] = new Camera({0.0f, 20.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f},
                                   FRONT_VIEW); // Vista de enfrente
    cameras[FRONT_VIEW] = new Camera({0.0f, 0.0f, 20.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f},
                                     TOP_VIEW); // Vista de arriba
    cameras[SIDE_VIEW] = new Camera({-20.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
                                    SIDE_VIEW); // Vista de lado izquierdo
    cameras[FREE_VIEW] = new Camera({30.0f, 20.0f, 15.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f},
                                    FREE_VIEW); // Camara movil

    selectedCamera = FREE_VIEW;
    cameras[selectedCamera]->setThetaAngle(-M_PI / 3.0f);
    cameras[selectedCamera]->setPhiAngle(M_PI / 2.8f);
    cameras[selectedCamera]->updateOrientation();
}

void reshape(GLsizei width, GLsizei height) {

    GLfloat aspect = (GLfloat) width / (GLfloat) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);                    // Select Projection matrix
    glLoadIdentity();                               // Reset the Projection matrix
    gluPerspective(FOV, aspect, NEARFACE, FARFACE);

    glMatrixMode(GL_MODELVIEW);
}


void paintSectors() {
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f); // Linea horizontal
    glVertex3f(100.0f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 100.0f, 0.0f); // Linea vertical
    glVertex3f(0.0f, -100.0, 0.0f);

    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);    // Linea eje z
    glVertex3f(0.0f, 0.0f, -100.0f);
    glEnd();
}

void paintGrid() {
    for (int i = 0; i < 40; i++) {

        glPushMatrix();

        if (i < 20)
            glTranslatef(0, 0, i);
        if (i >= 20) {
            glTranslatef(i - 20, 0, 0);
            glRotatef(-90, 0, 1, 0);
        }

        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex3f(0.0f, -0.1f, 0.0f);
        glVertex3f(19.0f, -0.1f, 0.0f);
        glEnd();
        glPopMatrix();

        //paintSectors();
    }
}


void cambiarSombreado() {
    if (sombreado == 0) {
        glShadeModel(GL_FLAT);
    } else {
        glShadeModel(GL_SMOOTH);
    }
}

// Funcion que renderiza la escena OpenGL
void render() {

    // Borramos la escena
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    // Resetear transformaciones
    vector<float> cameraPosition = cameras[selectedCamera]->getPosition();
    vector<float> lookDirection = cameras[selectedCamera]->getDirection();
    vector<float> cameraRotation = cameras[selectedCamera]->getRotation();

    gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],  // Posicion de la camara
              cameraPosition[0] + lookDirection[0],                     // Dirección donde apunta la camara
              cameraPosition[1] + lookDirection[1],
              cameraPosition[2] + lookDirection[2],
              cameraRotation[0], cameraRotation[1], cameraRotation[2]); // Vector de rotacion

    // Pintar suelo
    glColor3f(1.0f, 1.0f, 1.0f);
    paintGrid();
    //Pintar las figuras
    glColor3f(0.6f, 0.6f, 0.6f);
    glTranslatef(10, 5, 10);
    glutSolidTeapot(4.0);

    cambiarSombreado();


    glutSwapBuffers();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado
void idle() {

    //Repintar la pantalla
    glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            selectedCamera = FREE_VIEW;
            break;
        case GLUT_KEY_F2:
            selectedCamera = TOP_VIEW;
            break;
        case GLUT_KEY_F3:
            selectedCamera = FRONT_VIEW;
            break;
        case GLUT_KEY_F4:
            selectedCamera = SIDE_VIEW;
            break;
        case GLUT_KEY_LEFT:
            cameras[selectedCamera]->moveRight(SPEED);
            break;
        case GLUT_KEY_UP:
            cameras[selectedCamera]->moveForward(SPEED);
            break;
        case GLUT_KEY_DOWN:
            cameras[selectedCamera]->moveBackward(SPEED);
            break;
        case GLUT_KEY_RIGHT:

            break;
    }
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
        case 'a':
        case 'A':
            lightPosition1[0] = lightPosition1[0] + 0.1;
            glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
            break;
        case 'z':
        case 'Z':
            lightPosition1[0] = lightPosition1[0] - 0.1;
            glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
            break;
        case 's':
        case 'S':
            lightPosition1[1] = lightPosition1[1] + 0.1;
            glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
            break;
        case 'x':
        case 'X':
            lightPosition1[1] = lightPosition1[1] - 0.1;
            glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
            break;
        case 'd':
        case 'D':
            lightPosition1[2] = lightPosition1[2] + 0.1;
            glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
            break;
        case 'c':
        case 'C':
            lightPosition1[2] = lightPosition1[2] - 0.1;
            glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
            break;
        case 32:    // espacio
            if (sombreado == 0) {
                sombreado = 1;
            } else {
                sombreado = 0;
            }

            cambiarSombreado();
    }
}

// Acciones del mouse [PULSAR]
void mouseCallback(int button, int state, int x, int y) {

    if (button == GLUT_LEFT_BUTTON)
        leftMouseButton = state;
    else if (button == GLUT_RIGHT_BUTTON)
        rightMouseButton = state;

    mouseX = x;
    mouseY = y;
}

// Acciones del mouse [MOVER]
void mouseMotion(int x, int y) {

    // Solo se ejecutara cuando el boton este pulsado.
    if (leftMouseButton == GLUT_DOWN) {

        float thetaAngle = cameras[selectedCamera]->getThetaAngle();
        float phiAngle = cameras[selectedCamera]->getPhiAngle();

        thetaAngle += (y - mouseY) * 0.002f;
        phiAngle += (mouseX - x) * 0.002f;

        cameras[selectedCamera]->setThetaAngle(thetaAngle);
        cameras[selectedCamera]->setPhiAngle(phiAngle);

        cameras[selectedCamera]->updateOrientation();
    }

    mouseX = x;
    mouseY = y;
}


void switchLights(int item) {

    switch (item) {
        case 0:
            statusLight0 = !statusLight0;
            if (statusLight0) {
                glEnable(GL_LIGHT0);
                glutChangeToMenuEntry(item + 1, "Disable diffuse light", item);
            } else {
                glDisable(GL_LIGHT0);
                glutChangeToMenuEntry(item + 1, "Enable diffuse light", item);
            }
            break;
        case 1:
            statusLight1 = !statusLight1;
            if (statusLight1) {
                glEnable(GL_LIGHT1);
                glutChangeToMenuEntry(item + 1, "Disable specular light", item);
            } else {
                glDisable(GL_LIGHT1);
                glutChangeToMenuEntry(item + 1, "Enable specular light", item);
            }
            break;
        case 2:
            statusLight2 = !statusLight2;
            if (statusLight2) {
                glEnable(GL_LIGHT2);
                glutChangeToMenuEntry(item + 1, "Disable ambient light", item);
            } else {
                glDisable(GL_LIGHT2);
                glutChangeToMenuEntry(item + 1, "Enable ambient light", item);
            }
            break;
        case 3:
            statusNormal = !statusNormal;
            if (statusNormal) {
                glEnable(GL_NORMALIZE);
                glutChangeToMenuEntry(item + 1, "Disable GL_NORMAL", item);
            } else {
                glDisable(GL_NORMALIZE);
                glutChangeToMenuEntry(item + 1, "Enable GL_NORMAL", item);
            }
            break;
    }

}

// Funcion principal
int main(int argc, char **argv) {
    // Inicializamos la libreria GLUT
    glutInit(&argc, argv);

    // Indicamos posición y tamaño de la ventana
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - W_WIDTH) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - W_HEIGHT) / 2);
    glutInitWindowSize(W_WIDTH, W_HEIGHT);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    // Creamos la nueva ventana
    glutCreateWindow("Etapa 5");

    //Configurar menu
    glutCreateMenu(switchLights);
    glutAddMenuEntry("Disable Diffuse light", 0);
    glutAddMenuEntry("Disable Specular light", 1);
    glutAddMenuEntry("Disable Ambient light", 2);
    glutAddMenuEntry("Disable GL_NORMAL", 3);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor2);
    glEnable(GL_LIGHT1);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, lightColor2);

    glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
    glLightfv(GL_LIGHT2, GL_AMBIENT, lightColor3);
    glEnable(GL_LIGHT2);
    // El color de fondo sera el negro (RGBA, RGB + Alpha channel)
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    init();

    // Indicamos cuales son las funciones de redibujado, idle y reshape
    glutDisplayFunc(render);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotion);

    // Habilitar
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Comienza la ejecucion del core de GLUT (RENDERING)
    glutMainLoop();
    return 0;
}
