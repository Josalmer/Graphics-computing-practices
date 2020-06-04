/*
Jose SM
Practica 3
*/

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_B5.h"

using namespace std;

// tipos
typedef enum
{
  CUBO,
  PIRAMIDE,
  OBJETO_PLY,
  ROTACION,
  ARTICULADO_ROBOT,
  ESFERA
} _tipo_objeto;

_tipo_objeto t_objeto = ARTICULADO_ROBOT;
_modo modo = SOLID;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x, Size_y, Front_plane, Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x = 50, Window_y = 50, Window_width = 1150, Window_high = 1150;

// objetos
_cubo cubo;
_piramide piramide(0.85, 1.3);
_objeto_ply ply;
_rotacionJose rotacion;
_rotacionJose esfera;
_robot robot;

// Variables para controlar la animación
bool animacionRobot = false;
bool antena_subiendo = true;
bool inclinandose_del = true;
bool avanzando = true;
float velocidad_animacion = 0.2;

// Variables para elegir que movimientos automaticos se quieren
bool rotar_cabeza = true;
bool rotar_cuerpo = true;
bool mover_atena = true;
bool desplazamiento = true;
bool girando = true;

// Variable para movimiento de luces
int giroLuz = 0;
bool luzGirando = true;

// Materiales
string materialSeleccionado = "ruby";
// Jade
_vertex4f jade_difusa = {0.135, 0.22, 0.1575, 0.95};
_vertex4f jade_ambiente = {0.54, 0.89, 0.63, 0.95};
_vertex4f jade_especular = {0.32, 0.32, 0.32, 0.95};
float jade_brillo = 12.8;
// Ruby
_vertex4f ruby_ambiente = {0.1745, 0.01175, 0.01175, 0.55};
_vertex4f ruby_difusa = {0.61424, 0.04136, 0.04136, 0.55};
_vertex4f ruby_especular = {0.727811, 0.626959, 0.626959, 0.55};
float ruby_brillo = 76.8;
// Emerald
_vertex4f emerald_ambiente = {0.0215, 0.1745, 0.0215, 0.55};
_vertex4f emerald_difusa = {0.07568, 0.6142, 0.0756, 0.55};
_vertex4f emerald_especular = {0.633, 0.7278, 0.633, 0.55};
float emerald_brillo = 76.8;

//**************************************************************************
// Práctica 5
//***************************************************************************

int estadoRaton[3], xc, yc;
float factor = 1.0;
string vista = "perspectiva";
bool cambioColor[4] = {false, false, false, false};
int trianguloSelected = -1;

//***************************************************************************
// Funciones para seleccion por color
//***************************************************************************

void procesar_color(unsigned char color [3]) {
  if (color[0] >= 80 && color [0]<150) {// Soporte izquierdo
    trianguloSelected = -1;
    if (!cambioColor[0]) {
      cambioColor[0] = true;
      cambioColor[1] = false;
      cambioColor[2] = false;
      cambioColor[3] = false;
    } else {
      cambioColor[0] = false;
    }
  } else if (color[0] >= 0 && color[0]<80) {// Soporte derecho
    if (color[0] < 12) {
      trianguloSelected = color[0];
    } else {
      trianguloSelected = -1;
    }
    if (!cambioColor[1]) {
      cambioColor[0] = false;
      cambioColor[1] = true;
      cambioColor[2] = false;
      cambioColor[3] = false;
    } else {
      cambioColor[1] = false;
    }
  } else if (color[0] == 150) {// Cuerpo
    trianguloSelected = -1;
    if (!cambioColor[2]) {
      cambioColor[0] = false;
      cambioColor[1] = false;
      cambioColor[2] = true;
      cambioColor[3] = false;
    } else {
      cambioColor[2] = false;
    }
  } else if (color[0] == 175) { // Cabeza
    trianguloSelected = -1;
    if (!cambioColor[3]) {
      cambioColor[0] = false;
      cambioColor[1] = false;
      cambioColor[2] = false;
      cambioColor[3] = true;
    } else {
      cambioColor[3] = false;
    }
  } else {
    trianguloSelected = -1;
  }
}

