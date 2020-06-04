//**************************************************************************
// Práctica 4 usando objetos
//**************************************************************************

#include "objetos_B4.h"
#include <algorithm>

//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
	//**** usando vertex_array ****
	glPointSize(grosor);
	glColor3f(r, g, b);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glDrawArrays(GL_POINTS, 0, vertices.size());
}

//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D()
{
	b_normales_caras = false;
	b_normales_vertices = false;
	ambiente = {0.1745, 0.01175, 0.01175, 0.55};
	difusa = {0.61424, 0.04136, 0.04136, 0.55};
	especular = {0.727811, 0.626959, 0.626959, 0.55};
	brillo = 76.8;
}

//*************************************************************************
// _triangulos3D
//*************************************************************************

void _triangulos3D::cambiarMaterial(_vertex4f ambienteN, _vertex4f difusaN, _vertex4f especularN, float brilloN){
	
	ambiente = ambienteN;
	difusa = difusaN;
	especular = especularN;
	brillo = brilloN;

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(GLfloat *)&ambienteN);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,(GLfloat *)&difusaN);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(GLfloat *)&especularN);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,(GLfloat *)&brilloN);

}

//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
	//**** usando vertex_array ****
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(grosor);
	glColor3f(r, g, b);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glDrawElements(GL_TRIANGLES, caras.size() * 3, GL_UNSIGNED_INT, &caras[0]);
}

//*************************************************************************
// dibujar en modo sólido
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
	int i;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < caras.size(); i++)
	{
		glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
	}
	glEnd();
}

//*************************************************************************
// dibujar en modo sólido con apariencia de ajedrez
//*************************************************************************

void _triangulos3D::draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2)
{
	int i;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < caras.size(); i++)
	{
		if (i % 2 == 0)
			glColor3f(r1, g1, b1);
		else
			glColor3f(r2, g2, b2);
		glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
	}
	glEnd();
}

//*************************************************************************
// dibujar en modo sólido con iluminación plana
//*************************************************************************

void _triangulos3D::draw_iluminacion_plana()
{
	int i;
	if (!b_normales_caras) { calcular_normales_caras(); }
	glShadeModel(GL_FLAT);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	// Material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)&ambiente);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)&difusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

	// Dibujamos objeto
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
		for (i = 0; i < caras.size(); i++) {
			glNormal3fv((GLfloat *) &normales_caras[i]);
			glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
			glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
			glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
		}
	glEnd();
	glDisable(GL_LIGHTING);
}


//*************************************************************************
// dibujar en modo sólido con iluminación Gouraud
//*************************************************************************

void _triangulos3D::draw_iluminacion_suave()
{
	int i;
	if (!b_normales_vertices) { calcular_normales_vertices(); }

	// Calcular maximos y minimos
	float max_x, min_x, max_y, min_y;
	max_x = min_x = vertices[0].x;
	max_y = min_y = vertices[0].y;

	for(i = 0; i < vertices.size(); i++) {
		if(vertices[i].x > max_x) max_x = vertices[i].x;
		if(vertices[i].x < min_x) min_x = vertices[i].x;
		if(vertices[i].y > max_y) max_y = vertices[i].y;
		if(vertices[i].y < min_y) min_y = vertices[i].y;
	}

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	// Material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)&ambiente);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)&difusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
		for(i = 0; i < caras.size(); i++) {
			glNormal3fv((GLfloat *)&normales_vertices[caras[i]._0]);
			glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
			glNormal3fv((GLfloat *)&normales_vertices[caras[i]._1]);
			glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
			glNormal3fv((GLfloat *)&normales_vertices[caras[i]._2]);
			glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
		}
	glEnd();
	glDisable(GL_LIGHTING);
}

//*************************************************************************
// dibujar en modo sólido con iluminación Gouraud multicolor
//*************************************************************************

