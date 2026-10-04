#include "CGScene.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

const char* RUTAS_COCHES[NUM_COCHES] = { "Mclaren/", "Hrt/" };


const glm::vec3 DIR_LUZ = glm::vec3(-0.6f, -1.0f, -0.4f);

CGScene::CGScene()
{
	glm::vec3 Ldir = glm::normalize(DIR_LUZ);
	light = new CGLight();
	light->SetLightDirection(Ldir);
	light->SetAmbientLight(glm::vec3(0.3f, 0.3f, 0.3f));
	light->SetDifusseLight(glm::vec3(0.7f, 0.7f, 0.7f));
	light->SetSpecularLight(glm::vec3(0.4f, 0.4f, 0.4f));

	circuito = new Circuito();

	for (int i = 0; i < NUM_COCHES; i++)
	{
		coche[i] = new Coche();
		if (!coche[i]->Cargar(RUTAS_COCHES[i]))
			std::cout << "No se ha podido cargar el coche " << (i + 1) << std::endl;
		recorrido[i] = INICIO_COCHE[i];
		nivel[i] = 3;
	}
	PlaceCars();

	for (int c = 0; c < 2; c++)
	{
		float vuelta = circuito->LongitudCarril(c);
		for (float s = 0.0f; s < vuelta; s = s + 200.0f)
			puntos.push_back(glm::vec3(circuito->Pose(c, s)[3]));
	}
	InitLightMatrices();
}

CGScene::~CGScene()
{
	for (int i = 0; i < NUM_COCHES; i++)
		delete coche[i];
	delete circuito;
	delete light;
}


void CGScene::PlaceCars()
{
	for (int i = 0; i < NUM_COCHES; i++)
		coche[i]->SetLocation(circuito->Pose(i, recorrido[i]));
}


void CGScene::InitLightMatrices()
{
	glm::vec3 dir = glm::normalize(light->GetLightDirection());

	glm::vec3 minimo = glm::vec3(1e9f);
	glm::vec3 maximo = glm::vec3(-1e9f);
	for (int i = 0; i < (int)puntos.size(); i++)
	{
		minimo = glm::min(minimo, puntos[i]);
		maximo = glm::max(maximo, puntos[i]);
	}
	glm::vec3 centro = 0.5f * (minimo + maximo);
	lightViewMatrix = glm::lookAt(centro - dir * 10000.0f, centro, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 minLuz = glm::vec3(1e9f);
	glm::vec3 maxLuz = glm::vec3(-1e9f);
	for (int i = 0; i < (int)puntos.size(); i++)
	{
		glm::vec3 q = glm::vec3(lightViewMatrix * glm::vec4(puntos[i], 1.0f));
		minLuz = glm::min(minLuz, q);
		maxLuz = glm::max(maxLuz, q);
	}

	float margen = 200.0f;
	lightProjection = glm::ortho(minLuz.x - margen, maxLuz.x + margen,
		minLuz.y - margen, maxLuz.y + margen,
		-maxLuz.z - margen, -minLuz.z + margen);
}

glm::mat4 CGScene::GetLightViewMatrix()
{
	return lightViewMatrix;
}

glm::mat4 CGScene::GetLightProjectionMatrix()
{
	return lightProjection;
}

std::vector<glm::vec3>& CGScene::GetPoints()
{
	return puntos;
}

glm::mat4 CGScene::GetCarPose(int i)
{
	return circuito->Pose(i, recorrido[i]);
}


void CGScene::Update()
{
	for (int i = 0; i < NUM_COCHES; i++)
	{
		recorrido[i] = recorrido[i] + nivel[i] * VEL_MAX / 10.0f;

		float vuelta = circuito->LongitudCarril(i);
		if (recorrido[i] >= vuelta)
			recorrido[i] = recorrido[i] - vuelta;
	}
	PlaceCars();
}

void CGScene::Accelerate(int i)
{
	if (nivel[i] < 10)
		nivel[i]++;
}

void CGScene::Brake(int i)
{
	if (nivel[i] > 0)
		nivel[i]--;
}


void CGScene::Draw(CGShaderProgram* program, glm::mat4 proj, glm::mat4 view, glm::mat4 shadowMatrix)
{
	light->SetUniforms(program);

	circuito->Draw(program, proj, view, shadowMatrix);
	for (int i = 0; i < NUM_COCHES; i++)
		coche[i]->Draw(program, proj, view, shadowMatrix);
}

void CGScene::DrawShadow(CGShaderProgram* program, glm::mat4 shadowMatrix)
{
	for (int i = 0; i < NUM_COCHES; i++)
		coche[i]->DrawShadow(program, shadowMatrix);
}
