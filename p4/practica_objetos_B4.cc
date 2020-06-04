/*
Practica 4
*/

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_B4.h"

using namespace std;

// tipos
typedef enum {
  CUBO,
  PIRAMIDE,
  OBJETO_PLY,
  ROTACION,
  ARTICULADO_ROBOT,
  ESFERA
} _tipo_objeto;

_tipo_objeto t_objeto = PIRAMIDE;
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
float velocidad_animacion = 1.0;

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
_vertex4f jade_difusa = {0.135, 0.22,0.1575,0.95}; 
_vertex4f jade_ambiente = {0.54, 0.89, 0.63, 0.95};
_vertex4f jade_especular = {0.32,0.32,0.32,0.95};     
float jade_brillo = 12.8;  
// Ruby
_vertex4f ruby_ambiente = {0.1745, 0.01175, 0.01175, 0.55};
_vertex4f ruby_difusa = {0.61424, 0.04136, 0.04136, 0.55};
_vertex4f ruby_especular = {0.727811, 0.626959, 0.626959, 0.55};
float ruby_brillo = 76.8;
// Emerald
_vertex4f emerald_ambiente = {0.0215,0.1745,0.0215,0.55};
_vertex4f emerald_difusa = {0.07568,0.6142,0.0756, 0.55};
_vertex4f emerald_especular = {0.633,0.7278,0.633, 0.55};
float emerald_brillo = 76.8;

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
  glTranslatef(0, 0, -Observer_distance);
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

  switch (t_objeto) {
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
//
//***************************************************************************

void draw(void) {
  clean_window();
  change_observer();
  draw_axis();
  draw_objects();
  glutSwapBuffers();
}


//**************************************************************************
// Funcion que define las luces
//**************************************************************************

void luces(float alfa){

  // Valores para la luz 1
  GLfloat position1[4] = {20,-15,0,1},
          ambient1[4] = {0.5, 0.5, 0.5, 1},
          diffuse1[4] = {0.8, 0.8, 0.8, 1},
          intensity1[4] = {0.9, 0.9, 0.9, 1};

  GLfloat position2[4] = {0,30,0,1},
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

  if (luzGirando) {
    // Movimiento de rotación de la luz en torno al objeto
    luces(0+giroLuz);
    giroLuz += 1*velocidad_animacion;
  }

  if (animacionRobot) {
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
    case 'M':
      if (modo == POINTS) {
        modo = EDGES;
      } else if (modo == EDGES) {
        modo = SOLID;
      } else if (modo == SOLID) {
        modo = SOLID_CHESS;
      } else if (modo == SOLID_CHESS) {
        modo = SOLID_ILLUMINATED_FLAT;
        glutIdleFunc(idle);
      } else if (modo == SOLID_ILLUMINATED_FLAT) {
        modo = SOLID_ILLUMINATED_GOURAUD;
      } else if (modo == SOLID_ILLUMINATED_GOURAUD){
        modo = SOLID_ILLUMINATED_GOURAUD_MULTI;
      } else if (modo ==SOLID_ILLUMINATED_GOURAUD_MULTI){
        modo = POINTS;
      }
      break;
    case 'A':
      glutIdleFunc(idle);
      animacionRobot = true;
      break;
    case 'S':
      animacionRobot = false;
      break;
    case 'L':
      if (luzGirando) {
        glDisable(GL_LIGHT1);
      } else {
        glEnable(GL_LIGHT1);
      }
      luzGirando = !luzGirando;
      break;

    case 'T':
      ply.cambiarMaterial(jade_ambiente,jade_difusa, jade_especular, jade_brillo);
      if (t_objeto == ARTICULADO_ROBOT) {
        if (materialSeleccionado == "ruby"){
          materialSeleccionado = "jade";
          robot.cambiarMaterial(jade_ambiente,jade_difusa, jade_especular, jade_brillo);
        } else if (materialSeleccionado == "jade") {
          materialSeleccionado = "emerald";
          robot.cambiarMaterial(emerald_ambiente,emerald_difusa, emerald_especular, emerald_brillo);
        } else if (materialSeleccionado == "emerald"){
          materialSeleccionado = "ruby";
          robot.cambiarMaterial(ruby_ambiente,ruby_difusa, ruby_especular, ruby_brillo);
        }
      } else if (t_objeto == ROTACION) {
        if (materialSeleccionado == "ruby"){
          materialSeleccionado = "jade";
          rotacion.cambiarMaterial(jade_ambiente,jade_difusa, jade_especular, jade_brillo);
        } else if (materialSeleccionado == "jade") {
          materialSeleccionado = "emerald";
          rotacion.cambiarMaterial(emerald_ambiente,emerald_difusa, emerald_especular, emerald_brillo);
        } else if (materialSeleccionado == "emerald"){
          materialSeleccionado = "ruby";
          rotacion.cambiarMaterial(ruby_ambiente,ruby_difusa, ruby_especular, ruby_brillo);
        }
      } else if (t_objeto == OBJETO_PLY) {
        if (materialSeleccionado == "ruby"){
          materialSeleccionado = "jade";
          ply.cambiarMaterial(jade_ambiente,jade_difusa, jade_especular, jade_brillo);
        } else if (materialSeleccionado == "jade") {
          materialSeleccionado = "emerald";
          ply.cambiarMaterial(emerald_ambiente,emerald_difusa, emerald_especular, emerald_brillo);
        } else if (materialSeleccionado == "emerald"){
          materialSeleccionado = "ruby";
          ply.cambiarMaterial(ruby_ambiente,ruby_difusa, ruby_especular, ruby_brillo);
        }
      } else if (t_objeto == PIRAMIDE) {
        if (materialSeleccionado == "ruby"){
          materialSeleccionado = "jade";
          piramide.cambiarMaterial(jade_ambiente,jade_difusa, jade_especular, jade_brillo);
        } else if (materialSeleccionado == "jade") {
          materialSeleccionado = "emerald";
          piramide.cambiarMaterial(emerald_ambiente,emerald_difusa, emerald_especular, emerald_brillo);
        } else if (materialSeleccionado == "emerald"){
          materialSeleccionado = "ruby";
          piramide.cambiarMaterial(ruby_ambiente,ruby_difusa, ruby_especular, ruby_brillo);
        }
      } else if (t_objeto == CUBO) {
        if (materialSeleccionado == "ruby"){
          materialSeleccionado = "jade";
          cubo.cambiarMaterial(jade_ambiente,jade_difusa, jade_especular, jade_brillo);
        } else if (materialSeleccionado == "jade") {
          materialSeleccionado = "emerald";
          cubo.cambiarMaterial(emerald_ambiente,emerald_difusa, emerald_especular, emerald_brillo);
        } else if (materialSeleccionado == "emerald"){
          materialSeleccionado = "ruby";
          cubo.cambiarMaterial(ruby_ambiente,ruby_difusa, ruby_especular, ruby_brillo);
        }
      } else if (t_objeto == ESFERA) {
        if (materialSeleccionado == "ruby"){
          materialSeleccionado = "jade";
          esfera.cambiarMaterial(jade_ambiente,jade_difusa, jade_especular, jade_brillo);
        } else if (materialSeleccionado == "jade") {
          materialSeleccionado = "emerald";
          esfera.cambiarMaterial(emerald_ambiente,emerald_difusa, emerald_especular, emerald_brillo);
        } else if (materialSeleccionado == "emerald"){
          materialSeleccionado = "ruby";
          esfera.cambiarMaterial(ruby_ambiente,ruby_difusa, ruby_especular, ruby_brillo);
        }
      }
      break;

    case 'O':
      if (t_objeto == ARTICULADO_ROBOT) {
        t_objeto = ROTACION;
      } else if (t_objeto == ROTACION) {
        t_objeto = OBJETO_PLY;
      } else if (t_objeto == OBJETO_PLY) {
        t_objeto = PIRAMIDE;
      } else if (t_objeto == PIRAMIDE) {
        t_objeto = CUBO;
      } else if (t_objeto == CUBO) {
        t_objeto = ARTICULADO_ROBOT;
      } else if (t_objeto == ESFERA) {
        modo = SOLID_ILLUMINATED_GOURAUD;
        t_objeto = PIRAMIDE;
      }
      break;

    case 'E':
      t_objeto = ESFERA;
      modo = SOLID_ILLUMINATED_ESFERA_GOURAUD;
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
  luces(0);
  glViewport(0, 0, Window_width, Window_high);
}

//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************

int main(int argc, char **argv) {

  // creación del objeto ply
  ply.parametros(argv[1]);

  // perfil
  vector<_vertex3f> perfil2;
  _vertex3f aux;
  aux.x = 1.0;
  aux.y = -1.4;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 1.0;
  aux.y = -1.1;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.5;
  aux.y = -0.7;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.4;
  aux.y = -0.4;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.4;
  aux.y = 0.5;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.5;
  aux.y = 0.6;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.3;
  aux.y = 0.6;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.5;
  aux.y = 0.8;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.55;
  aux.y = 1.0;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.5;
  aux.y = 1.2;
  aux.z = 0.0;
  perfil2.push_back(aux);
  aux.x = 0.3;
  aux.y = 1.4;
  aux.z = 0.0;
  perfil2.push_back(aux);
  rotacion.parametros(perfil2, 32);

  // Esfera
  vector<_vertex3f> perfil_esfera;
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
  esfera.parametros(perfil_esfera, 32);

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
