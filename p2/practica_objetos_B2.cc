//**************************************************************************
// Práctica 2 usando objetos
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_B2.h"


using namespace std;

// tipos
typedef enum{OBJETO_PLY_REVOLUTION, OBJETO_PLY, ESFERA, REVOL_X, REVOL_Y, REVOL_Z, CONO, CILINDRO} _tipo_objeto;
_tipo_objeto t_objeto=OBJETO_PLY_REVOLUTION;
_modo modo=POINTS;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x,Size_y,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x=50,Window_y=50,Window_width=450,Window_high=450;

// objetos
_objeto_ply  ply; 
_objeto_ply ply_rotate;
_rotacion  plyRevolution; 
_rotacion esfera;
_rotacion revolX;
_rotacion revolY;
_rotacion revolZ;
_rotacion cono;
_rotacion cilindro;

// _objeto_ply *ply1;


//**************************************************************************
//
//***************************************************************************

void clean_window() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
  //  plano_delantero>0  plano_trasero>PlanoDelantero)
  glFrustum(-Size_x,Size_x,-Size_y,Size_y,Front_plane,Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer() {
  // posicion del observador
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0,0,-Observer_distance);
  glRotatef(Observer_angle_x,1,0,0);
  glRotatef(Observer_angle_y,0,1,0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis() {
  glDisable(GL_LIGHTING);
  glLineWidth(2);
  glBegin(GL_LINES);
    // eje X, color rojo
    glColor3f(1,0,0);
    glVertex3f(-AXIS_SIZE,0,0);
    glVertex3f(AXIS_SIZE,0,0);
    // eje Y, color verde
    glColor3f(0,1,0);
    glVertex3f(0,-AXIS_SIZE,0);
    glVertex3f(0,AXIS_SIZE,0);
    // eje Z, color azul
    glColor3f(0,0,1);
    glVertex3f(0,0,-AXIS_SIZE);
    glVertex3f(0,0,AXIS_SIZE);
  glEnd();
}


//**************************************************************************
// Funcion que dibuja los objetos
//****************************2***********************************************

void draw_objects() {
  switch (t_objeto){
    case OBJETO_PLY_REVOLUTION: plyRevolution.draw(modo,1.0,0.6,0.0,0.0,1.0,0.3,2);break;
    case OBJETO_PLY: ply.draw(modo,1.0,0.6,0.0,0.0,1.0,0.3,2);break;
    case ESFERA: esfera.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,2);break;
    case REVOL_X: revolX.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,2);break;
    case REVOL_Y: revolY.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,2);break;
    case REVOL_Z: revolZ.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,2);break;
    case CONO: cono.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,2);break;
    case CILINDRO: cilindro.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,2);break;
  }
}


//**************************************************************************
//
//***************************************************************************

void draw(void) {
  clean_window();
  change_observer();
  draw_axis();
  draw_objects();
  glutSwapBuffers();
}



//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1,int Alto1) {
  float Aspect_ratio;

  Aspect_ratio=(float) Alto1/(float )Ancho1;
  Size_y=Size_x*Aspect_ratio;
  change_projection();
  glViewport(0,0,Ancho1,Alto1);
  glutPostRedisplay();
}


//**********-o*****************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_key(unsigned char Tecla1,int x,int y) {
  switch (toupper(Tecla1)) {
    case 'Q':exit(0);
    case 'T':
    case 'M':
      if (modo==POINTS){
        modo=EDGES;
      } else if (modo==EDGES){
        modo=SOLID;
      } else if (modo==SOLID){
        modo=SOLID_CHESS;
      } else if (modo==SOLID_CHESS){
        modo=POINTS;
      }
    break;
    case 'O':
      if (t_objeto==OBJETO_PLY_REVOLUTION) {
        t_objeto=OBJETO_PLY;
      } else if (t_objeto==OBJETO_PLY){
        t_objeto=ESFERA;
      } else if (t_objeto==ESFERA){
        t_objeto=REVOL_X;
      } else if (t_objeto==REVOL_X){
        t_objeto=REVOL_Y;
      } else if (t_objeto==REVOL_Y){
        t_objeto=REVOL_Z;
      } else if (t_objeto==REVOL_Z){
        t_objeto=CONO;
      } else if (t_objeto==CONO){
        t_objeto=CILINDRO;
      } else if (t_objeto==CILINDRO){
        t_objeto=OBJETO_PLY_REVOLUTION;
      }
    break;
  }
  glutPostRedisplay();
}

