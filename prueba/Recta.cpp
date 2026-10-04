#include "Recta.h"
#include <glm/gtc/matrix_transform.hpp>


Recta::Recta(float l)
{
	largo = l;
	float mitad = ANCHURA / 2.0f;   // 78 mm
	float vFinal = largo / 350.0f;

	numVertices = 4;
	numFaces = 2;
	vertices = new GLfloat[numVertices * 3];
	normals = new GLfloat[numVertices * 3];
	textures = new GLfloat[numVertices * 2];
	indexes = new GLushort[numFaces * 3];

	GLfloat v[12] = { 0.0f, 0.0f, -mitad,   0.0f, 0.0f, mitad,   largo, 0.0f, mitad,   largo, 0.0f, -mitad };
	GLfloat t[8] = { 0.0f, 0.0f,   1.0f, 0.0f,   1.0f, vFinal,   0.0f, vFinal };
	for (int i = 0; i < 12; i++)
		vertices[i] = v[i];
	for (int i = 0; i < 4; i++)
	{
		normals[3 * i] = 0.0f;
		normals[3 * i + 1] = 1.0f;
		normals[3 * i + 2] = 0.0f;
	}
	for (int i = 0; i < 8; i++)
		textures[i] = t[i];

	GLushort idx[6] = { 0, 1, 2,   0, 2, 3 };
	for (int i = 0; i < 6; i++)
		indexes[i] = idx[i];

	InitBuffers();
}


glm::mat4 Recta::Salida()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(largo, 0.0f, 0.0f));
}


glm::mat4 Recta::PoseRanura(int carril, float s)
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(s, 0.0f, DesplazamientoCarril(carril)));
}
