
#include <GL/glut.h>
#include <stdlib.h>
#include <windows.h>

#include "glm.h"
#include "texture.h"
#include "stdafx.h"


#define PI 3.1415926535897932

double camera_angle;
float velocidadCadena = 0;
float velocidadBrazoDer = 0.3, velocidadBrazoIzq = 0.3, velocidadPiernaIzq = 0.2, velocidadPiernaDer = 0.2;
int dir2 = 1, dir3 = 1, dir4 = 1;

float ypoz = 0, zpoz = 0, anguloBrazoIzq = 10, anguloBrazoDer = -10, anguloPiernaIzq = 0, anguloPiernaDer = 0;

    int windowHeight ;
    int windowWidth;
    bool forward = false;
    bool backward = false;
    bool left = false;
    bool right = false;
    float previousTime = 0;
    float moveforward_backward = 50.0f;
    float moveSiteward = 30.0f;
    //Gibt die Position des Betrachters an.
    float eyeX = 0.0f;
    float eyeY = 1.85f;
    float eyeZ = 3.0f;
    //centerX, centerY, centerZ 	Gibt die Position des Refernenzpunktes an, auf den "geblickt" wird.
    float centerX = 0.0f;
    float centerY = 0.0f;
    float centerZ = 0.0f;
    //upX, upY, upZ 	Gibt die Richtung des Vektors an, der nach oben zeigt.
    float mouseDirectionX = 0;
    float mouseDirectionY = 0;

    bool mouseWarp = true;



	void changeSize(int w, int h)
	{
		int windowHeight = w;
		int windowWidth = h;
		// Prevent a divide by zero, when window is too short
		// (you cant make a window of zero width).
		if(h == 0)
			h = 1;
		float ratio = 1.0* w / h;

		// Use the Projection Matrix
		glMatrixMode(GL_PROJECTION);

	        // Reset Matrix
		glLoadIdentity();

		// Set the viewport to be the entire window
		glViewport(0, 0, w, h);

		// Set the correct perspective.
		gluPerspective(45,ratio,1,1000);

		// Get Back to the Modelview
		glMatrixMode(GL_MODELVIEW);
	}


	void mousePosition(int x, int y)
	{
		if(mouseWarp)
		{
			mouseDirectionX -= (x - windowWidth/2) * 0.1f;
			mouseDirectionY += (y - windowHeight/2) * 0.1f;
			if(mouseDirectionY > 180) mouseDirectionY = 180;
			else if(mouseDirectionY < 0) mouseDirectionY = 0;
		}
	}

	void renderCameraView()
	{

	    int currentTime = glutGet(GLUT_ELAPSED_TIME);
	    int timeInterval = currentTime - previousTime;

	    //Berechne Blickrichtung
	    GLdouble tmpEyeX, tmpEyeY, tmpEyeZ;

	    tmpEyeX = sin(mouseDirectionY*PI/180)*sin(mouseDirectionX*PI/180);
	    tmpEyeZ = sin(mouseDirectionY*PI/180)*cos(mouseDirectionX*PI/180);
	    tmpEyeY = cos(mouseDirectionY*PI/180);

	    //Bewegung Berechnen
	    if(forward)
	    {
	    	eyeX += timeInterval * tmpEyeX / 10;
	    	eyeY += timeInterval * tmpEyeY / 10;
	    	eyeZ += timeInterval * tmpEyeZ / 10;
	    }
	    if(backward)
	    {
	    	eyeZ -= timeInterval * tmpEyeZ / 10;
	    	eyeY -= timeInterval * tmpEyeY / 10;
	    	eyeX -= timeInterval * tmpEyeX / 10;
	    }
	    if(left)
	    {
	    	eyeX += timeInterval * sin((mouseDirectionX+90)*PI/180) / 10;
	    	eyeZ += timeInterval * cos((mouseDirectionX+90)*PI/180) / 10;
	    }
	    if(right)
	    {
	    	eyeX -= timeInterval * sin((mouseDirectionX+90)*PI/180) / 10;
	    	eyeZ -= timeInterval * cos((mouseDirectionX+90)*PI/180) / 10;
	    }


	    previousTime = currentTime;

	    glLoadIdentity();


		gluLookAt(	eyeX, eyeY, eyeZ,
					eyeX + tmpEyeX, eyeY + tmpEyeY, eyeZ + tmpEyeZ ,
					0.0f, 10.0f,  0.0f);


		//Maus wieder in die mitte Positionieren
		if(mouseWarp)
			glutWarpPointer(windowWidth / 2, windowHeight / 2);
	}

	//Getter und Setter

	void toggleMouseWarp()
	{
		mouseWarp = !mouseWarp;
	}
	void goFarwardStart()
	{
		forward = true;
	}

	void goFarwardStop()
	{
		forward = false;
	}

	void goBackwardStart()
	{
		backward = true;
	}

	void goBackwardStop()
	{
		backward = false;
	}

	void goLeftStart()
	{
		left = true;
	}

	void goLeftStop()
	{
		left = false;

	}

	void goRightStart()
	{
		right = true;
	}

	void goRightStop()
	{
		right = false;
	}


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
    //glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glClearColor(0.7, 0.7, 0.8, 0.0);
    LoadTreeTextures();
    glShadeModel(GL_SMOOTH);
}

