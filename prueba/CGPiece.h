#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "CGMaterial.h"
#include "CGShaderProgram.h"

#define VERTEX_DATA 0
#define INDEX_DATA 1
#define NORMAL_DATA 2
#define TEXTURE_DATA 3

class CGPiece {
protected:
	GLushort* indexes;
	GLfloat* vertices; 
	GLfloat* normals;  
	GLfloat* textures; 

	GLuint numFaces;   
	GLuint numVertices; 
	GLuint VBO[4];
	GLuint VAO;

	glm::mat4 location; 
	CGMaterial* material;

public:
	CGPiece();
	virtual ~CGPiece();
	void InitBuffers();
	void SetMaterial(CGMaterial* m);
	CGMaterial* GetMaterial();
	void SetLocation(glm::mat4 loc);
	glm::mat4 GetLocation();
	void Translate(glm::vec3 t);
	void Rotate(GLfloat angle, glm::vec3 axis);
	void Draw(CGShaderProgram* program, glm::mat4 projection,
		glm::mat4 view, glm::mat4 model, glm::mat4 shadowMatrix);
	void DrawShadow(CGShaderProgram* program, glm::mat4 shadowMatrix, glm::mat4 model);
};