void _triangulos3D::draw_iluminacion_suave_multi()
{
	int i;
	if (!b_normales_vertices) { calcular_normales_vertices(); }

	GLfloat oro[4]={0.6,0.7,0.2,1};
	GLfloat jade[4]={0.54, 0.89, 0.63, 0.95};

	// Calcular maximos y minimos
	float max_x, min_x, max_y, min_y;
	max_x = min_x = vertices[0].x;
	max_y = min_y = vertices[0].y;

	for(i = 0; i < vertices.size(); i++) {
		if(vertices[i].x > max_x) max_x = vertices[i].x;
		if(vertices[i].x < min_x) min_x = vertices[i].x;
		if(vertices[i].y > max_y) max_y = vertices[i].y;
		if(vertices[i].y < min_y) min_y = vertices[i].y;
	}

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	// Material
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
		for(i = 0; i < caras.size(); i++) {
			oro[0]=0.7-(0.7-1.0)*(max_x-vertices[caras[i]._0].x)/(max_x-min_x);
			oro[1]=0.9-(0.9-0.2)*(max_x-vertices[caras[i]._0].x)/(max_x-min_x);
			oro[2]=0.1-(0.1-0.3)*(max_x-vertices[caras[i]._0].x)/(max_x-min_x);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *)&oro);
			glNormal3fv((GLfloat *)&normales_vertices[caras[i]._0]);
			glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
			glNormal3fv((GLfloat *)&normales_vertices[caras[i]._1]);
			glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
			glNormal3fv((GLfloat *)&normales_vertices[caras[i]._2]);
			glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
		}
	glEnd();
	glDisable(GL_LIGHTING);
}

//*************************************************************************
// dibujar en modo sólido ESFERA con iluminación Gouraud
//*************************************************************************

void _triangulos3D::draw_esfera_optimizada_suave()
{
	int i;
	if (!b_normales_vertices) { calcular_normales_vertices(); }

	// Calcular maximos y minimos
	float max_x, min_x, max_y, min_y;
	max_x = min_x = vertices[0].x;
	max_y = min_y = vertices[0].y;

	for(i = 0; i < vertices.size(); i++) {
		if(vertices[i].x > max_x) max_x = vertices[i].x;
		if(vertices[i].x < min_x) min_x = vertices[i].x;
		if(vertices[i].y > max_y) max_y = vertices[i].y;
		if(vertices[i].y < min_y) min_y = vertices[i].y;
	}

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	// Material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)&ambiente);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)&difusa);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
		for(i = 0; i < caras.size(); i++) {
			glNormal3fv((GLfloat *)&normales_vertices[caras[i]._0]);
			glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
			glNormal3fv((GLfloat *)&normales_vertices[caras[i]._1]);
			glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
			glNormal3fv((GLfloat *)&normales_vertices[caras[i]._2]);
			glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
		}
	glEnd();
	glDisable(GL_LIGHTING);
}


//*************************************************************************
// calcular normales a las caras para iluminación plana
//*************************************************************************

void _triangulos3D::calcular_normales_caras()
{
	normales_caras.resize(caras.size());
	float modulo;
	_vertex3f aux1, aux2, product;
	for (int i = 0; i < caras.size(); i++){
		aux1 = vertices[caras[i]._1] - vertices[caras[i]._0],
		aux2 = vertices[caras[i]._2] - vertices[caras[i]._0],
		product = aux1.cross_product(aux2);
		// Modulo
		modulo = sqrt(product.x * product.x + product.y * product.y + product.z * product.z);
		// Vector normal normalizado
		normales_caras[i] = _vertex3f(product.x/modulo, product.y/modulo, product.z/modulo);
	}
	b_normales_caras = true;
}

//*************************************************************************
// calcular normales a los verticés para iluminación suave
//*************************************************************************

void _triangulos3D::calcular_normales_vertices()
{
	int i;
	// initgauss();
	if(!b_normales_vertices) {
		if(!b_normales_vertices) { calcular_normales_caras(); }
		normales_vertices.resize(vertices.size());
		for(i = 0; i < vertices.size(); i++) {
			normales_vertices[i].x=0.0;
			normales_vertices[i].y=0.0;
			normales_vertices[i].z=0.0;
		}
		for(i=0; i < caras.size(); i++) {
			normales_vertices[caras[i]._0]+=normales_caras[i];
			normales_vertices[caras[i]._1]+=normales_caras[i];
			normales_vertices[caras[i]._2]+=normales_caras[i];
		}
		for(i=0; i < vertices.size(); i++) {
			normales_vertices[i].normalize();
		}
	}
}

