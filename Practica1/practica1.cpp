// Etapa1.cpp
// Fichero principal 
////////////////////////////////////////////////////
#if defined(__APPLE__)

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#else

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#endif

const int W_WIDTH = 500; // Tamano incial de la ventana
const int W_HEIGHT = 500;
GLfloat rotateAngle; // Variable que indica el angulo de rotacion de los ejes.

// Funcion que visualiza la escena OpenGL
void Display() {
    // Borramos la escena
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    // Rotamos las proximas primitivas
    glRotatef(rotateAngle, 0.0f, 0.0f, 1.0f);
    // Creamos a continuacion dibujamos los tres poligonos
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 0.866f, 0.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.866f, 0.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-0.5f, 0.866f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.866f, 0.0f);
    glEnd();
    glPopMatrix();

    glFlush();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado
void Idle() {
    // Incrementamos el angulo
    rotateAngle += 0.3f;
    // Si es mayor que dos pi la decrementamos
    if (rotateAngle > 360)
        rotateAngle -= 360;
    // Indicamos que es necesario repintar la pantalla
    glutPostRedisplay();
}

// Funcion principal
int main(int argc, char **argv) {
    // Inicializamos la libreria GLUT
    glutInit(&argc, argv);

    // Indicamos como ha de ser la nueva ventana
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(W_WIDTH, W_HEIGHT);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);

    // Creamos la nueva ventana
    glutCreateWindow("Mi primera Ventana");

    // Indicamos cuales son las funciones de redibujado e idle
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);

    // El color de fondo sera el negro (RGBA, RGB + Alpha channel)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f);

    // Comienza la ejecucion del core de GLUT
    glutMainLoop();
    return 0;
}