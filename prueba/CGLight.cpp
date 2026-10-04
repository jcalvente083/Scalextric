#include "CGLight.h"
#include <glm/glm.hpp>

CGLight::CGLight()
{
	Ldir = glm::vec3(0.0f, 0.0f, -1.0f);
	La = glm::vec3(1.0f, 1.0f, 1.0f);
	Ld = glm::vec3(1.0f, 1.0f, 1.0f);
	Ls = glm::vec3(0.0f, 0.0f, 0.0f);
}

void CGLight::SetLightDirection(glm::vec3 d)
{
	Ldir = d;
}

void CGLight::SetAmbientLight(glm::vec3 a)
{
	La = a;
}

void CGLight::SetDifusseLight(glm::vec3 d)
{
	Ld = d;
}

void CGLight::SetSpecularLight(glm::vec3 s)
{
	Ls = s;
}

glm::vec3 CGLight::GetLightDirection()
{
	return Ldir;
}

void CGLight::SetUniforms(CGShaderProgram* program)
{
	program->SetUniformVec3("Light.Ldir", Ldir);
	program->SetUniformVec3("Light.La", La);
	program->SetUniformVec3("Light.Ld", Ld);
	program->SetUniformVec3("Light.Ls", Ls);
}