//*************************************************************************
// calcular normales a los verticés para esfera optimizado
//*************************************************************************

void _triangulos3D::calcular_normales_vertices_esfera()
{
	int i;
	// initgauss();
	if(!b_normales_vertices) {
		normales_vertices.resize(vertices.size());
		for(i = 0; i < vertices.size(); i++) {
			normales_vertices[i].x=vertices[i].x;
			normales_vertices[i].y=vertices[i].y;
			normales_vertices[i].z=vertices[i].z;
		}
		for(i=0; i < vertices.size(); i++) {
			normales_vertices[i].normalize();
		}
	}
}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	switch (modo)
	{
	case POINTS:
		draw_puntos(r1, g1, b1, grosor);
		break;
	case EDGES:
		draw_aristas(r1, g1, b1, grosor);
		break;
	case SOLID_CHESS:
		draw_solido_ajedrez(r1, g1, b1, r2, g2, b2);
		break;
	case SOLID:
		draw_solido(r1, g1, b1);
		break;
	case SOLID_ILLUMINATED_FLAT:
		draw_iluminacion_plana();
		break;
	case SOLID_ILLUMINATED_GOURAUD:
		draw_iluminacion_suave();
		break;
	case SOLID_ILLUMINATED_GOURAUD_MULTI:
		draw_iluminacion_suave_multi();
		break;
	case SOLID_ILLUMINATED_ESFERA_GOURAUD:
		draw_esfera_optimizada_suave();
		break;
	}
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
	//vertices
	vertices.resize(8);
	vertices[0].x = -tam;
	vertices[0].y = -tam;
	vertices[0].z = tam;
	vertices[1].x = tam;
	vertices[1].y = -tam;
	vertices[1].z = tam;
	vertices[2].x = tam;
	vertices[2].y = tam;
	vertices[2].z = tam;
	vertices[3].x = -tam;
	vertices[3].y = tam;
	vertices[3].z = tam;
	vertices[4].x = -tam;
	vertices[4].y = -tam;
	vertices[4].z = -tam;
	vertices[5].x = tam;
	vertices[5].y = -tam;
	vertices[5].z = -tam;
	vertices[6].x = tam;
	vertices[6].y = tam;
	vertices[6].z = -tam;
	vertices[7].x = -tam;
	vertices[7].y = tam;
	vertices[7].z = -tam;

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

//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{

	//vertices
	vertices.resize(5);
	vertices[0].x = -tam;
	vertices[0].y = 0;
	vertices[0].z = tam;
	vertices[1].x = tam;
	vertices[1].y = 0;
	vertices[1].z = tam;
	vertices[2].x = tam;
	vertices[2].y = 0;
	vertices[2].z = -tam;
	vertices[3].x = -tam;
	vertices[3].y = 0;
	vertices[3].z = -tam;
	vertices[4].x = 0;
	vertices[4].y = al;
	vertices[4].z = 0;

	caras.resize(6);
	caras[0]._0 = 0;
	caras[0]._1 = 1;
	caras[0]._2 = 4;
	caras[1]._0 = 1;
	caras[1]._1 = 2;
	caras[1]._2 = 4;
	caras[2]._0 = 2;
	caras[2]._1 = 3;
	caras[2]._2 = 4;
	caras[3]._0 = 3;
	caras[3]._1 = 0;
	caras[3]._2 = 4;
	caras[4]._0 = 3;
	caras[4]._1 = 1;
	caras[4]._2 = 0;
	caras[5]._0 = 3;
	caras[5]._1 = 2;
	caras[5]._2 = 1;
}

//*************************************************************************
// clase objeto ply
//*************************************************************************

_objeto_ply::_objeto_ply()
{
	// leer lista de coordenadas de vértices y lista de indices de vértices
}

