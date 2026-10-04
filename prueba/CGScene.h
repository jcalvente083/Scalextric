#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "CGShaderProgram.h"
#include "CGLight.h"
#include "Circuito.h"
#include "Coche.h"

const int NUM_COCHES = 2;
const float VEL_MAX = 15.0f;   

const float INICIO_COCHE[NUM_COCHES] = { 105.0f, 232.0f };
class CGScene {
public:
	CGScene();
	~CGScene();
	void Draw(CGShaderProgram* program, glm::mat4 proj, glm::mat4 view, glm::mat4 shadowMatrix);
	void DrawShadow(CGShaderProgram* program, glm::mat4 shadowMatrix);


	void Update();


	void Accelerate(int i);
	void Brake(int i);

	glm::mat4 GetCarPose(int i);

	
	glm::mat4 GetLightViewMatrix();
	glm::mat4 GetLightProjectionMatrix();

	std::vector<glm::vec3>& GetPoints();

private:
	Circuito* circuito;
	Coche* coche[NUM_COCHES];
	CGLight* light;

	float recorrido[NUM_COCHES];   
	int nivel[NUM_COCHES];        

	std::vector<glm::vec3> puntos;
	glm::mat4 lightViewMatrix;
	glm::mat4 lightProjection;

	void PlaceCars();
	void InitLightMatrices();
};
