#include "Curva.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

float Curva::Radio(float d)
{
	if (izquierda)
		return radioCentral + d;
	else
		return radioCentral - d;
}

glm::vec2 Curva::Punto(float d, float t)
{
	float r = Radio(d);
	float x = r * std::sin(t);
	float z;
	if (izquierda)
		z = -radioCentral + r * std::cos(t);
	else
		z = radioCentral - r * std::cos(t);
	return glm::vec2(x, z);
}


Curva::Curva(float radioInterior, float radioExterior, float anguloGrados, bool izq)
{
	radioCentral = (radioInterior + radioExterior) / 2.0f;
	angulo = glm::radians(anguloGrados);
	izquierda = izq;

	
	int n = (int)std::ceil(anguloGrados / 2.5f);
	if (n < 1)
		n = 1;

	float mitad = ANCHURA / 2.0f;

	float anchoCaja = radioExterior - radioInterior * std::cos(angulo);
	float altoCaja = radioExterior * std::sin(angulo);
	float ladoMayor = anchoCaja;
	if (altoCaja > ladoMayor)
		ladoMayor = altoCaja;
	float escala = 1.0f / ladoMayor;
	float pixelesPorMm = 512.0f * escala;
	float margenAngulo = 3.0f / (pixelesPorMm * radioInterior * angulo);

	numVertices = 2 * (n + 1);
	numFaces = 2 * n;
	vertices = new GLfloat[numVertices * 3];
	normals = new GLfloat[numVertices * 3];
	textures = new GLfloat[numVertices * 2];
	indexes = new GLushort[numFaces * 3];


	for (int i = 0; i <= n; i++)
	{
		float t = angulo * i / n;
		for (int lado = 0; lado < 2; lado++)
		{
			float d = -mitad;
			if (lado == 1)
				d = mitad;
			int k = 2 * i + lado;
			glm::vec2 p = Punto(d, t);
			vertices[3 * k] = p.x;
			vertices[3 * k + 1] = 0.0f;
			vertices[3 * k + 2] = p.y;
			normals[3 * k] = 0.0f;
			normals[3 * k + 1] = 1.0f;
			normals[3 * k + 2] = 0.0f;

			float r = Radio(d);
			float rTextura = radioCentral + (r - radioCentral) * 0.985f;
			float tTextura = t * (1.0f - margenAngulo);
			textures[2 * k] = 1.0f - (radioExterior - rTextura * std::cos(tTextura)) * escala;
			textures[2 * k + 1] = rTextura * std::sin(tTextura) * escala;
		}
	}

	for (int i = 0; i < n; i++)
	{
		GLushort a = 2 * i;
		GLushort b = a + 1;
		GLushort c = a + 2;
		GLushort d = a + 3;
		indexes[6 * i] = a;
		indexes[6 * i + 1] = b;
		indexes[6 * i + 2] = d;
		indexes[6 * i + 3] = a;
		indexes[6 * i + 4] = d;
		indexes[6 * i + 5] = c;
	}

	InitBuffers();
}


glm::mat4 Curva::Salida()
{
	glm::vec2 p = Punto(0.0f, angulo);
	float giro;
	if (izquierda)
		giro = angulo;
	else
		giro = -angulo;
	return glm::translate(glm::mat4(1.0f), glm::vec3(p.x, 0.0f, p.y)) *
		glm::rotate(glm::mat4(1.0f), giro, glm::vec3(0.0f, 1.0f, 0.0f));
}


float Curva::LongitudRanura(int carril)
{
	return Radio(DesplazamientoCarril(carril)) * angulo;
}


glm::mat4 Curva::PoseRanura(int carril, float s)
{
	float d = DesplazamientoCarril(carril);
	float t = s / Radio(d);
	glm::vec2 p = Punto(d, t);
	float giro;
	if (izquierda)
		giro = t;
	else
		giro = -t;
	return glm::translate(glm::mat4(1.0f), glm::vec3(p.x, 0.0f, p.y)) *
		glm::rotate(glm::mat4(1.0f), giro, glm::vec3(0.0f, 1.0f, 0.0f));
}
