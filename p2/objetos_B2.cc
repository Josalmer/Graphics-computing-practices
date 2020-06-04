//**************************************************************************
// Práctica 2 usando objetos
//**************************************************************************

#include "objetos_B2.h"
#include "file_ply_stl.h"
#include <algorithm>


//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D() {  }

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor) {
  int i;
  glPointSize(grosor);
  glColor3f(r,g,b);
  glBegin(GL_POINTS);
    for (i=0;i<vertices.size();i++){
      glVertex3fv((GLfloat *) &vertices[i]);
    }
  glEnd();
}


//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D() {  }


//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor) {
  int i;
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glLineWidth(grosor);
  glColor3f(r,g,b);
  glBegin(GL_TRIANGLES);
    for (i=0;i<caras.size();i++){
      glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
      glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
      glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
    }
  glEnd();
}

//*************************************************************************
// dibujar en modo sólido
//*************************************************************************

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
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor) {
  switch (modo){
    case POINTS:draw_puntos(r1, g1, b1, grosor); break;
    case EDGES:draw_aristas(r1, g1, b1, grosor); break;
    case SOLID_CHESS:draw_solido_ajedrez(r1, g1, b1, r2, g2, b2); break;
    case SOLID:draw_solido(r1, g1, b1); break;
  }
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
// clase objeto ply
//*************************************************************************


_objeto_ply::_objeto_ply() {
  // leer lista de coordenadas de vértices y lista de indices de vértices

}



int _objeto_ply::parametros(char *archivo) {
  int n_ver,n_car;

  vector<float> ver_ply ;
  vector<int>   car_ply ;

  _file_ply::read(archivo, ver_ply, car_ply );

  n_ver=ver_ply.size()/3;
  n_car=car_ply.size()/3;

  printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

  vertices.resize(n_ver);
  caras.resize(n_car);

  _vertex3f ver_aux;
  _vertex3i car_aux;

  for (int i = 0; i<n_ver; i++){
    ver_aux.x=ver_ply[i*3];
    ver_aux.y=ver_ply[i*3+1];
    ver_aux.z=ver_ply[i*3+2];
    vertices[i]=ver_aux;
  }

  for (int i = 0; i<n_car; i++) {
    car_aux._0=car_ply[i*3];
    car_aux._1=car_ply[i*3+1];
    car_aux._2=car_ply[i*3+2];
    caras[i]=car_aux;
  }

  return(0);
}

vector<_vertex3f> _objeto_ply::plyRevolution(char *archivo) {
  vector<float> imported_perfil;

  _file_ply::read_vertices(archivo,imported_perfil);
  vector<_vertex3f> vertices(imported_perfil.size()/3);

  for(unsigned int i=0; i<vertices.size(); i++) {
      vertices[i].x = imported_perfil[i*3];
      vertices[i].y = imported_perfil[i*3+1];
      vertices[i].z = imported_perfil[i*3+2];
  }

  return vertices;
}

//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion() {

}


