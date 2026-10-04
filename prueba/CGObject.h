#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "CGShaderProgram.h"
#include "CGPiece.h"


class CGObject {
protected:
	glm::mat4 model; 

public:
	CGObject();
	virtual ~CGObject() {}
	void ResetLocation();
	void Translate(glm::vec3 t);
	void Rotate(GLfloat angle, glm::vec3 axis);
	void SetLocation(glm::mat4 loc);
	glm::mat4 GetLocation();
	void Draw(CGShaderProgram* program, glm::mat4 projection, glm::mat4 view, glm::mat4 shadowMatrix);
	void DrawShadow(CGShaderProgram* program, glm::mat4 shadowMatrix);

	virtual int GetNumPieces() = 0;
	virtual CGPiece* GetPiece(int i) = 0;
};
