
#if defined(__APPLE__)

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#else

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#endif

#include <cmath>
#include <string>
#include <iostream>

using namespace std;

const GLint W_WINDOW = 2;
const GLint H_WINDOW = 2;

// Variables de proyección
GLfloat const NEARFACE = 1.0f;
GLfloat const FARFACE = 100.0f;
GLfloat const FOV = 45.0f;

//Variables luces
GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat ambientLightPosition[] = {0.0f, 0.9f, 0.0f};
GLfloat difuseLightPosition[] = {0.0f, 0.9f, 0.0f};
GLfloat specularLightPosition[] = {0.0f, 0.9f, 0.0f};
GLfloat spot_direction[] = {0.0f, -0.5f, 0.0f};
GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat specularLight[] = {1.0f, 1.0f, 0.0f, 1.0f};
GLfloat difuseLight[] = {0.8f, 0.8f, 0.8f, 1.0f};


//Variables enunciado
const int W_WIDTH = 500;
const int W_HEIGHT = 500;

GLfloat pos_zero[] = {0, 0, 0, 1};
GLfloat dir_y_neg[] = {0, -1, 0};
GLfloat light1_dif[] = {0.7, 0.7, 0.7, 1};
GLfloat eye_x = 10;
GLfloat eye_y = 12;
GLfloat eye_z = 10;
GLfloat torre_x = 0;
GLfloat brazo_angulo = 0;
GLfloat enganche_pos = 2.0f;
GLfloat cubo_x = 3;
GLfloat cubo_y = 0.5; //(caso cubo blanco, valores libres)
GLfloat cubo_z = 3.5;

bool camara = false;        //(para cambio de cámara desde teclado)
int apertura = 45;          //(para cambio de apertura foco desde teclado)

void reshape(GLsizei width, GLsizei height) {

    GLfloat aspect = (GLfloat) width / (GLfloat) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);   // Seleccionar la matriz proyeccion
    glLoadIdentity();              // Introducir dentro de la pila la matriz identidad

    gluPerspective(apertura, aspect, NEARFACE, FARFACE);

    glMatrixMode(GL_MODELVIEW);
}

void drawAxisXYZ() {
    //Eje X -> RED
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(8.0f, 0.0f, 0.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(8.0f, 0.0f, 0.0f);
    glVertex3f(0.95f, 0.01f, 0.0f);
    glVertex3f(0.95f, -0.01f, 0.0f);
    glEnd();

    //Eje Y -> GREEN
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 8.0f, 0.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 8.0f, 0.0f);
    glVertex3f(0.01f, 0.95f, -0.01f);
    glVertex3f(-0.01f, 0.95f, 0.01f);
    glEnd();

    //Eje Z -> BLUE
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 8.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 8.0f);
    glVertex3f(0.0f, 0.01f, 0.95f);
    glVertex3f(0.0f, -0.01f, 0.95f);
    glEnd();
}

//Dibujar suelo
void drawSuelo() {
    glPushMatrix();
    glColor3f(0.65f, 0.33f, 0.07f);
    glBegin(GL_QUADS);

    glVertex3f(4.0f, -0.1f, 4.0f);
    glVertex3f(4.0f, -0.1f, -4.0f);
    glVertex3f(-4.0f, -0.1f, -4.0f);
    glVertex3f(-4.0f, -0.1f, 4.0f);
    glEnd();
    glPopMatrix();
}

//Dibujar base grua
void drawBaseGrua() {
    //Rectangulo rojo
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(3.0f, 0.0f, 0.2f);
    glVertex3f(3.0f, 0.0f, -0.2f);
    glVertex3f(-3.0f, 0.0f, -0.2f);
    glVertex3f(-3.0f, 0.0f, 0.2f);
    glEnd();
    glPopMatrix();
}

//Dibujar torre grua
void drawTorreGrua() {
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(torre_x, 0.0f, 0.2f);
    glVertex3f(torre_x, 0.0f, -0.2f);
    glVertex3f(torre_x, 4.0f, -0.2f);
    glVertex3f(torre_x, 4.0f, 0.2f);
    glEnd();
    glPopMatrix();
}