void _rotacion::parametros(vector<_vertex3f> perfil, int num, string ejeGiro, string tapas) { // 2_tapas, tapa_inf, tapa_sup, no_tapas 


  int j, i; // Indices bucles

  int original_perfil_points = perfil.size();

  // Comprobamos en que orden vienen los puntos del perfil
  if (ejeGiro=="y"){
    if (perfil[0].y > perfil[original_perfil_points].y){
      reverse(perfil.begin(), perfil.end());
    }
  } else if (ejeGiro=="x"){
    if (perfil[0].x > perfil[original_perfil_points].x){
      reverse(perfil.begin(), perfil.end());
    }
  } else if (ejeGiro=="z"){
    if (perfil[0].z > perfil[original_perfil_points].z){
      reverse(perfil.begin(), perfil.end());
    }
  }
  
  bool p_inf = false;
  if (ejeGiro=="y"){
    if (perfil[0].x == 0.0) p_inf = true; // Ext inf en eje de rotación
  } else if (ejeGiro=="x"){
    if (perfil[0].y == 0.0) p_inf = true; // Ext inf en eje de rotación
  } else if (ejeGiro=="z"){
    if (perfil[0].x == 0.0) p_inf = true; // Ext inf en eje de rotación
  }
  bool p_sup = false;
  if (ejeGiro=="y"){
    if (perfil[original_perfil_points -1].x == 0.0) p_sup = true; // Ext sup en eje de rotación
  } else if (ejeGiro=="x"){
    if (perfil[original_perfil_points -1].y == 0.0) p_sup = true; // Ext sup en eje de rotación
  } else if (ejeGiro=="z"){
    if (perfil[original_perfil_points -1].x == 0.0) p_inf = true; // Ext inf en eje de rotación
  }
  
  int tam_aux = original_perfil_points;

  if (p_inf) tam_aux--;
  if (p_sup) tam_aux--;

  int n_points = tam_aux * num;     // Total vértices
  int n_points_rotated = n_points;  // Puntos en revolución

  // Vértices en eje de revolución adicionales
  if (p_inf) n_points++;
  if (p_sup) n_points++;

  int n_faces = 2 * num * (tam_aux - 1);

  // Caras adicionales de las tapas
  if (p_inf) n_faces = n_faces + num;
  if (p_sup) n_faces = n_faces + num;

  vertices.resize(n_points);
  caras.resize(n_faces);

  // Elementos auxiliares
  _vertex3f vertice_aux;
  _vertex3i cara_aux;

  // TRATAMIENTO DE LOS VÉRTICES
  // Rotación respecto del eje Y
  if (ejeGiro=="y"){
    // Tipo cilindro (sin extremos en eje)
    if (!p_inf && !p_sup) {
      for (j=0;j<num;j++) {
        for (i=0;i<original_perfil_points;i++) {
          vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num)) + perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
          vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num)) + perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
          vertice_aux.y=perfil[i].y;
          vertices[i+j*original_perfil_points]=vertice_aux;
        }
      }
    } else { // Tipo cono o esfera
      if (p_inf && p_sup) { // Ambos extremos en eje de rotación
        for (j=0; j<num; j++) {
          for (i=1; i < original_perfil_points - 1; i++) {
            vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num)) + perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
            vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num)) + perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
            vertice_aux.y=perfil[i].y;
            vertices[i + j * tam_aux - 1]=vertice_aux;
          }
          vertices[n_points - 2] = perfil[0];
          vertices[n_points - 1] = perfil[original_perfil_points - 1];
        }
      } else if (p_inf && !p_sup) { // Extremo inferior en eje de revolución
        for (j=0; j<num; j++) {
          for (i=1; i < original_perfil_points; i++) {
            vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num)) + perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
            vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num)) + perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
            vertice_aux.y=perfil[i].y;
            vertices[i + j * tam_aux - 1]=vertice_aux;
          }
          vertices[n_points - 1] = perfil[0];
        }
      } else if (!p_inf && p_sup) { // Extremo superior en eje de revolución
        for (j=0; j<num; j++) {
          for (i=0; i < original_perfil_points - 1; i++) {
            vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num)) + perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
            vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num)) + perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
            vertice_aux.y=perfil[i].y;
            vertices[i + j * tam_aux]=vertice_aux;
          }
          vertices[n_points - 1] = perfil[original_perfil_points - 1];
        }
      }
    } 
  } else if (ejeGiro=="x"){ // Rotación respecto del eje X
    // Tipo cilindro (sin extremos en eje)
    if (!p_inf && !p_sup) {
      for (j=0;j<num;j++) {
        for (i=0;i<original_perfil_points;i++) {
          vertice_aux.y=perfil[i].y*sin(2.0*M_PI*j/(1.0*num)) + perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
          vertice_aux.z=-perfil[i].y*cos(2.0*M_PI*j/(1.0*num)) + perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
          vertice_aux.x=perfil[i].x;
          vertices[i+j*original_perfil_points]=vertice_aux;
        }
      }
    } else { // Tipo cono o esfera
      if (p_inf && p_sup) { // Ambos extremos en eje de rotación
        for (j=0; j<num; j++) {
          for (i=1; i < original_perfil_points - 1; i++) {
            vertice_aux.y=perfil[i].y*sin(2.0*M_PI*j/(1.0*num)) + perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
            vertice_aux.z=-perfil[i].y*cos(2.0*M_PI*j/(1.0*num)) + perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
            vertice_aux.x=perfil[i].x;
            vertices[i + j * tam_aux - 1]=vertice_aux;
          }
          vertices[n_points - 2] = perfil[0];
          vertices[n_points - 1] = perfil[original_perfil_points - 1];
        }
      } else if (p_inf && !p_sup) { // Extremo inferior en eje de revolución
        for (j=0; j<num; j++) {
          for (i=1; i < original_perfil_points; i++) {
            vertice_aux.y=perfil[i].y*sin(2.0*M_PI*j/(1.0*num)) + perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
            vertice_aux.z=-perfil[i].y*cos(2.0*M_PI*j/(1.0*num)) + perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
            vertice_aux.x=perfil[i].x;
            vertices[i + j * tam_aux - 1]=vertice_aux;
          }
          vertices[n_points - 1] = perfil[0];
        }
      } else if (!p_inf && p_sup) { // Extremo superior en eje de revolución
        for (j=0; j<num; j++) {
          for (i=0; i < original_perfil_points - 1; i++) {
            vertice_aux.y=perfil[i].y*sin(2.0*M_PI*j/(1.0*num)) + perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
            vertice_aux.z=-perfil[i].y*cos(2.0*M_PI*j/(1.0*num)) + perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
            vertice_aux.x=perfil[i].x;
            vertices[i + j * tam_aux]=vertice_aux;
          }
          vertices[n_points - 1] = perfil[original_perfil_points - 1];
        }
      }
    }     
  } else if (ejeGiro=="z"){ // Rotación respecto del eje Z
    // Tipo cilindro (sin extremos en eje)
    if (!p_inf && !p_sup) {
      for (j=0;j<num;j++) {
        for (i=0;i<original_perfil_points;i++) {
          vertice_aux.y=perfil[i].x*sin(2.0*M_PI*j/(1.0*num)) + perfil[i].y*cos(2.0*M_PI*j/(1.0*num));
          vertice_aux.x=-perfil[i].x*cos(2.0*M_PI*j/(1.0*num)) + perfil[i].y*sin(2.0*M_PI*j/(1.0*num));
          vertice_aux.z=perfil[i].z;
          vertices[i+j*original_perfil_points]=vertice_aux;
        }
      }
    } else { // Tipo cono o esfera
      if (p_inf && p_sup) { // Ambos extremos en eje de rotación
        for (j=0; j<num; j++) {
          for (i=1; i < original_perfil_points - 1; i++) {
            vertice_aux.y=perfil[i].x*sin(2.0*M_PI*j/(1.0*num)) + perfil[i].y*cos(2.0*M_PI*j/(1.0*num));
            vertice_aux.x=-perfil[i].x*cos(2.0*M_PI*j/(1.0*num)) + perfil[i].y*sin(2.0*M_PI*j/(1.0*num));
            vertice_aux.z=perfil[i].z;
            vertices[i + j * tam_aux - 1]=vertice_aux;
          }
          vertices[n_points - 2] = perfil[0];
          vertices[n_points - 1] = perfil[original_perfil_points - 1];
        }
      } else if (p_inf && !p_sup) { // Extremo inferior en eje de revolución
        for (j=0; j<num; j++) {
          for (i=1; i < original_perfil_points; i++) {
            vertice_aux.y=perfil[i].x*sin(2.0*M_PI*j/(1.0*num)) + perfil[i].y*cos(2.0*M_PI*j/(1.0*num));
            vertice_aux.x=-perfil[i].x*cos(2.0*M_PI*j/(1.0*num)) + perfil[i].y*sin(2.0*M_PI*j/(1.0*num));
            vertice_aux.z=perfil[i].z;
            vertices[i + j * tam_aux - 1]=vertice_aux;
          }
          vertices[n_points - 1] = perfil[0];
        }
      } else if (!p_inf && p_sup) { // Extremo superior en eje de revolución
        for (j=0; j<num; j++) {
          for (i=0; i < original_perfil_points - 1; i++) {
            vertice_aux.y=perfil[i].x*sin(2.0*M_PI*j/(1.0*num)) + perfil[i].y*cos(2.0*M_PI*j/(1.0*num));
            vertice_aux.x=-perfil[i].x*cos(2.0*M_PI*j/(1.0*num)) + perfil[i].y*sin(2.0*M_PI*j/(1.0*num));
            vertice_aux.z=perfil[i].z;
            vertices[i + j * tam_aux]=vertice_aux;
          }
          vertices[n_points - 1] = perfil[original_perfil_points - 1];
        }
      }
    }     
  }

  // TRATAMIENTO DE LAS CARAS
  int pos;
  int face_position = 0;
  
  // Tipo cilindro (sin extremos en eje)
  if (!p_inf && !p_sup) {
    for (int i = 0; i<(tam_aux-1); i++) {
      for (int j = 0; j<num; j++) {
        pos = tam_aux*j+i;
        cara_aux._0 = (pos+1)%n_points_rotated;
        cara_aux._1 = (pos+tam_aux)%n_points_rotated;
        cara_aux._2 = (pos)%n_points_rotated;
        caras[face_position++]=cara_aux;    
        cara_aux._0 = (pos+1)%n_points_rotated;
        cara_aux._1 = (pos+tam_aux+1)%n_points_rotated;
        cara_aux._2 = (pos+tam_aux)%n_points_rotated;
        caras[face_position++]=cara_aux;
      }
    }
  } else {
    // Tipo cono o esfera
    if (p_inf && p_sup) { // Ambos extremos en eje de rotación
      pos = n_points - 2;
      for(i=0; i< num; i++) { // Triangulos inferiores
        cara_aux._0 = ((i + 1) * tam_aux)%n_points_rotated;
        cara_aux._1 = (i * tam_aux)%n_points_rotated;
        cara_aux._2 = pos;
        caras[face_position++]=cara_aux;
      }
      for (int i = 0; i<tam_aux - 1; i++) {
        for (int j = 0; j<num; j++) {
          pos = tam_aux*j+i;
          cara_aux._0 = (pos+1)%n_points_rotated;
          cara_aux._1 = (pos+tam_aux)%n_points_rotated;
          cara_aux._2 = (pos)%n_points_rotated;
          caras[face_position++]=cara_aux;    
          cara_aux._0 = (pos+1)%n_points_rotated;
          cara_aux._1 = (pos+tam_aux+1)%n_points_rotated;
          cara_aux._2 = (pos+tam_aux)%n_points_rotated;
          caras[face_position++]=cara_aux;
        }
      }
      pos = n_points - 1;
      for(i=0; i< num; i++) { // Triangulos superiores
        cara_aux._0 = pos;
        cara_aux._1 = (pos + tam_aux + (i) * tam_aux -2)%n_points_rotated;
        cara_aux._2 = (pos + tam_aux + (i + 1) * tam_aux-2)%n_points_rotated;
        caras[face_position++]=cara_aux;
      }
    } else if (p_inf && !p_sup) { // Extremo inferior en eje de revolución
      pos = n_points - 1;
      for(i=0; i< num; i++) { // Triangulos inferiores
        cara_aux._0 = ((i + 1) * tam_aux)%n_points_rotated;
        cara_aux._1 = (i * tam_aux)%n_points_rotated;
        cara_aux._2 = pos;
        caras[face_position++]=cara_aux;
      }
      for (int i = 0; i<tam_aux - 1; i++) {
        for (int j = 0; j<num; j++) {
          pos = tam_aux*j+i;
          cara_aux._0 = (pos+1)%n_points_rotated;
          cara_aux._1 = (pos+tam_aux)%n_points_rotated;
          cara_aux._2 = (pos)%n_points_rotated;
          caras[face_position++]=cara_aux;    
          cara_aux._0 = (pos+1)%n_points_rotated;
          cara_aux._1 = (pos+tam_aux+1)%n_points_rotated;
          cara_aux._2 = (pos+tam_aux)%n_points_rotated;
          caras[face_position++]=cara_aux;
        }
      }
    } else if (!p_inf && p_sup) { // Extremo superior en eje de revolución
      for (int i = 0; i<tam_aux - 1; i++) {
        for (int j = 0; j<num; j++) {
          pos = tam_aux*j+i;
          cara_aux._0 = (pos+1)%n_points_rotated;
          cara_aux._1 = (pos+tam_aux)%n_points_rotated;
          cara_aux._2 = (pos)%n_points_rotated;
          caras[face_position++]=cara_aux;    
          cara_aux._0 = (pos+1)%n_points_rotated;
          cara_aux._1 = (pos+tam_aux+1)%n_points_rotated;
          cara_aux._2 = (pos+tam_aux)%n_points_rotated;
          caras[face_position++]=cara_aux;
        }
      }
      pos = n_points - 1;
      for(i=0; i< num; i++) { // Triangulos superiores
        cara_aux._0 = pos;
        cara_aux._1 = (pos + tam_aux + (i) * tam_aux -2)%n_points_rotated;
        cara_aux._2 = (pos + tam_aux + (i + 1) * tam_aux-2)%n_points_rotated;
        caras[face_position++]=cara_aux;
      }
    }
  }

  // TRATAMIENTO DE LAS TAPAS
  // Rotación respecto del eje Y
  if (ejeGiro=="y"){
    // Tapa inferior
    if(!p_inf && (tapas == "2_tapas" || tapas == "tapa_inf")) { // Pintar tapa inf
      n_faces = n_faces + num;
      n_points++;
      vertice_aux.x = 0.0;
      vertice_aux.y = perfil[0].y;
      vertice_aux.z = perfil[0].z;
      caras.resize(n_faces);
      vertices.resize(n_points);
      vertices[(n_points - 1)]=vertice_aux;
      for(i=0; i<num; i++) {
          pos = i*tam_aux;
          cara_aux._0 = n_points-1;
          cara_aux._1 = (pos+tam_aux)%n_points_rotated;
          cara_aux._2 = (pos)%n_points_rotated;
          caras[face_position++]=cara_aux;
      }
    }
    // Tapa superior
    if(!p_sup && (tapas == "2_tapas" || tapas == "tapa_inf")) { // Pintar tapa sup
      n_faces = n_faces + num;
      n_points++;
      vertice_aux.x = 0.0;
      vertice_aux.y = perfil[original_perfil_points - 1].y;
      vertice_aux.z = perfil[original_perfil_points - 1].z;
      caras.resize(n_faces);
      vertices.resize(n_points);
      vertices[(n_points - 1)]=vertice_aux;
      for(i=0; i<num; i++) {
          pos = i*tam_aux + tam_aux-1;
          cara_aux._0 = n_points-1;
          cara_aux._1 = (pos)%n_points_rotated;
          cara_aux._2 = (pos+tam_aux)%n_points_rotated;
          caras[face_position++]=cara_aux;
      }
    }
  } else if (ejeGiro=="x"){ // Rotación respecto del eje X
    // Tapa inferior
    if(!p_inf && (tapas == "2_tapas" || tapas == "tapa_inf")) { // Pintar tapa inf
      n_faces = n_faces + num;
      n_points++;
      vertice_aux.y = 0.0;
      vertice_aux.x = perfil[0].x;
      vertice_aux.z = perfil[0].z;
      caras.resize(n_faces);
      vertices.resize(n_points);
      vertices[(n_points - 1)]=vertice_aux;
      for(i=0; i<num; i++) {
          pos = i*tam_aux;
          cara_aux._0 = n_points-1;
          cara_aux._1 = (pos+tam_aux)%n_points_rotated;
          cara_aux._2 = (pos)%n_points_rotated;
          caras[face_position++]=cara_aux;
      }
    }
    // Tapa superior
    if(!p_sup && (tapas == "2_tapas" || tapas == "tapa_inf")) { // Pintar tapa sup
      n_faces = n_faces + num;
      n_points++;
      vertice_aux.y = 0.0;
      vertice_aux.x = perfil[original_perfil_points - 1].x;
      vertice_aux.z = perfil[original_perfil_points - 1].z;
      caras.resize(n_faces);
      vertices.resize(n_points);
      vertices[(n_points - 1)]=vertice_aux;
      for(i=0; i<num; i++) {
          pos = i*tam_aux + tam_aux-1;
          cara_aux._0 = n_points-1;
          cara_aux._1 = (pos)%n_points_rotated;
          cara_aux._2 = (pos+tam_aux)%n_points_rotated;
          caras[face_position++]=cara_aux;
      }
    }
  } else if (ejeGiro=="z"){ // Rotación respecto del eje Z
    // Tapa inferior
    if(!p_inf && (tapas == "2_tapas" || tapas == "tapa_inf")) { // Pintar tapa inf
      n_faces = n_faces + num;
      n_points++;
      vertice_aux.x = 0.0;
      vertice_aux.z = perfil[0].z;
      vertice_aux.y = perfil[0].y;
      caras.resize(n_faces);
      vertices.resize(n_points);
      vertices[(n_points - 1)]=vertice_aux;
      for(i=0; i<num; i++) {
          pos = i*tam_aux;
          cara_aux._0 = n_points-1;
          cara_aux._1 = (pos+tam_aux)%n_points_rotated;
          cara_aux._2 = (pos)%n_points_rotated;
          caras[face_position++]=cara_aux;
      }
    }
    // Tapa superior
    if(!p_sup && (tapas == "2_tapas" || tapas == "tapa_inf")) { // Pintar tapa sup
      n_faces = n_faces + num;
      n_points++;
      vertice_aux.x = 0.0;
      vertice_aux.z = perfil[original_perfil_points - 1].z;
      vertice_aux.y = perfil[original_perfil_points - 1].y;
      caras.resize(n_faces);
      vertices.resize(n_points);
      vertices[(n_points - 1)]=vertice_aux;
      for(i=0; i<num; i++) {
          pos = i*tam_aux + tam_aux-1;
          cara_aux._0 = n_points-1;
          cara_aux._1 = (pos)%n_points_rotated;
          cara_aux._2 = (pos+tam_aux)%n_points_rotated;
          caras[face_position++]=cara_aux;
      }
    }
  }
}