void graficar(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    double eyex = 25.0*cos(camera_angle*PI/180);
    double eyez = 25.0*sin(camera_angle*PI/180);

    gluLookAt(eyex, 10.0, -eyez, 0.0, 3.0, 0.0, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(10,10,10);
    glutSolidCube(1);
    glPopMatrix();
    //glRotatef(-70, 0, 1, 0);
    glTranslatef(0, -1, 0);
    glScalef(4.7,4.7,4.7);

    glPushMatrix();
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


    glutSwapBuffers();
}

void redimensionar(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 500.0);
}
/*
void animate(int i) {
    ypoz += 0.1;
    if (ypoz > 360) ypoz = 0;
    glutPostRedisplay();
    glutTimerFunc(16, animate, 1);
}


void girarBrazo(int i) {
    anguloBrazoDer += velocidadBrazoDer;
    if(anguloBrazoDer >= 10 || anguloBrazoDer <= -20){
        velocidadBrazoDer = velocidadBrazoDer * -1;
    }
    glutPostRedisplay();
    glutTimerFunc(16, girarBrazo, 1);
}

void girarBrazoIzq(int i) {
    anguloBrazoIzq -= velocidadBrazoIzq;
    if(anguloBrazoIzq >= 10 || anguloBrazoIzq <= -20){
        velocidadBrazoIzq = velocidadBrazoIzq * -1;
    }
    glutPostRedisplay();
    glutTimerFunc(16, girarBrazoIzq, 1);
}

void girarPiernaIzq(int i) {
    anguloPiernaIzq -=velocidadPiernaIzq;
    if(anguloPiernaIzq >= 10 || anguloPiernaIzq <= -13){
        velocidadPiernaIzq = velocidadPiernaIzq * -1;
    }
    glutPostRedisplay();
    glutTimerFunc(16, girarPiernaIzq, 1);
}

void girarPiernaDer(int i) {
    anguloPiernaDer +=velocidadPiernaDer;
    if(anguloPiernaDer >= 10 || anguloPiernaDer <= -13){
        velocidadPiernaDer = velocidadPiernaDer * -1;
    }
    glutPostRedisplay();
    glutTimerFunc(16, girarPiernaDer, 1);
}

void girarCadena(int i) {
    velocidadCadena += 3.7;
	if (velocidadCadena>360) velocidadCadena = 0;
	glutPostRedisplay();
	glutTimerFunc(16, girarCadena, 1);
}
*/
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

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(1000, 900);
    glutInitWindowPosition(300, 50);
    glutCreateWindow("OpenGl project");
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
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
    glutReshapeFunc(redimensionar);/*
    glutTimerFunc(16, girarBrazo, 1);
    glutTimerFunc(16, girarBrazoIzq, 1);
    glutTimerFunc(16, girarCadena, 1);
    glutTimerFunc(16, girarPiernaIzq, 1);
    glutTimerFunc(16, girarPiernaDer, 1);*/
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
