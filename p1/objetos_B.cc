//**************************************************************************
// Práctica 1 usando objetos
//**************************************************************************

#include "objetos_B.h"

//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D() { }

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor) {
  int i;
  glPointSize(grosor);
  glColor3f(r,g,b);
  glBegin(GL_POINTS);
  for (i=0;i<vertices.size();i++) {
    glVertex3fv((GLfloat *) &vertices[i]);
  }
  glEnd();
}

//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D() { }

//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor) {
  int i;
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glLineWidth(grosor);
  glColor3f(r,g,b);
  glBegin(GL_TRIANGLES);
  for (i=0;i<caras.size();i++) {
    glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
  }
  glEnd();
}

void _triangulos3D::draw_solido(float r, float g, float b) {
  int i;
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glColor3f(r,g,b);
  glBegin(GL_TRIANGLES);
  for (i=0;i<caras.size();i++) {
    glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
  }
  glEnd();
}

//*************************************************************************
// dibujar en modo sólido con apariencia de ajedrez
//*************************************************************************

void _triangulos3D::draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2) {
  int i;
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBegin(GL_TRIANGLES);
  for (i=0;i<caras.size();i++) {
    if (i%2==0) {
      glColor3f(r1,g1,b1);
    } else {
      glColor3f(r2,g2,b2);
    }
    glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
  }
  glEnd();
}

//*************************************************************************
// dibujar en modo sólido con multicolor
//*************************************************************************

void _triangulos3D::draw_solido_multicolor() {
  int i;
  double r;
  double g;
  double b;
  double aux;
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBegin(GL_TRIANGLES);
  for (i=0;i<caras.size();i++) {
    r = (i+50+20*i)*255 / 255.0;
    g = (i+0+91*i)%255 / 255.0;
    b = (i+200+117*i)%255 / 255.0;
    glColor3f(r, g, b);
    glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
  }
  glEnd();
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam) {

  //vertices
  vertices.resize(8); 
  vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
  vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
  vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
  vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
  vertices[4].x=-tam;vertices[4].y=2*tam;vertices[4].z=tam;
  vertices[5].x=tam;vertices[5].y=2*tam;vertices[5].z=tam;
  vertices[6].x=tam;vertices[6].y=2*tam;vertices[6].z=-tam;
  vertices[7].x=-tam;vertices[7].y=2*tam;vertices[7].z=-tam;


  // triangulos
  caras.resize(12);
  // Base
  caras[0]._0=0;caras[0]._1=3;caras[0]._2=2;
  caras[1]._0=0;caras[1]._1=2;caras[1]._2=1;
  // Lado A
  caras[2]._0=2;caras[2]._1=1;caras[2]._2=5;
  caras[3]._0=2;caras[3]._1=5;caras[3]._2=6;
  // Lado opuesto a A
  caras[4]._0=0;caras[4]._1=3;caras[4]._2=7;
  caras[5]._0=0;caras[5]._1=7;caras[5]._2=4;
  // Lado B
  caras[6]._0=3;caras[6]._1=2;caras[6]._2=6;
  caras[7]._0=3;caras[7]._1=6;caras[7]._2=7;
  // Lado opuesto a B
  caras[8]._0=1;caras[8]._1=0;caras[8]._2=4;
  caras[9]._0=1;caras[9]._1=4;caras[9]._2=5;
  // Cara superior
  caras[10]._0=7;caras[10]._1=5;caras[10]._2=4;
  caras[11]._0=7;caras[11]._1=6;caras[11]._2=5;

}


//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al) {

  //vertices 
  vertices.resize(5); 
  vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
  vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
  vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
  vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
  vertices[4].x=0;vertices[4].y=al;vertices[4].z=0;

  caras.resize(6);
  caras[0]._0=0;caras[0]._1=1;caras[0]._2=4;
  caras[1]._0=1;caras[1]._1=2;caras[1]._2=4;
  caras[2]._0=2;caras[2]._1=3;caras[2]._2=4;
  caras[3]._0=3;caras[3]._1=0;caras[3]._2=4;
  caras[4]._0=3;caras[4]._1=1;caras[4]._2=0;
  caras[5]._0=3;caras[5]._1=2;caras[5]._2=1;
}

//*************************************************************************
// clase doble Piramide
//*************************************************************************

_doblePiramide::_doblePiramide(float tam, float al) {

  //vertices 
  vertices.resize(6); 
  vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
  vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
  vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
  vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
  vertices[4].x=0;vertices[4].y=al;vertices[4].z=0;
  vertices[5].x=0;vertices[5].y=-al;vertices[5].z=0;

  caras.resize(8);
  caras[0]._0=0;caras[0]._1=1;caras[0]._2=4;
  caras[1]._0=1;caras[1]._1=2;caras[1]._2=4;
  caras[2]._0=2;caras[2]._1=3;caras[2]._2=4;
  caras[3]._0=3;caras[3]._1=0;caras[3]._2=4;
  caras[4]._0=1;caras[4]._1=2;caras[4]._2=5;
  caras[5]._0=0;caras[5]._1=1;caras[5]._2=5;
  caras[6]._0=3;caras[6]._1=0;caras[6]._2=5;
  caras[7]._0=2;caras[7]._1=3;caras[7]._2=5;
}

//*************************************************************************
// clase estrella
//*************************************************************************

