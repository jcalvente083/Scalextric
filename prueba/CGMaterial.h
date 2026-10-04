#pragma once

#include <glm/glm.hpp>
#include "CGShaderProgram.h"

class CGMaterial {

private:
	glm::vec3 Ka;        
	glm::vec3 Kd;        
	glm::vec3 Ks;       
	GLfloat Shininess;  
	GLfloat Dissolved;  
	GLuint textureId;    

public:
	CGMaterial();
	void SetAmbientReflect(GLfloat r, GLfloat g, GLfloat b);
	void SetDifusseReflect(GLfloat r, GLfloat g, GLfloat b);
	void SetSpecularReflect(GLfloat r, GLfloat g, GLfloat b);
	void SetShininess(GLfloat f);
	void SetDissolved(GLfloat d);
	GLfloat GetDissolved();
	void SetUniforms(CGShaderProgram* program);
	void SetTexture(GLuint id);
	void InitTexture(const char* filename);
	GLuint GetTexture();
};