//***************************************************************************
// Funcion l-olamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_key(int Tecla1,int x,int y) {
  switch (Tecla1) {
    case GLUT_KEY_LEFT:Observer_angle_y--;break;
    case GLUT_KEY_RIGHT:Observer_angle_y++;break;
    case GLUT_KEY_UP:Observer_angle_x--;break;
    case GLUT_KEY_DOWN:Observer_angle_x++;break;
    case GLUT_KEY_PAGE_UP:Observer_distance*=1.2;break;
    case GLUT_KEY_PAGE_DOWN:Observer_distance/=1.2;break;
  }
  glutPostRedisplay();
}



//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void) {

  // se inicalizan la ventana y los planos de corte
  Size_x=0.5;
  Size_y=0.5;
  Front_plane=1;
  Back_plane=1000;

  // se incia la posicion del observador, en el eje z
  Observer_distance=4*Front_plane;
  Observer_angle_x=0;
  Observer_angle_y=0;

  // se indica cua*ply1l sera el color para limpiar la ventana	(r,v,a,al)
  // blanco=(1,1,1,1) rojo=(1,0,0,1), ...
  glClearColor(1,1,1,1);

  // se habilita el z-bufer
  glEnable(GL_DEPTH_TEST);
  change_projection();
  glViewport(0,0,Window_width,Window_high);

}


//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************