_estrella::_estrella(float tam, float al) {

  //vertices 
  vertices.resize(14); 
  // sup e inf
  vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
  vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
  vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
  vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
  vertices[4].x=0;vertices[4].y=al;vertices[4].z=0;
  vertices[5].x=-tam;vertices[5].y=-2*tam;vertices[5].z=tam;
  vertices[6].x=tam;vertices[6].y=-2*tam;vertices[6].z=tam;
  vertices[7].x=tam;vertices[7].y=-2*tam;vertices[7].z=-tam;
  vertices[8].x=-tam;vertices[8].y=-2*tam;vertices[8].z=-tam;
  vertices[9].x=0;vertices[9].y=-2*tam-al;vertices[9].z=0;
  // laterales
  vertices[10].x=tam+al;vertices[10].y=-tam;vertices[10].z=0;
  vertices[11].x=-tam-al;vertices[11].y=-tam;vertices[11].z=0;
  // fondos
  vertices[12].x=0;vertices[12].y=-tam;vertices[12].z=tam+al;
  vertices[13].x=0;vertices[13].y=-tam;vertices[13].z=-tam-al;

  caras.resize(24);
  // sup e inf
  caras[0]._0=1;caras[0]._1=2;caras[0]._2=4;
  caras[1]._0=0;caras[1]._1=1;caras[1]._2=4;
  caras[2]._0=3;caras[2]._1=0;caras[2]._2=4;
  caras[3]._0=2;caras[3]._1=3;caras[3]._2=4;
  caras[4]._0=6;caras[4]._1=7;caras[4]._2=9;
  caras[5]._0=5;caras[5]._1=6;caras[5]._2=9;
  caras[6]._0=8;caras[6]._1=5;caras[6]._2=9;
  caras[7]._0=7;caras[7]._1=8;caras[7]._2=9;
  // laterales
  caras[8]._0=2;caras[8]._1=7;caras[8]._2=10;
  caras[9]._0=1;caras[9]._1=2;caras[9]._2=10;
  caras[10]._0=6;caras[10]._1=1;caras[10]._2=10;
  caras[11]._0=7;caras[11]._1=6;caras[11]._2=10;
  caras[12]._0=0;caras[12]._1=5;caras[12]._2=11;
  caras[13]._0=3;caras[13]._1=0;caras[13]._2=11;
  caras[14]._0=8;caras[14]._1=3;caras[14]._2=11;
  caras[15]._0=5;caras[15]._1=8;caras[15]._2=11;
  // fondos
  caras[16]._0=0;caras[16]._1=1;caras[16]._2=12;
  caras[17]._0=1;caras[17]._1=6;caras[17]._2=12;
  caras[18]._0=6;caras[18]._1=5;caras[18]._2=12;
  caras[19]._0=5;caras[19]._1=0;caras[19]._2=12;
  caras[20]._0=2;caras[20]._1=3;caras[20]._2=13;
  caras[21]._0=3;caras[21]._1=8;caras[21]._2=13;
  caras[22]._0=8;caras[22]._1=7;caras[22]._2=13;
  caras[23]._0=7;caras[23]._1=2;caras[23]._2=13;
}

_basePata::_basePata(float h)
{
	//vertices
	vertices.resize(8);
	vertices[0].x = 3;
	vertices[0].y = 0;
	vertices[0].z = 2.5;
	vertices[1].x = 3;
	vertices[1].y = 0;
	vertices[1].z = 0;
	vertices[2].x = -3;
	vertices[2].y = 0;
	vertices[2].z = 0;
	vertices[3].x = -3;
	vertices[3].y = 0;
	vertices[3].z = 2.5;
	vertices[4].x = 1.5;
	vertices[4].y = h;
	vertices[4].z = 2.5;
	vertices[5].x = 1.5;
	vertices[5].y = h;
	vertices[5].z = 0;
	vertices[6].x = -1.5;
	vertices[6].y = h;
	vertices[6].z = 0;
	vertices[7].x = -1.5;
	vertices[7].y = h;
	vertices[7].z = 2.5;

	// triangulos
	caras.resize(12);
	caras[0]._0 = 0;
	caras[0]._1 = 1;
	caras[0]._2 = 3;
	caras[1]._0 = 3;
	caras[1]._1 = 1;
	caras[1]._2 = 2;
	caras[2]._0 = 1;
	caras[2]._1 = 5;
	caras[2]._2 = 2;
	caras[3]._0 = 5;
	caras[3]._1 = 6;
	caras[3]._2 = 2;
	caras[4]._0 = 5;
	caras[4]._1 = 4;
	caras[4]._2 = 6;
	caras[5]._0 = 4;
	caras[5]._1 = 7;
	caras[5]._2 = 6;
	caras[6]._0 = 0;
	caras[6]._1 = 7;
	caras[6]._2 = 4;
	caras[7]._0 = 0;
	caras[7]._1 = 3;
	caras[7]._2 = 7;
	caras[8]._0 = 3;
	caras[8]._1 = 2;
	caras[8]._2 = 7;
	caras[9]._0 = 2;
	caras[9]._1 = 6;
	caras[9]._2 = 7;
	caras[10]._0 = 0;
	caras[10]._1 = 1;
	caras[10]._2 = 4;
	caras[11]._0 = 1;
	caras[11]._1 = 5;
	caras[11]._2 = 4;
}