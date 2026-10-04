#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>


class CGCamera {
public:
	CGCamera();
	glm::mat4 ViewMatrix();

	void SetPosition(GLfloat x, GLfloat y, GLfloat z);
	void SetDirection(GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat xUp, GLfloat yUp, GLfloat zUp);

	glm::vec3 GetPosition();
	glm::vec3 GetDirection();
	glm::vec3 GetUpDirection();

private:
	glm::vec3 Pos;
	glm::vec3 Dir;
	glm::vec3 Up;
	glm::vec3 Right;
};
