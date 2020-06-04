/*
Practica 3
*/

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_B3.h"

using namespace std;

// tipos
typedef enum {
  ARTICULADO_ROBOT
} _tipo_objeto;

_tipo_objeto t_objeto = ARTICULADO_ROBOT;
_modo modo = EDGES;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x, Size_y, Front_plane, Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x = 50, Window_y = 50, Window_width = 1150, Window_high = 1150;

// objetos
_robot robot;

// Variables para controlar la animación
bool antena_subiendo = true;
bool inclinandose_del = true;
bool avanzando = true;
float velocidad_animacion = 1.0;

// Variables para elegir que movimientos automaticos se quieren
bool rotar_cabeza = true;
bool rotar_cuerpo = true;
bool mover_atena = true;
bool desplazamiento = true;
bool girando = true;

//**************************************************************************
//
//***************************************************************************

void clean_window() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
  //  plano_delantero>0  plano_trasero>PlanoDelantero)
  glFrustum(-Size_x, Size_x, -Size_y, Size_y, Front_plane, Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer() {
  // posicion del observador
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, -10, -Observer_distance);
  glRotatef(Observer_angle_x, 1, 0, 0);
  glRotatef(Observer_angle_y, 0, 1, 0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis() {
  glDisable(GL_LIGHTING);
  glLineWidth(2);
  glBegin(GL_LINES);
    // eje X, color rojo
    glColor3f(1, 0, 0);
    glVertex3f(-AXIS_SIZE, 0, 0);
    glVertex3f(AXIS_SIZE, 0, 0);
    // eje Y, color verde
    glColor3f(0, 1, 0);
    glVertex3f(0, -AXIS_SIZE, 0);
    glVertex3f(0, AXIS_SIZE, 0);
    // eje Z, color azul
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, -AXIS_SIZE);
    glVertex3f(0, 0, AXIS_SIZE);
  glEnd();
}

//**************************************************************************
// Funcion que dibuja los objetos
//****************************2***********************************************

void draw_objects() {
  robot.draw(modo, 0.05, 0.05, 0.9, 0.8, 0.8, 0.8, 2);
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

void change_window_size(int Ancho1, int Alto1) {
  float Aspect_ratio;
  Aspect_ratio = (float)Alto1 / (float)Ancho1;
  Size_y = Size_x * Aspect_ratio;
  change_projection();
  glViewport(0, 0, Ancho1, Alto1);
  glutPostRedisplay();
}

//**************************************************************************
// Funcion que anima los objetos
//**************************************************************************

void idle() {
  // Movimiento adelante - atras
  if(robot.avance >= robot.avance_max) {
    avanzando = false;
  } else if(robot.avance <= robot.avance_min) {
    avanzando = true;
  }
  // Inclinación del cuerpo
  if(robot.inclinacion_cuerpo >= robot.inclinacion_cuerpo_max) {
    inclinandose_del = false;
  } else if(robot.inclinacion_cuerpo <= robot.inclinacion_cuerpo_min) {
    inclinandose_del = true;
  }
  // Antena
  if(robot.cabeza.altura_antena >= robot.cabeza.altura_antena_max) {
    antena_subiendo = false;
  } else if(robot.cabeza.altura_antena <= robot.cabeza.altura_antena_min) {
    antena_subiendo = true;
  }
  // Realizamos movimientos
  if (rotar_cabeza){
    robot.giro_cabeza += 2 * velocidad_animacion;
  }
  if (girando) {
    robot.giro_completo += 0.5 * velocidad_animacion;
  }
  if (desplazamiento) {
    if(avanzando) {
      robot.avance += 0.2 * velocidad_animacion;
    } else {
      robot.avance -= 0.2 * velocidad_animacion;
    }
  }
  if (rotar_cuerpo) {
    if(inclinandose_del) {
      robot.inclinacion_cuerpo += 2 * velocidad_animacion;
    } else {
      robot.inclinacion_cuerpo -= 2 * velocidad_animacion;
    }
  }
  if (mover_atena){
    if(antena_subiendo) {
      robot.cabeza.altura_antena += 0.15 * velocidad_animacion;
    } else {
      robot.cabeza.altura_antena -= 0.15 * velocidad_animacion;
    }
  }

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

void normal_key(unsigned char Tecla1, int x, int y) {
  switch (toupper(Tecla1)) {
    case 'Q':
      exit(0);
    case 'T':
    case 'M':
      if (modo == POINTS) {
        modo = EDGES;
      } else if (modo == EDGES) {
        modo = SOLID;
      } else if (modo == SOLID) {
        modo = SOLID_CHESS;
      } else if (modo == SOLID_CHESS) {
        modo = POINTS;
      }
      break;
    case 'A':
      glutIdleFunc(idle);
      break;
    case 'S':
      glutIdleFunc(NULL);
      break;
    case '+':
      velocidad_animacion *= 1.2;
      if (velocidad_animacion >= 10) {
        velocidad_animacion = 10;
      }
      break;
    case '-':
      velocidad_animacion /= 1.2;
      if (velocidad_animacion <= 0.1) {
        velocidad_animacion = 0.1;
      }
      break;
    case '1':
      girando = !girando;
      break;
    case '2':
      desplazamiento = !desplazamiento;
      break;
    case '3':
      rotar_cuerpo = !rotar_cuerpo;
      break;
    case '4':
      rotar_cabeza = !rotar_cabeza;
      break;
    case '5':
      mover_atena = !mover_atena;
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

void special_key(int Tecla1, int x, int y) {

  switch (Tecla1) {
    case GLUT_KEY_LEFT:
      Observer_angle_y--;
      break;
    case GLUT_KEY_RIGHT:
      Observer_angle_y++;
      break;
    case GLUT_KEY_UP:
      Observer_angle_x--;
      break;
    case GLUT_KEY_DOWN:
      Observer_angle_x++;
      break;
    case GLUT_KEY_PAGE_UP:
      Observer_distance *= 1.2;
      break;
    case GLUT_KEY_PAGE_DOWN:
      Observer_distance /= 1.2;
      break;
    case GLUT_KEY_F1:
      robot.giro_cabeza += 2;
      break;
    case GLUT_KEY_F2:
      robot.giro_cabeza -= 2;
      break;
    case GLUT_KEY_F3:
      robot.avance += 0.2;
      break;
    case GLUT_KEY_F4:
      robot.avance -= 0.2;
      break;
    case GLUT_KEY_F5:
      robot.giro_completo += 2;
      break;
    case GLUT_KEY_F6:
      robot.giro_completo -= 2;
      break;
    case GLUT_KEY_F7:
      robot.cabeza.altura_antena += 0.15;
      if (robot.cabeza.altura_antena > robot.cabeza.altura_antena_max) {
        robot.cabeza.altura_antena = robot.cabeza.altura_antena_max;
      }
      break;
    case GLUT_KEY_F8:
      robot.cabeza.altura_antena -= 0.15;
      if (robot.cabeza.altura_antena < robot.cabeza.altura_antena_min) {
        robot.cabeza.altura_antena = robot.cabeza.altura_antena_min;
      }
      break;
    case GLUT_KEY_F9:
      robot.inclinacion_cuerpo += 2;
      if (robot.inclinacion_cuerpo > robot.inclinacion_cuerpo_max) {
        robot.inclinacion_cuerpo = robot.inclinacion_cuerpo_max;
      }
      break;
    case GLUT_KEY_F10:
      robot.inclinacion_cuerpo -= 2;
      if (robot.inclinacion_cuerpo < robot.inclinacion_cuerpo_min) {
        robot.inclinacion_cuerpo = robot.inclinacion_cuerpo_min;
      }
      break;
  }

  glutPostRedisplay();
}

//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void) {
  // se inicalizan la ventana y los planos de corte
  Size_x = 0.5;
  Size_y = 0.5;
  Front_plane = 1;
  Back_plane = 1000;

  // se incia la posicion del observador, en el eje z
  Observer_distance = 40 * Front_plane;
  Observer_angle_x = 0;
  Observer_angle_y = 0;

  // se indica cua*ply1l sera el color para limpiar la ventana	(r,v,a,al)
  // blanco=(1,1,1,1) rojo=(1,0,0,1), ...
  glClearColor(1, 1, 1, 1);

  // se habilita el z-bufer
  glEnable(GL_DEPTH_TEST);
  change_projection();
  glViewport(0, 0, Window_width, Window_high);
}

//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************

int main(int argc, char **argv) {

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
  glutInitWindowPosition(Window_x, Window_y);

  // tamaño de la ventana (ancho y alto)
  glutInitWindowSize(Window_width, Window_high);

  // llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
  // al bucle de eventos)
  glutCreateWindow("PRACTICA - 3");

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

  // inicio del bucle de eventos
  glutMainLoop();
  return 0;
}