//Dibujar brazo grua
void drawBrazoGrua() {
    glPushMatrix();

    //Rotación del brazo
    glTranslatef(-torre_x, 0.0f, 0.0f);
    glRotatef(brazo_angulo, 0.0f, 1.0f, 0.0f);
    glTranslatef(torre_x, 0.0f, 0.0f);

    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(torre_x - 0.2f, 4.0f, 0.0f);
    glVertex3f(torre_x + 0.2f, 4.0f, 0.0f);
    glVertex3f(torre_x, 4.0f, 4.0f);

    glEnd();
    glPopMatrix();
}

//Dibujar enganche grua
void drawEngancheGrua() {
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.1f);

    //Rotación del enganche
    glRotatef(brazo_angulo, 0.0f, 1.0f, 0.0f);

    glBegin(GL_POLYGON);
    glVertex3f(torre_x, 4.0f, enganche_pos);
    glVertex3f(torre_x + 0.5f, 0.6f, enganche_pos);
    glVertex3f(torre_x - 0.5f, 0.6f, enganche_pos);
    glEnd();
    glPopMatrix();
}

//Dibujar enganche grua
void drawCubos() {

    //Cubo blanco
    glPushMatrix();
    glTranslatef(cubo_x, cubo_y, cubo_z);
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Cubo azul
    glPushMatrix();
    glTranslatef(-3.0f, cubo_y, -1.0f);
    glColor3f(0.0f, 0.2f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Cubo verde
    glPushMatrix();
    glTranslatef(-1.5f, cubo_y, -2.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glutSolidCube(1.1f);
    glPopMatrix();

    //Cubo rojo
    glPushMatrix();
    glTranslatef(0.0f, cubo_y, -3.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidCube(1.2f);
    glPopMatrix();
}

// Funcion que renderiza la escena OpenGL
void render() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eye_x, eye_y, eye_z,
              0, 0, 0,
              0, 1, 0); // Posicionar la cámara

    //Dibujar ejes 3D
    drawAxisXYZ();

    //Draw suelo
    drawSuelo();

    //Draw base de la grua
    drawBaseGrua();

    //Draw torre de la grua
    drawTorreGrua();

    //Draw brazo de la grua
    drawBrazoGrua();

    //Draw enganche de la grua
    drawEngancheGrua();

    //Draw cargas (cubos)
    drawCubos();

    //Luz especular
    glPushMatrix();
    glLightfv(GL_LIGHT2, GL_POSITION, specularLightPosition);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glPopMatrix();

    glutSwapBuffers();
}

// That function only executes in dead cycles of the processor
void idle() {
    glutPostRedisplay(); // Repaint screen
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
        case ' ':
            enganche_pos += 0.1;
            break;
        case 'a':
        case 'A':
            enganche_pos += 0.1;
            break;
        case 'z':
        case 'Z':
            enganche_pos -= 0.1;
            break;
        case 's':
        case 'S':
            torre_x += 0.1;
            break;
        case 'x':
        case 'X':
            torre_x -= 0.1;
            break;
        case 'd':
        case 'D':
            brazo_angulo += 2.5f;
            break;
        case 'c':
        case 'C':
            brazo_angulo -= 2.5f;
            break;
        case 'f':
        case 'F':
            apertura += 0.1;
            reshape(W_WIDTH, W_HEIGHT);
            break;
        case 'v':
        case 'V':
            apertura -= 0.1;
            reshape(W_WIDTH, W_HEIGHT);
            break;
    }
}


// Activar luces
void enableLighting() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Configurar luces
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, difuseLightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difuseLight);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_POSITION, ambientLightPosition);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT2, GL_POSITION, specularLightPosition);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20.0f);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 4);
    glEnable(GL_LIGHT2);
}

// Main function
int main(int argc, char **argv) {
    // Inicializamos la libreria GLUT
    glutInit(&argc, argv);

    // Indicamos posición y tamaño de la ventana
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - W_WIDTH) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - W_HEIGHT) / 2);
    glutInitWindowSize(W_WIDTH, W_HEIGHT);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    // Creamos la nueva ventana
    glutCreateWindow("Examen junio");

    //Activar luces
    enableLighting();

    // Indicamos cuales son las funciones de redibujado, idle y reshape
    glutDisplayFunc(render);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);

    //Función para escuchar las teclas
    glutKeyboardFunc(keyboard);

    // Fondo blanco
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Comienza la ejecucion del core de GLUT (RENDERING)
    glutMainLoop();
    return 0;
}