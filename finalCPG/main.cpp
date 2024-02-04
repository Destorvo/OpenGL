#include <GL/glut.h>
#include <stdlib.h>
#include <windows.h>
#include "glm.h"
#include "texture.h"
#include "stdafx.h"
#include "camera.h"

#define PI 3.1415926535897932

float adelante= 0.0,
costados = 0.0,
altura = 0.0,
ojos = 8.6,
apunta = 8.6,
girar = 0.0;

double horizontal_angle = 0.0;
double rotation_speed = 1.0;  // Puedes ajustar la velocidad según tu preferencia


float anguloX = 0.0f;  // Ángulo en el eje X
float anguloY = 0.0f;  // Ángulo en el eje Y

double camera_angle;

GLMmodel* cabeza = NULL;
GLMmodel* cuello = NULL;

GLMmodel* pecho = NULL;

GLMmodel* brazo_izq = NULL;
GLMmodel* antebrazo_izq = NULL;
GLMmodel* mano_izq = NULL;

GLMmodel* brazo_der = NULL;
GLMmodel* antebrazo_der = NULL;
GLMmodel* mano_der = NULL;
GLMmodel* palo = NULL;
GLMmodel* cadena = NULL;

GLMmodel* cintura = NULL;

GLMmodel* pierna_izq = NULL;
GLMmodel* pantorrilla_izq = NULL;
GLMmodel* pie_izq = NULL;

GLMmodel* pierna_der = NULL;
GLMmodel* pantorrilla_der = NULL;
GLMmodel* pie_der = NULL;

GLuint texture;
Texture treeTextureAr[7];

void suelo() {
    glBegin(GL_QUADS);
    glVertex3f(-10.0, 0.0, -10.0); // Esquina inferior izquierda
    glVertex3f(-10.0, 0.0, 10.0);  // Esquina inferior derecha
    glVertex3f(10.0, 0.0, 10.0);   // Esquina superior derecha
    glVertex3f(10.0, 0.0, -10.0);  // Esquina superior izquierda
    glEnd();
}