int _objeto_ply::parametros(char *archivo)
{
	int n_ver, n_car;

	vector<float> ver_ply;
	vector<int> car_ply;

	_file_ply::read(archivo, ver_ply, car_ply);

	n_ver = ver_ply.size() / 3;
	n_car = car_ply.size() / 3;

	printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

	//if (n_ver<3 || n_car<1){
	//	printf("Error %d %d\n",__FILE__,__LINE__);
	//	exit(-1);
	//	}

	vertices.resize(n_ver);
	caras.resize(n_car);

	_vertex3f ver_aux;
	_vertex3i car_aux;

	for (int i = 0; i < n_ver; i++)
	{
		ver_aux.x = ver_ply[i * 3];
		ver_aux.y = ver_ply[i * 3 + 1];
		ver_aux.z = ver_ply[i * 3 + 2];
		vertices[i] = ver_aux;
	}

	for (int i = 0; i < n_car; i++)
	{
		car_aux.x = car_ply[i * 3];
		car_aux.y = car_ply[i * 3 + 1];
		car_aux.z = car_ply[i * 3 + 2];
		caras[i] = car_aux;
	}

	return (0);
}

//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{
}

void _rotacion::parametros(vector<_vertex3f> perfil, int num, int tapa)
{
	int i, j;
	_vertex3f vertice_aux;
	_vertex3i cara_aux;
	int num_aux;
	num_aux = perfil.size();
	vertices.resize(num_aux * num);
	for (j = 0; j < num; j++)
	{
		for (i = 0; i < num_aux; i++)
		{
			vertice_aux.x = perfil[i].x * cos(2.0 * M_PI * j / (1.0 * num)) +
							perfil[i].z * sin(2.0 * M_PI * j / (1.0 * num));
			vertice_aux.z = -perfil[i].x * sin(2.0 * M_PI * j / (1.0 * num)) +
							perfil[i].z * cos(2.0 * M_PI * j / (1.0 * num));
			vertice_aux.y = perfil[i].y;
			vertices[i + j * num_aux] = vertice_aux;
		}
	}
	// tratamiento de las caras
	for (j = 0; j < num; j++)
	{
		for (i = 0; i < num_aux - 1; i++)
		{
			cara_aux._0 = i + ((j + 1) % num) * num_aux;
			cara_aux._1 = i + 1 + ((j + 1) % num) * num_aux;
			cara_aux._2 = i + 1 + j * num_aux;
			caras.push_back(cara_aux);

			cara_aux._0 = i + 1 + j * num_aux;
			cara_aux._1 = i + j * num_aux;
			cara_aux._2 = i + ((j + 1) % num) * num_aux;
			caras.push_back(cara_aux);
		}
	}
	// tapa inferior
	if (fabs(perfil[0].x) > 0.0 && tapa == 1)
	{
	}
	// tapa superior
	if (fabs(perfil[num_aux - 1].x) > 0.0 && tapa == 1)
	{
	}
}

//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacionJose::_rotacionJose() {

}


