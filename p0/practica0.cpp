#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>



void Ejes (int width) {  
  glLineWidth(width);

  glBegin(GL_LINES);
    glColor3f(1.0,0.0,0.0);
    glVertex3f(-1.0,0.0,0.0);
    glVertex3f(1.0,0.0,0.0);

    glColor3f(0.0,1.0,0.0);
    glVertex3f(0.0,-1.0,0.0);
    glColor3f(1.0,1.0,0.0);
    glVertex3f(0.0,1.0,0.0);   
  glEnd();
}

void Circle (GLfloat radio, GLfloat cx, GLfloat cy, GLint n, GLenum modo) {
  int i;
  if (modo==GL_LINE) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else if (modo==GL_FILL) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
  glBegin( GL_POLYGON );
  for (i=0;i<n;i++)
    glVertex2f( cx+radio*cos(2.0*M_PI*i/n), cy+radio*sin(2.0*M_PI*i/n));
  glEnd();
}

void Monigote () {
  glLineWidth(1);

  // Cara
  glColor3f(1.0,0.8,0.6);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_POLYGON);
    glVertex3f(-0.2,0.0,0.0);
    glVertex3f(0.2,0.0,0.0);
    glVertex3f(0.2,0.55,0.0);
    glVertex3f(-0.2,0.55,0.0);
  glEnd(); 

  glColor3f(0.0,0.0,0.);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_POLYGON);
    glVertex3f(-0.2,0.0,0.0);
    glVertex3f(0.2,0.0,0.0);
    glVertex3f(0.2,0.55,0.0);
    glVertex3f(-0.2,0.55,0.0);
  glEnd();

  // Orejas
  glColor3f(0.7,0.8,0.6);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_POLYGON);
    glVertex3f(0.2,0.2,0.0);
    glVertex3f(0.25,0.2,0.0);
    glVertex3f(0.25,0.3,0.0);
    glVertex3f(0.2,0.3,0.0);
  glEnd();
  glColor3f(0.0,0.0,0.);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_POLYGON);
    glVertex3f(0.2,0.2,0.0);
    glVertex3f(0.25,0.2,0.0);
    glVertex3f(0.25,0.3,0.0);
    glVertex3f(0.2,0.3,0.0);
  glEnd(); 

  glColor3f(0.7,0.8,0.6);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_POLYGON);
    glVertex3f(-0.2,0.2,0.0);
    glVertex3f(-0.25,0.2,0.0);
    glVertex3f(-0.25,0.3,0.0);
    glVertex3f(-0.2,0.3,0.0);
  glEnd();
  glColor3f(0.0,0.0,0.);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_POLYGON);
    glVertex3f(-0.2,0.2,0.0);
    glVertex3f(-0.25,0.2,0.0);
    glVertex3f(-0.25,0.3,0.0);
    glVertex3f(-0.2,0.3,0.0);
  glEnd(); 

  // Gorro
  glColor3f(0.3,0.2,0.3);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_POLYGON);
    glVertex3f(-0.1,0.55,0.0);
    glVertex3f(0.1,0.55,0.0);
    glVertex3f(0.0,0.65,0.0);
  glEnd();
  glColor3f(0.0,0.0,0.);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glVertex3f(-0.1,0.55,0.0);
    glVertex3f(0.1,0.55,0.0);
    glVertex3f(0.0,0.65,0.0);
  glEnd();

  // Ojos
  glColor3f(0.0,0.0,0.0);
  Circle(0.05,-0.1,0.3,20,GL_FILL);
  Circle(0.05,0.1,0.3,20,GL_FILL);

}


static void Init() {
  glShadeModel(GL_SMOOTH);
}


static void Reshape(int width, int height) {
  glViewport(0, 0, (GLint)width, (GLint)height);
  glOrtho (-1.0, 1.0,-1.0, 1.0, -10, 10.0);
}

static void Display() {

  glClearColor(0.5,0.5,0.5,0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  Ejes(6);
  Monigote();

  glFlush();
}


static void Keyboard(unsigned char key, int x, int y) {
 
  if (key==27) {
    exit(0);
  }

  if (key=='a') {
    glShadeModel(GL_SMOOTH);
    Display();
  }

  if (key=='s') {
  glShadeModel(GL_FLAT);
  Display();
  }
}


int main(int argc, char **argv){
  glutInit(&argc,argv);
  glutInitDisplayMode( GLUT_RGB );

  glutInitWindowPosition( 20, 100 );
  glutInitWindowSize(500, 500 );
  glutCreateWindow("Practica 0 IG");


  Init();

  glutReshapeFunc(Reshape);
  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);

  glutMainLoop( );

  return 0;
}