void pick_color(int x, int y)
{
  GLint viewport[4];
  unsigned char pixel[3];

  glGetIntegerv(GL_VIEWPORT, viewport);
  glReadBuffer(GL_BACK);
  glReadPixels(x,viewport[3]-y,1,1,GL_RGB,GL_UNSIGNED_BYTE,(GLubyte *) &pixel[0]);
  printf(" valor x %d, valor y %d, color %d, %d, %d \n",x,y,pixel[0],pixel[1],pixel[2]);

  procesar_color(pixel);
  glutPostRedisplay();
}
  
//***************************************************************************
// Funciones para manejo de eventos del ratón
//***************************************************************************

void clickRaton(int boton, int estado, int x, int y)
{
  switch(boton) {
    case GLUT_RIGHT_BUTTON:
      if (estado == GLUT_DOWN)
      {
        estadoRaton[2] = 1;
        xc = x;
        yc = y;
      }
      else {
        estadoRaton[2] = 1;
      }
      break;
    case GLUT_LEFT_BUTTON:
      if( estado == GLUT_DOWN) {
        estadoRaton[2] = 2;
        xc=x;
        yc=y;
        pick_color(xc, yc);
      }
      break;
    case 3:
        factor/=1.1;
        Observer_distance/=1.1;
        glutPostRedisplay();
        break;
    case 4:
        factor*=1.1;
        Observer_distance*=1.1;
        glutPostRedisplay();
        break;
  }
}

void getCamara(GLfloat *x, GLfloat *y)
{
  *x = Observer_angle_x;
  *y = Observer_angle_y;
}

void setCamara(GLfloat x, GLfloat y)
{
  Observer_angle_x = x;
  Observer_angle_y = y;
}

void RatonMovido(int x, int y)
{
  float x0, y0, xn, yn;
  if (estadoRaton[2] == 1)
  {
    getCamara(&y0, &x0);
    yn = y0 + (y - yc);
    xn = x0 - (x - xc);
    setCamara(yn, xn);
    xc = x;
    yc = y;
    glutPostRedisplay();
  }
}

//**************************************************************************
//
//***************************************************************************

void clean_window()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection_per()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-Size_x, Size_x, -Size_y, Size_y, Front_plane, Back_plane);
}

void change_projection_ortho_alzado()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-15 * factor, 15 * factor, -15 * factor, 15 * factor, -100, 100);
}

void change_projection_ortho_vistas()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-15 * factor, 15 * factor, -15 * factor, 15 * factor, -100, 100);
}
//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer_perspectiva()
{
  // posicion del observador
  glViewport(0, 0, Window_width, Window_high);
  change_projection_per();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -Observer_distance);
  glRotatef(Observer_angle_x, 1, 0, 0);
  glRotatef(Observer_angle_y, 0, 1, 0);
}