bool LoadTreeTextures()
{
    int i;
    if (LoadTGA(&treeTextureAr[0], const_cast<char*>("models/tga/cabeza.tga")) && LoadTGA(&treeTextureAr[1], const_cast<char*>("models/tga/torso.tga"))  && LoadTGA(&treeTextureAr[2], const_cast<char*>("models/tga/piernas.tga"))  && LoadTGA(&treeTextureAr[3], const_cast<char*>("models/tga/pies.tga"))  && LoadTGA(&treeTextureAr[4], const_cast<char*>("models/tga/arma.tga"))  && LoadTGA(&treeTextureAr[5], const_cast<char*>("models/tga/baken.tga")))
    {
        for (i = 0; i < 6; i++) {
            glGenTextures(1, &treeTextureAr[i].texID);
            glBindTexture(GL_TEXTURE_2D, treeTextureAr[i].texID);
            glTexImage2D(GL_TEXTURE_2D, 0, treeTextureAr[i].bpp / 8, treeTextureAr[i].width, treeTextureAr[i].height, 0, treeTextureAr[i].type, GL_UNSIGNED_BYTE, treeTextureAr[i].imageData);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glEnable(GL_TEXTURE_2D);
            if (treeTextureAr[i].imageData)
            {
                free(treeTextureAr[i].imageData);
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

void init(void)
{
    glEnable(GL_DEPTH_TEST);    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    

    GLfloat light_position[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glClearColor(0.1, 0.1, 0.1, 0.0);
    LoadTreeTextures();
    glShadeModel(GL_SMOOTH);
}

void graficar(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
      
    double eyex = 25.0*cos(camera_angle*PI/180);
    double eyez = 25.0*sin(camera_angle*PI/180);


   double horizontal_offset = horizontal_angle * PI / 180;

    gluLookAt(eyex, ojos, -eyez,
              eyex + sin(horizontal_offset), ojos, -eyez + cos(horizontal_offset),
              0.0f, 1.0f, 0.0f);

    glPushMatrix();
    //glRotatef(-90, 0, 1, 0);
    glScalef(5,5,5);
    glTranslatef(adelante, altura, costados);
    glRotatef(girar, 0,0,0);
    
        glPushMatrix();
        glTranslatef(-4,1,0);
        glutSolidCube(1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0,0,0);
        suelo();
        glPopMatrix();

        glPushMatrix();
        glRotatef(90,0,1,0);
        glTranslatef(0,0,0);

            glBindTexture(GL_TEXTURE_2D, treeTextureAr[1].texID);
            glmDraw(pecho, GLM_SMOOTH |GLM_TEXTURE);
            //CUELLO
            glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, treeTextureAr[1].texID);
                glmDraw(cuello, GLM_SMOOTH |GLM_TEXTURE);
                //CABEZA
                glPushMatrix();
                    glBindTexture(GL_TEXTURE_2D, treeTextureAr[0].texID);
                    glmDraw(cabeza, GLM_SMOOTH |GLM_TEXTURE);
                glPopMatrix();
            glPopMatrix();

            glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, treeTextureAr[1].texID);
                glmDraw(brazo_izq, GLM_SMOOTH |GLM_TEXTURE);
                    glPushMatrix();    
                        glBindTexture(GL_TEXTURE_2D, treeTextureAr[1].texID);
                        glmDraw(antebrazo_izq, GLM_SMOOTH |GLM_TEXTURE);
                        glPushMatrix();
                            glBindTexture(GL_TEXTURE_2D, treeTextureAr[1].texID);
                            glmDraw(mano_izq, GLM_SMOOTH |GLM_TEXTURE);
                        glPopMatrix();
                    glPopMatrix();
            glPopMatrix();


            glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, treeTextureAr[1].texID);
                glmDraw(brazo_der, GLM_SMOOTH |GLM_TEXTURE);
                    glPushMatrix();
                        glBindTexture(GL_TEXTURE_2D, treeTextureAr[1].texID);
                        glmDraw(antebrazo_der, GLM_SMOOTH |GLM_TEXTURE);
                        glPushMatrix();
                            glBindTexture(GL_TEXTURE_2D, treeTextureAr[1].texID);
                            glmDraw(mano_der, GLM_SMOOTH |GLM_TEXTURE);
                            glPushMatrix();
                                glBindTexture(GL_TEXTURE_2D, treeTextureAr[4].texID);
                                glmDraw(palo, GLM_SMOOTH | GLM_TEXTURE);
                                glPushMatrix();
                                    glBindTexture(GL_TEXTURE_2D, treeTextureAr[5].texID);
                                    glmDraw(cadena, GLM_SMOOTH | GLM_TEXTURE);
                                glPopMatrix();
                            glPopMatrix();
                        glPopMatrix();
                    glPopMatrix();
            glPopMatrix();

            glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, treeTextureAr[2].texID);
                glmDraw(cintura, GLM_SMOOTH |GLM_TEXTURE);
                glPushMatrix();
                    glBindTexture(GL_TEXTURE_2D, treeTextureAr[2].texID);
                    glmDraw(pierna_izq, GLM_SMOOTH |GLM_TEXTURE);
                    glPushMatrix();
                        glBindTexture(GL_TEXTURE_2D, treeTextureAr[2].texID);
                        glmDraw(pantorrilla_izq, GLM_SMOOTH |GLM_TEXTURE);
                        glPushMatrix();
                            glBindTexture(GL_TEXTURE_2D, treeTextureAr[3].texID);
                            glmDraw(pie_izq, GLM_SMOOTH |GLM_TEXTURE);
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();

                glPushMatrix();
                    glBindTexture(GL_TEXTURE_2D, treeTextureAr[2].texID);
                    glmDraw(pierna_der, GLM_SMOOTH |GLM_TEXTURE);
                    glPushMatrix();
                        glBindTexture(GL_TEXTURE_2D, treeTextureAr[2].texID);
                        glmDraw(pantorrilla_der, GLM_SMOOTH |GLM_TEXTURE);
                        glPushMatrix();
                            glBindTexture(GL_TEXTURE_2D, treeTextureAr[3].texID);
                            glmDraw(pie_der, GLM_SMOOTH |GLM_TEXTURE);
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}

void redimensionar(int w, int h){
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 500.0);
}