void _rotacionJose::parametros(vector<_vertex3f> perfil, int num, string ejeGiro, string tapas) { // 2_tapas, tapa_inf, tapa_sup, no_tapas 


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
    for (int j = 0; j<num; j++) {
       for (int i = 0; i<(tam_aux-1); i++) {
			cara_aux._0 = i + ((j + 1) % num) * tam_aux;
			cara_aux._1 = i + 1 + ((j + 1) % num) * tam_aux;
			cara_aux._2 = i + 1 + j * tam_aux;
			caras[face_position++]=cara_aux;

			cara_aux._0 = i + 1 + j * tam_aux;
			cara_aux._1 = i + j * tam_aux;
			cara_aux._2 = i + ((j + 1) % num) * tam_aux;
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

// Practica 3: R2D2 -----------------------------------------------------------------------

// ROBOT ----------------------------------------------------------------------------------

_robot::_robot()
{
	giro_cabeza = 0.0;
	giro_completo = 0.0;
	avance = 0.0;
	inclinacion_cuerpo = 0.0;

	// Limites
	avance_max = 12;
	avance_min = -12;
	inclinacion_cuerpo_max = 40;
    inclinacion_cuerpo_min = -40;
};

void _robot::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	glPushMatrix();
		glTranslatef(0, -10, 0);
		glPushMatrix();
			glRotatef(giro_completo, 0, 1, 0);
			glTranslatef(avance, 0, 0);
			glPushMatrix();
				glTranslatef(0, 0, 5);
				soporte.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(0, 0, -5);
				glRotatef(180.0, 0, 1, 0);
				soporte.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
			glPopMatrix();

			glTranslatef(0, 10.5, 0);
			glRotatef(inclinacion_cuerpo, 0, 0, 1);
			glTranslatef(0, -10.5, 0);
			glPushMatrix();
				glTranslatef(0, 3.5, 0);
				cuerpo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
			glPopMatrix();

			glRotatef(giro_cabeza, 0, 1, 0);
			glPushMatrix();
				glTranslatef(0, 14.5, 0);
				cabeza.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
};

// SOPORTE --------------------------------------------------------------------------------

_soporte::_soporte()
{
	// perfil para un cilindro
	vector<_vertex3f> perfil;
	_vertex3f aux;
	aux.x = 0.5;
	aux.y = 0.0;
	aux.z = 0.0;
	perfil.push_back(aux);
	aux.x = 0.5;
	aux.y = 1.0;
	aux.z = 0.0;
	perfil.push_back(aux);
	rueda.parametros(perfil, 20);
	eje.parametros(perfil, 20);
};

void _soporte::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	glPushMatrix();
	glTranslatef(0, 0.5, 0);
	basePata.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.5, 0.1);
	glRotatef(90.0, 1, 0, 0);
	glScalef(1, 2.3, 1);
	rueda.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, 0.5, 0.1);
	glRotatef(90.0, 1, 0, 0);
	glScalef(1, 2.3, 1);
	rueda.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 0.5, 0.1);
	glRotatef(90.0, 1, 0, 0);
	glScalef(1, 2.3, 1);
	rueda.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 7.25, 1.25);
	glScalef(2, 8.5, 1.5);
	pata.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 11.5, -0.99);
	glRotatef(90.0, 1, 0, 0);
	glScalef(2, 3, 2);
	eje.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

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
	caras[0]._0 = 3;
	caras[0]._1 = 1;
	caras[0]._2 = 0;
	caras[1]._0 = 3;
	caras[1]._1 = 2;
	caras[1]._2 = 1;
	caras[2]._0 = 1;
	caras[2]._1 = 2;
	caras[2]._2 = 5;
	caras[3]._0 = 5;
	caras[3]._1 = 2;
	caras[3]._2 = 6;
	caras[4]._0 = 5;
	caras[4]._1 = 6;
	caras[4]._2 = 4;
	caras[5]._0 = 4;
	caras[5]._1 = 6;
	caras[5]._2 = 7;
	caras[6]._0 = 0;
	caras[6]._1 = 4;
	caras[6]._2 = 7;
	caras[7]._0 = 0;
	caras[7]._1 = 7;
	caras[7]._2 = 3;
	caras[8]._0 = 3;
	caras[8]._1 = 7;
	caras[8]._2 = 2;
	caras[9]._0 = 2;
	caras[9]._1 = 7;
	caras[9]._2 = 6;
	caras[10]._0 = 0;
	caras[10]._1 = 1;
	caras[10]._2 = 4;
	caras[11]._0 = 1;
	caras[11]._1 = 5;
	caras[11]._2 = 4;
}

// CUERPO ---------------------------------------------------------------------------------

_cuerpo::_cuerpo()
{
	// perfil para un cilindro
	vector<_vertex3f> perfil;
	_vertex3f aux;
	aux.x = 0.5;
	aux.y = 0.0;
	aux.z = 0.0;
	perfil.push_back(aux);
	aux.x = 0.5;
	aux.y = 1.0;
	aux.z = 0.0;
	perfil.push_back(aux);
	tronco.parametros(perfil, 20);
	vector<_vertex3f> perfil2;
	aux.x = 3.5;
	aux.y = 0.0;
	aux.z = 0.0;
	perfil2.push_back(aux);
	aux.x = 4.5;
	aux.y = 1.0;
	aux.z = 0.0;
	perfil2.push_back(aux);
	baseTronco.parametros(perfil2, 20);
};

void _cuerpo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	glPushMatrix();
	glTranslatef(0, 0, 0);
	baseTronco.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1, 0);
	glScalef(9, 10, 9);
	tronco.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
	glPopMatrix();

}