void change_observer_ortho_vista_a()
{
  const int w = Window_width;
  const int h = Window_high;

  // right bottom
  glViewport(w/2, h/2, w, h);
  glRotatef(Observer_angle_x, 1, 0, 0);
  glRotatef(Observer_angle_y, 0, 1, 0);

  change_projection_ortho_vistas();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void change_observer_ortho_vista_b()
{
  const int w = Window_width;
  const int h = Window_high;

  // left bottom
  glViewport(0, h/2, w/2, h);
  glRotatef(Observer_angle_x, 1, 0, 0);
  glRotatef(Observer_angle_y, 0, 1, 0);

  change_projection_ortho_vistas();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void change_observer_ortho_vista_c()
{
  const int w = Window_width;
  const int h = Window_high;

  // top right
  glViewport(w/2, 0, w, h/2);
  glRotatef(Observer_angle_x, 1, 0, 0);
  glRotatef(Observer_angle_y, 0, 1, 0);

  change_projection_ortho_vistas();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void change_observer_ortho_vista_d()
{
  const int w = Window_width;
  const int h = Window_high;

  // top left
  glViewport(0, 0, w/2, h/2);
  glRotatef(Observer_angle_x, 1, 0, 0);
  glRotatef(Observer_angle_y, 0, 1, 0);

  change_projection_ortho_vistas();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void change_observer_ortho()
{
  glViewport(0, 0, Window_width, Window_high);
  change_projection_ortho_alzado();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef(Observer_angle_x, 1, 0, 0);
  glRotatef(Observer_angle_y, 0, 1, 0);
}
//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{
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

void draw_objects()
{

  switch (t_objeto)
  {
  case CUBO:
    cubo.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
    break;
  case PIRAMIDE:
    piramide.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
    break;
  case OBJETO_PLY:
    ply.draw(modo, 1.0, 0.6, 0.0, 0.0, 1.0, 0.3, 2);
    break;
  case ROTACION:
    rotacion.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
    break;
  case ARTICULADO_ROBOT:
    robot.draw(modo, 0.05, 0.05, 0.9, 0.8, 0.8, 0.8, 2);
    break;
  case ESFERA:
    esfera.draw(modo, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 2);
    break;
  }
}

//**************************************************************************
// Funcion que dibuja los objetos
//****************************2***********************************************

void draw_objects_seleccion()
{
  robot.draw_seleccion_color(0,0,0);
}

//**************************************************************************
//
//***************************************************************************

void draw(void)
{
  glDrawBuffer(GL_FRONT);
  clean_window();
  if (vista == "perspectiva") {
    change_observer_perspectiva();
    draw_axis();
    draw_objects();
  } else if (vista == "camara") {
    change_observer_ortho();
    draw_axis();
    draw_objects();
  } else if (vista == "vistas") {
    change_observer_ortho_vista_a();
    draw_axis();
    draw_objects();
    change_observer_ortho_vista_b();
    draw_axis();
    draw_objects();
    change_observer_ortho_vista_c();
    draw_axis();
    draw_objects();
    change_observer_ortho_vista_d();
    draw_axis();
    draw_objects();
  }

  glDrawBuffer(GL_BACK);
  clean_window();
  if (vista == "perspectiva") {
    change_observer_perspectiva();
    draw_objects_seleccion();
  } else if (vista == "camara") {
    change_observer_ortho();
    draw_objects_seleccion();
  } else if (vista == "vistas") {
  }
  glFlush();
  // glutPostRedisplay();
}

//**************************************************************************
// Funcion que define las luces
//**************************************************************************

void luces(float alfa)
{

  // Valores para la luz 1
  GLfloat position1[4] = {20, -15, 0, 1},
          ambient1[4] = {0.5, 0.5, 0.5, 1},
          diffuse1[4] = {0.8, 0.8, 0.8, 1},
          intensity1[4] = {0.9, 0.9, 0.9, 1};

  GLfloat position2[4] = {0, 30, 0, 1},
          ambient2[4] = {0.3, 0.1, 0.1, 1},
          diffuse2[4] = {0.7, 0.2, 0.2, 1},
          intensity2[4] = {1.0, 0.2, 0.2, 1};

  glDisable(GL_LIGHT0);

  glLightfv(GL_LIGHT1, GL_POSITION, position1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, intensity1);
  glEnable(GL_LIGHT1);

  glLightfv(GL_LIGHT2, GL_POSITION, position2);
  glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
  glLightfv(GL_LIGHT2, GL_SPECULAR, intensity2);
  glEnable(GL_LIGHT2);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glRotatef(alfa, 0, 1, 0);
  glLightfv(GL_LIGHT1, GL_POSITION, position1);
  glPopMatrix();
}

//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1, int Alto1)
{
  Window_width = Ancho1;
  Window_high = Alto1;
  float Aspect_ratio;
  Aspect_ratio = (float)Alto1 / (float)Ancho1;
  Size_y = Size_x * Aspect_ratio;
  change_projection_per();
  glViewport(0, 0, Ancho1, Alto1);
  glutPostRedisplay();
}

//**************************************************************************
// Funcion que anima los objetos
//**************************************************************************

void idle()
{

  if (luzGirando)
  {
    // Movimiento de rotación de la luz en torno al objeto
    luces(0 + giroLuz);
    giroLuz += 1 * velocidad_animacion;
  }

  if (animacionRobot)
  {
    // Movimiento adelante - atras
    if (robot.avance >= robot.avance_max)
    {
      avanzando = false;
    }
    else if (robot.avance <= robot.avance_min)
    {
      avanzando = true;
    }
    // Inclinación del cuerpo
    if (robot.inclinacion_cuerpo >= robot.inclinacion_cuerpo_max)
    {
      inclinandose_del = false;
    }
    else if (robot.inclinacion_cuerpo <= robot.inclinacion_cuerpo_min)
    {
      inclinandose_del = true;
    }
    // Antena
    if (robot.cabeza.altura_antena >= robot.cabeza.altura_antena_max)
    {
      antena_subiendo = false;
    }
    else if (robot.cabeza.altura_antena <= robot.cabeza.altura_antena_min)
    {
      antena_subiendo = true;
    }
    // Realizamos movimientos
    if (rotar_cabeza)
    {
      robot.giro_cabeza += 2 * velocidad_animacion;
    }
    if (girando)
    {
      robot.giro_completo += 0.5 * velocidad_animacion;
    }
    if (desplazamiento)
    {
      if (avanzando)
      {
        robot.avance += 0.2 * velocidad_animacion;
      }
      else
      {
        robot.avance -= 0.2 * velocidad_animacion;
      }
    }
    if (rotar_cuerpo)
    {
      if (inclinandose_del)
      {
        robot.inclinacion_cuerpo += 2 * velocidad_animacion;
      }
      else
      {
        robot.inclinacion_cuerpo -= 2 * velocidad_animacion;
      }
    }
    if (mover_atena)
    {
      if (antena_subiendo)
      {
        robot.cabeza.altura_antena += 0.15 * velocidad_animacion;
      }
      else
      {
        robot.cabeza.altura_antena -= 0.15 * velocidad_animacion;
      }
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

void normal_key(unsigned char Tecla1, int x, int y)
{
  switch (toupper(Tecla1))
  {
  case 'C':
    if (vista != "perspectiva") {
      vista = "perspectiva";
    }
    break;
  case 'V':
    if (vista != "camara") {
      vista = "camara";
    }
    break;
  case 'B':
    if (vista != "vistas") {
      vista = "vistas";
    }
    break;
  case 'Q':
    exit(0);
  case 'A':
    glutIdleFunc(idle);
    animacionRobot = true;
    break;
  case 'S':
    glutIdleFunc(idle);
    animacionRobot = false;
    break;
  case '+':
    velocidad_animacion *= 1.2;
    if (velocidad_animacion >= 10)
    {
      velocidad_animacion = 10;
    }
    break;
  case '-':
    velocidad_animacion /= 1.2;
    if (velocidad_animacion <= 0.1)
    {
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

void special_key(int Tecla1, int x, int y)
{

  switch (Tecla1)
  {
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
    if (robot.cabeza.altura_antena > robot.cabeza.altura_antena_max)
    {
      robot.cabeza.altura_antena = robot.cabeza.altura_antena_max;
    }
    break;
  case GLUT_KEY_F8:
    robot.cabeza.altura_antena -= 0.15;
    if (robot.cabeza.altura_antena < robot.cabeza.altura_antena_min)
    {
      robot.cabeza.altura_antena = robot.cabeza.altura_antena_min;
    }
    break;
  case GLUT_KEY_F9:
    robot.inclinacion_cuerpo += 2;
    if (robot.inclinacion_cuerpo > robot.inclinacion_cuerpo_max)
    {
      robot.inclinacion_cuerpo = robot.inclinacion_cuerpo_max;
    }
    break;
  case GLUT_KEY_F10:
    robot.inclinacion_cuerpo -= 2;
    if (robot.inclinacion_cuerpo < robot.inclinacion_cuerpo_min)
    {
      robot.inclinacion_cuerpo = robot.inclinacion_cuerpo_min;
    }
    break;
  }

  glutPostRedisplay();
}

//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{
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
  change_projection_per();
  luces(0);
  glViewport(0, 0, Window_width, Window_high);
}

//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************

int main(int argc, char **argv)
{

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
  glutCreateWindow("PRACTICA - 5");

  // asignación de la funcion llamada "dibujar" al evento de dibujo
  glutDisplayFunc(draw);
  // asignación de la funcion llamada "change_window_size" al evento correspondiente
  glutReshapeFunc(change_window_size);
  // asignación de la funcion llamada "normal_key" al evento correspondiente
  glutKeyboardFunc(normal_key);
  // asignación de la funcion llamada "tecla_Especial" al evento correspondiente
  glutSpecialFunc(special_key);

  // eventos ratón
  glutMouseFunc( clickRaton );
  glutMotionFunc( RatonMovido );

  // funcion de inicialización
  initialize();

  // inicio del bucle de eventos
  glutMainLoop();
  return 0;
}
