#pragma once

#include <glm/glm.hpp>
#include "CGShaderProgram.h"

class CGLight {

private:
	glm::vec3 Ldir; 
	glm::vec3 La;   
	glm::vec3 Ld;   
	glm::vec3 Ls;  

public:
	CGLight();
	void SetLightDirection(glm::vec3 d);
	void SetAmbientLight(glm::vec3 a);
	void SetDifusseLight(glm::vec3 d);
	void SetSpecularLight(glm::vec3 s);
	glm::vec3 GetLightDirection();
	void SetUniforms(CGShaderProgram* program);
};
