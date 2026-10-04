#include "Circuito.h"
#include "RectaEstandar.h"
#include "MediaRecta.h"
#include "CuartoRecta.h"
#include "CurvaInterior.h"
#include "CurvaEstandar.h"
#include "CurvaExterior.h"
#include <GL/glew.h>
#include <string>
#include <cmath>


const char* RUTA_TEXTURAS = "textures/";


Circuito::Circuito()
{
	total[0] = 0.0f;
	total[1] = 0.0f;
	longitudCentro = 0.0f;
	actual = glm::mat4(1.0f);


	std::string ruta = RUTA_TEXTURAS;
	matRecta = CrearMaterial((ruta + "RectaStd.png").c_str());
	matSalida = CrearMaterial((ruta + "RectaSalida.png").c_str());
	matCurvaInterior = CrearMaterial((ruta + "CurvaInterior.png").c_str());
	matCurvaEstandar = CrearMaterial((ruta + "CurvaStd.png").c_str());
	matCurvaExterior = CrearMaterial((ruta + "CurvaExterior.png").c_str());

	CrearCircuito();
	CerrarCircuito();
}

Circuito::~Circuito()
{
	for (int i = 0; i < (int)tramos.size(); i++)
		delete tramos[i].pista;

	CGMaterial* materiales[5] = { matRecta, matSalida, matCurvaInterior, matCurvaEstandar, matCurvaExterior };
	for (int i = 0; i < 5; i++)
	{
		GLuint textura = materiales[i]->GetTexture();
		glDeleteTextures(1, &textura);
		delete materiales[i];
	}
}

CGMaterial* Circuito::CrearMaterial(const char* fichero)
{
	CGMaterial* m = new CGMaterial();
	m->SetAmbientReflect(1.0f, 1.0f, 1.0f);
	m->SetDifusseReflect(1.0f, 1.0f, 1.0f);
	m->SetSpecularReflect(0.05f, 0.05f, 0.05f);
	m->SetShininess(16.0f);
	m->InitTexture(fichero);
	return m;
}


void Circuito::CrearCircuito()
{
   
    Agregar(new RectaEstandar(), matSalida);
    for (int i = 0; i < 4; i++)
        Agregar(new RectaEstandar(), matRecta);
    Agregar(new CurvaEstandar(false), matCurvaEstandar);
    Agregar(new CurvaExterior(false), matCurvaExterior);
    Agregar(new RectaEstandar(), matRecta);
    Agregar(new CurvaExterior(true), matCurvaExterior);
    for (int i = 0; i < 3; i++)
        Agregar(new CurvaInterior(false), matCurvaInterior);
    Agregar(new CurvaExterior(true), matCurvaExterior);
    Agregar(new RectaEstandar(), matRecta);
    Agregar(new CurvaExterior(false), matCurvaExterior);
    Agregar(new RectaEstandar(), matRecta);
    Agregar(new CurvaExterior(true), matCurvaExterior);
    Agregar(new CurvaExterior(true), matCurvaExterior);
    Agregar(new RectaEstandar(), matRecta);
    Agregar(new RectaEstandar(), matRecta);
    Agregar(new CurvaExterior(false), matCurvaExterior);
    for (int i = 0; i < 4; i++)
        Agregar(new RectaEstandar(), matRecta);
    Agregar(new CurvaExterior(false), matCurvaExterior);
    Agregar(new MediaRecta(), matRecta);
    Agregar(new CurvaExterior(true), matCurvaExterior);
    Agregar(new CurvaEstandar(true), matCurvaEstandar);
    Agregar(new RectaEstandar(), matRecta);
    Agregar(new CurvaInterior(false), matCurvaInterior);
    Agregar(new CurvaInterior(false), matCurvaInterior);
    Agregar(new RectaEstandar(), matRecta);
    Agregar(new CurvaExterior(false), matCurvaExterior);
    Agregar(new CuartoRecta(), matRecta);
    Agregar(new CurvaExterior(false), matCurvaExterior);
    Agregar(new MediaRecta(), matRecta);
    for (int i = 0; i < 7; i++)
        Agregar(new RectaEstandar(), matRecta);
    Agregar(new CurvaExterior(false), matCurvaExterior);
    Agregar(new RectaEstandar(), matRecta);
    Agregar(new RectaEstandar(), matRecta);
    Agregar(new CurvaExterior(false), matCurvaExterior);
    Agregar(new CurvaExterior(false), matCurvaExterior);
    for (int i = 0; i < 4; i++)
        Agregar(new RectaEstandar(), matRecta);
    Agregar(new CurvaExterior(false), matCurvaExterior);
    Agregar(new CurvaEstandar(false), matCurvaEstandar);
    for (int i = 0; i < 3; i++)
        Agregar(new RectaEstandar(), matRecta);
    Agregar(new CurvaExterior(false), matCurvaExterior);
    Agregar(new RectaEstandar(), matRecta);
    Agregar(new RectaEstandar(), matRecta);
    Agregar(new CurvaEstandar(false), matCurvaEstandar);
    Agregar(new CurvaEstandar(false), matCurvaEstandar);
    Agregar(new RectaEstandar(), matRecta);
    Agregar(new RectaEstandar(), matRecta);
    for (int i = 0; i < 3; i++)
        Agregar(new CurvaInterior(true), matCurvaInterior);
    for (int i = 0; i < 3; i++)
        Agregar(new RectaEstandar(), matRecta);}


void Circuito::Agregar(Pista* p, CGMaterial* material)
{
	p->SetMaterial(material);

	Tramo t;
	t.pista = p;
	t.modelo = actual;
	for (int c = 0; c < 2; c++)
	{
		t.inicio[c] = total[c];
		total[c] = total[c] + p->LongitudRanura(c);
	}
	t.inicioCentro = longitudCentro;
	longitudCentro = longitudCentro + p->Longitud();
	tramos.push_back(t);

	actual = actual * p->Salida();
}

void Circuito::CerrarCircuito()
{
	glm::vec3 error = glm::vec3(actual[3]) - glm::vec3(tramos[0].modelo[3]);
	for (int i = 0; i < (int)tramos.size(); i++)
	{
		float parte = tramos[i].inicioCentro / longitudCentro;
		tramos[i].modelo[3] = tramos[i].modelo[3] - glm::vec4(error * parte, 0.0f);
		tramos[i].pista->SetLocation(tramos[i].modelo);
	}
}

void Circuito::Draw(CGShaderProgram* program, glm::mat4 projection, glm::mat4 view, glm::mat4 shadowMatrix)
{
	for (int i = 0; i < (int)tramos.size(); i++)
		tramos[i].pista->Draw(program, projection, view, shadowMatrix);
}

glm::mat4 Circuito::Pose(int carril, float s)
{
	s = std::fmod(s, total[carril]);
	if (s < 0.0f)
		s = s + total[carril];

	int i = (int)tramos.size() - 1;
	while (i > 0 && tramos[i].inicio[carril] > s)
		i--;

	return tramos[i].modelo * tramos[i].pista->PoseRanura(carril, s - tramos[i].inicio[carril]);
}