int main(int argc, char *argv[]) {

  // perfil 

  vector<_vertex3f> perf_cilindro;
  vector<_vertex3f> perf_jarron_y;
  vector<_vertex3f> perf_jarron_x;
  vector<_vertex3f> perf_jarron_z;
  vector<_vertex3f> perfil_esfera;
  vector<_vertex3f> perfil_cono_inf;
  vector<_vertex3f> perfil_cono_sup;
  _vertex3f aux;

  aux.x=1.0; aux.y=-3.0; aux.z=0.0;
  perf_cilindro.push_back(aux);
  aux.x=1.0; aux.y=3.0; aux.z=0.0;
  perf_cilindro.push_back(aux);

  aux.x=0.0; aux.y=0.0; aux.z=0.0;
  perfil_cono_inf.push_back(aux);
  aux.x=1.0; aux.y=3.0; aux.z=0.0;
  perfil_cono_inf.push_back(aux);

  aux.x=1.8; aux.y=-3.0; aux.z=0.0;
  perfil_cono_sup.push_back(aux);
  aux.x=0.0; aux.y=3.0; aux.z=0.0;
  perfil_cono_sup.push_back(aux);

  // Esfera
  aux.x = 0.0;                    aux.y = -1.0;                   aux.z = 0.0;
  perfil_esfera.push_back(aux);
  aux.x = sin(M_PI / 8);          aux.y = -cos(M_PI / 8);         aux.z = 0.0;
  perfil_esfera.push_back(aux);
  aux.x = sin(M_PI / 4);          aux.y = -cos(M_PI / 4);         aux.z = 0.0;
  perfil_esfera.push_back(aux);
  aux.x = sin((M_PI / 8) * 3);    aux.y = -cos((M_PI / 8) * 3);   aux.z = 0.0;
  perfil_esfera.push_back(aux);
  aux.x = 1.0;                    aux.y = 0.0;                    aux.z = 0.0;
  perfil_esfera.push_back(aux);
  aux.x = sin((M_PI / 8) * 3);    aux.y = cos((M_PI / 8) * 3);    aux.z = 0.0;
  perfil_esfera.push_back(aux);
  aux.x = sin(M_PI / 4);          aux.y = cos(M_PI / 4);          aux.z = 0.0;
  perfil_esfera.push_back(aux);
  aux.x = sin(M_PI / 8);          aux.y = cos(M_PI / 8);          aux.z = 0.0;
  perfil_esfera.push_back(aux);
  aux.x = 0.0;                    aux.y = 1.0;                    aux.z = 0.0;
  perfil_esfera.push_back(aux);

  // Jarron eje y
  aux.x=1.0; aux.y=4.0; aux.z=0.0;
  perf_jarron_y.push_back(aux);
  aux.x=2.0; aux.y=2.0; aux.z=0.0;
  perf_jarron_y.push_back(aux);  
  aux.x=1.0; aux.y=0.0; aux.z=0.0;
  perf_jarron_y.push_back(aux); 
  aux.x=2.0; aux.y=-2.0; aux.z=0.0;
  perf_jarron_y.push_back(aux);  
  aux.x=1.0; aux.y=-4.0; aux.z=0.0;
  perf_jarron_y.push_back(aux);  

  // Jarron eje x
  aux.x=4.0; aux.y=1.0; aux.z=0.0;
  perf_jarron_x.push_back(aux);
  aux.x=2.0; aux.y=2.0; aux.z=0.0;
  perf_jarron_x.push_back(aux);  
  aux.x=0.0; aux.y=1.0; aux.z=0.0;
  perf_jarron_x.push_back(aux); 
  aux.x=-2.0; aux.y=2.0; aux.z=0.0;
  perf_jarron_x.push_back(aux);  
  aux.x=-4.0; aux.y=1.0; aux.z=0.0;
  perf_jarron_x.push_back(aux);  

  // Jarron eje z
  aux.x=1.0; aux.y=0.0; aux.z=4.0;
  perf_jarron_z.push_back(aux);
  aux.x=2.0; aux.y=0.0; aux.z=2.0;
  perf_jarron_z.push_back(aux);  
  aux.x=1.0; aux.y=0.0; aux.z=0.0;
  perf_jarron_z.push_back(aux); 
  aux.x=2.0; aux.y=0.0; aux.z=-2.0;
  perf_jarron_z.push_back(aux);  
  aux.x=1.0; aux.y=0.0; aux.z=-4.0;
  perf_jarron_z.push_back(aux);  

  esfera.parametros(perfil_esfera,20);
  revolX.parametros(perf_jarron_x, 20, "x");
  revolY.parametros(perf_jarron_y, 20);
  revolZ.parametros(perf_jarron_z, 20, "z");
  cono.parametros(perfil_cono_sup,20);
  cilindro.parametros(perf_cilindro,20);

  // se llama a la inicialización de glut
  glutInit(&argc, argv);

  // se indica las caracteristicas que se desean para la visualización con OpenGL
  // Las posibilidades son:
  // GLUT_SIMPLE -> memoria de imagen simple
  // GLUT_DOUBLE -> memoria de imagen doble
  // GLUT_INDEX -> memoria de imagen con color indizado
  // GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
  // GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
  // GLUT_DEPTH -> memoria de profundidad o z-bufer
  // GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  // posicion de la esquina inferior izquierdad de la ventana
  glutInitWindowPosition(Window_x,Window_y);

  // tamaño de la ventana (ancho y alto)
  glutInitWindowSize(Window_width,Window_high);

  // llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
  // al bucle de eventos)
  glutCreateWindow("PRACTICA - 2");

  // asignación de la funcion llamada "dibujar" al evento de dibujo
  glutDisplayFunc(draw);
  // asignación de la funcion llamada "change_window_size" al evento correspondiente
  glutReshapeFunc(change_window_size);
  // asignación de la funcion llamada "normal_key" al evento correspondiente
  glutKeyboardFunc(normal_key);
  // asignación de la funcion llamada "tecla_Especial" al evento correspondiente
  glutSpecialFunc(special_key);

  // funcion de inicialización
  initialize();

  // creación del objeto ply
  ply.parametros(argv[1]);

  // creación del objeto ply de revolución
  vector<_vertex3f> perfil_from_ply;
  perfil_from_ply = ply_rotate.plyRevolution(argv[2]);
  plyRevolution.parametros(perfil_from_ply,20);

  //ply1 = new _objeto_ply(argv[1]);

  // inicio del bucle de eventos
  glutMainLoop();
  return 0;
}
