//**************************************************************************
// Práctica 4 Iluminación
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>
#include "file_ply_stl.hpp"

using namespace std;

const float AXIS_SIZE = 5000;
typedef enum
{
       POINTS,
       EDGES,
       SOLID_CHESS,
       SOLID,
       SOLID_ILLUMINATED_FLAT,
       SOLID_ILLUMINATED_GOURAUD,
       SOLID_ILLUMINATED_GOURAUD_MULTI,
       SOLID_ILLUMINATED_ESFERA_GOURAUD
} _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:
       _puntos3D();
       void draw_puntos(float r, float g, float b, int grosor);

       vector<_vertex3f> vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D : public _puntos3D
{
public:
       _triangulos3D();
       void draw_aristas(float r, float g, float b, int grosor);
       void draw_solido(float r, float g, float b);
       void draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2);
       void draw_iluminacion_plana();
       void draw_iluminacion_suave();
       void draw_iluminacion_suave_multi();
       void draw_esfera_optimizada_suave();
       void draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

       void calcular_normales_caras();
       void calcular_normales_vertices();
       void calcular_normales_vertices_esfera();

       void cambiarMaterial(_vertex4f ambiente,_vertex4f difusa, _vertex4f especular, float brillo);

       vector<_vertex3i> caras;
       vector<_vertex3f> normales_caras;
       vector<_vertex3f> normales_vertices;
       bool b_normales_caras;
       bool b_normales_vertices;
       _vertex4f difusa;     //coeficientes ambiente y difuso
       _vertex4f ambiente;
       _vertex4f especular;           //coeficiente especular
       float brillo;                  //exponente del brillo 
};

//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo : public _triangulos3D
{
public:
       _cubo(float tam = 0.5);
};

//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide : public _triangulos3D
{
public:
       _piramide(float tam = 0.5, float al = 0.75);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply : public _triangulos3D
{
public:
       _objeto_ply();

       int parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************

class _rotacion : public _triangulos3D
{
public:
       _rotacion();
       void parametros(vector<_vertex3f> perfil1, int num1, int tapas);

       vector<_vertex3f> perfil;
       int num;
};

//************************************************************************
// objeto por revolución de Jose
//************************************************************************


class _rotacionJose: public _triangulos3D {
  public:
    _rotacionJose();
    void  parametros(vector<_vertex3f> perfil1, int num1, string ejeGiro="y", string tapas="2_tapas"); // tapas = [2_tapas, tapa_inf, tapa_sup, no_tapas] ejeGiro = [x, y, z]
    vector<_vertex3f> perfil; 
    int num;
};

// Practica 3: R2D2 -----------------------------------------------------------------------

// SOPORTE --------------------------------------------------------------------------------

class _basePata : public _triangulos3D
{
public:
       _basePata(float h = 2.5);
};

class _soporte : public _triangulos3D
{
public:
       _soporte();
       void draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
        _rotacionJose rueda;
        _basePata basePata;
        _cubo pata;
        _rotacionJose eje;
};

// CUERPO ---------------------------------------------------------------------------------

class _cuerpo : public _triangulos3D
{
public:
       _cuerpo();
       void draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
        _rotacionJose tronco;
        _rotacionJose baseTronco;
};


// CABEZA ---------------------------------------------------------------------------------

class _cabeza : public _triangulos3D
{
public:
       _cabeza();
       void draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

       float altura_antena;

       // Limites
       float altura_antena_max;
       float altura_antena_min;

protected:
        _rotacion baseCabeza;
        _rotacion bolaAntena;
        _rotacion baseAntena;
        _rotacion ojo;
};

// ROBOT ----------------------------------------------------------------------------------

class _robot : public _triangulos3D
{
public:
       _robot();
       void draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

       float giro_cabeza;
       float avance;
       float giro_completo;
       float inclinacion_cuerpo;

       _cabeza cabeza;

       // Limites
       float avance_max;
       float avance_min;
       float inclinacion_cuerpo_max;
       float inclinacion_cuerpo_min;

protected:
       _soporte soporte;
       _cuerpo cuerpo;
};