// CABEZA ---------------------------------------------------------------------------------

_cabeza::_cabeza()
{
	// perfil para un cilindro
	vector<_vertex3f> perfil;
	_vertex3f aux;
	aux.x = 0.5;
	aux.y = 0.0;
	aux.z = 0.0;
	perfil.push_back(aux);
	aux.x = 0.5;
	aux.y = 1.0;
	aux.z = 0.0;
	perfil.push_back(aux);
	baseAntena.parametros(perfil, 20, 1);
	// perfil para una esfera
	vector<_vertex3f> perfil_esfera;
	aux.x = 0.0;                    aux.y = -0.5;                   aux.z = 0.0;
	perfil_esfera.push_back(aux);
	aux.x = sin(M_PI / 8)*0.5;          aux.y = -cos(M_PI / 8)*0.5;         aux.z = 0.0;
	perfil_esfera.push_back(aux);
	aux.x = sin(M_PI / 4)*0.5;          aux.y = -cos(M_PI / 4)*0.5;         aux.z = 0.0;
	perfil_esfera.push_back(aux);
	aux.x = sin((M_PI / 8) * 3)*0.5;    aux.y = -cos((M_PI / 8) * 3)*0.5;   aux.z = 0.0;
	perfil_esfera.push_back(aux);
	aux.x = 0.5;                    aux.y = 0.0;                    aux.z = 0.0;
	perfil_esfera.push_back(aux);
	aux.x = sin((M_PI / 8) * 3)*0.5;    aux.y = cos((M_PI / 8) * 3)*0.5;    aux.z = 0.0;
	perfil_esfera.push_back(aux);
	aux.x = sin(M_PI / 4)*0.5;          aux.y = cos(M_PI / 4)*0.5;          aux.z = 0.0;
	perfil_esfera.push_back(aux);
	aux.x = sin(M_PI / 8)*0.5;          aux.y = cos(M_PI / 8)*0.5;          aux.z = 0.0;
	perfil_esfera.push_back(aux);
	aux.x = 0.0;                    aux.y = 0.5;                    aux.z = 0.0;
	perfil_esfera.push_back(aux);
	bolaAntena.parametros(perfil_esfera, 20, 0);
	ojo.parametros(perfil_esfera, 20, 0);
	// perfil para una semi-esfera
	vector<_vertex3f> perfil_semi_esfera;
	aux.x = 0.5;                    aux.y = 0.0;                    aux.z = 0.0;
	perfil_semi_esfera.push_back(aux);
	aux.x = sin((M_PI / 8) * 3)*0.5;    aux.y = cos((M_PI / 8) * 3)*0.5;    aux.z = 0.0;
	perfil_semi_esfera.push_back(aux);
	aux.x = sin(M_PI / 4)*0.5;          aux.y = cos(M_PI / 4)*0.5;          aux.z = 0.0;
	perfil_semi_esfera.push_back(aux);
	aux.x = sin(M_PI / 8)*0.5;          aux.y = cos(M_PI / 8)*0.5;          aux.z = 0.0;
	perfil_semi_esfera.push_back(aux);
	aux.x = 0.0;                    aux.y = 0.5;                    aux.z = 0.0;
	perfil_semi_esfera.push_back(aux);
	baseCabeza.parametros(perfil_semi_esfera, 20, 0);

	altura_antena = 1;

	// Limites
	altura_antena_max = 8;
	altura_antena_min = 1;
};

void _cabeza::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	glPushMatrix();
		glPushMatrix();
			glScalef(8, 8, 8);
			baseCabeza.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(3, 2, 0);
			glScalef(1.8, 1.8, 1.8);
			ojo.draw(modo, 1, 0, 0, 1, 0, 0, grosor);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-2.6, 2 + altura_antena, -2);
			glScalef(0.4, 0.4, 0.4);
			bolaAntena.draw(modo, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, grosor);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-2.6, 2, -2);
			glScalef(0.1, altura_antena, 0.1);
			baseAntena.draw(modo, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, grosor);
		glPopMatrix();
	glPopMatrix();

}