void spinDisplay(void) {
    camera_angle = camera_angle + 0.1;
    if(camera_angle>360.0)
        camera_angle = camera_angle -360.0;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {

    switch(button) {
    case GLUT_LEFT_BUTTON:
        if(state==GLUT_DOWN)
            glutIdleFunc(spinDisplay);
        break;
    case GLUT_RIGHT_BUTTON:
        if(state==GLUT_DOWN)
            glutIdleFunc(NULL);
        break;

    default:
        break;
    }
}

void keyboard(unsigned char key, int x, int y) {    
    switch (key)
    {
        case 27:
        exit(0);
        break;

        case 'w':
        adelante+=0.1;
        break;

        case 's':
        adelante-=0.1;
        break;

        case 'a':
        costados-=0.1;
        break;

        case 'd':
        costados+=0.1;
        break;

        case '<':
        altura+=3.1;
        if (altura > 3.1)
        altura=3.1;

        case '>':
        altura-=3.0;
        if (altura<0.0)
        altura=-0.0;
        break;

        case 'j':
            horizontal_angle -= rotation_speed;
            break;

        case 'l':
            horizontal_angle += rotation_speed;
            break;
        break;

        case 'i':
        apunta+=0.5;
        break;
        case 'k':
        apunta-=0.5;
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(1400, 900);
    glutInitWindowPosition(300, 50);
    glutCreateWindow("OpenGl project");
    init();
    
    cabeza = glmReadOBJ(const_cast<char*>("models/obj/cabeza.obj"));
    cuello = glmReadOBJ(const_cast<char*>("models/obj/cuello.obj"));

    pecho = glmReadOBJ(const_cast<char*>("models/obj/pecho.obj"));

    brazo_izq = glmReadOBJ(const_cast<char*>("models/obj/brazo_izq.obj"));
    antebrazo_izq = glmReadOBJ(const_cast<char*>("models/obj/antebrazo_izq.obj"));
    mano_izq = glmReadOBJ(const_cast<char*>("models/obj/mano_izq.obj"));

    brazo_der = glmReadOBJ(const_cast<char*>("models/obj/brazo_der.obj"));
    antebrazo_der = glmReadOBJ(const_cast<char*>("models/obj/antebrazo_der.obj"));
    mano_der = glmReadOBJ(const_cast<char*>("models/obj/mano_der.obj"));
    palo = glmReadOBJ(const_cast<char*>("models/obj/palo.obj"));
    cadena = glmReadOBJ(const_cast<char*>("models/obj/cadena.obj"));

    cintura = glmReadOBJ(const_cast<char*>("models/obj/cintura.obj"));

    pierna_izq = glmReadOBJ(const_cast<char*>("models/obj/pierna_izq.obj"));
    pantorrilla_izq = glmReadOBJ(const_cast<char*>("models/obj/pantorrilla_izq.obj"));
    pie_izq = glmReadOBJ(const_cast<char*>("models/obj/pie_izq.obj"));

    pierna_der = glmReadOBJ(const_cast<char*>("models/obj/pierna_der.obj"));
    pantorrilla_der = glmReadOBJ(const_cast<char*>("models/obj/pantorrilla_der.obj"));
    pie_der = glmReadOBJ(const_cast<char*>("models/obj/pie_der.obj"));


    glutDisplayFunc(graficar);
    glutReshapeFunc(redimensionar);
    glutKeyboardFunc(keyboard);

    //glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
