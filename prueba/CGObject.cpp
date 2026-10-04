#include "CGObject.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


CGObject::CGObject()
{
	model = glm::mat4(1.0f);
}


void CGObject::ResetLocation()
{
	model = glm::mat4(1.0f);
}


void CGObject::SetLocation(glm::mat4 loc)
{
	model = loc;
}


glm::mat4 CGObject::GetLocation()
{
	return model;
}


void CGObject::Translate(glm::vec3 t)
{
	model = glm::translate(model, t);
}

void CGObject::Rotate(GLfloat angle, glm::vec3 axis)
{
	model = glm::rotate(model, glm::radians(angle), axis);
}

void CGObject::Draw(CGShaderProgram* program, glm::mat4 projection, glm::mat4 view, glm::mat4 shadowMatrix)
{
	int num = GetNumPieces();
	for (int pasada = 0; pasada < 2; pasada++)
	{
		bool translucidas = (pasada == 1);
		if (translucidas)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(GL_FALSE);
		}
		for (int i = 0; i < num; i++)
		{
			CGPiece* piece = GetPiece(i);
			bool esTranslucida = (piece->GetMaterial() != NULL && piece->GetMaterial()->GetDissolved() < 1.0f);
			if (esTranslucida == translucidas)
				piece->Draw(program, projection, view, model, shadowMatrix);
		}
	}
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void CGObject::DrawShadow(CGShaderProgram* program, glm::mat4 shadowMatrix)
{
	int num = GetNumPieces();
	for (int i = 0; i < num; i++)
	{
		CGPiece* piece = GetPiece(i);
		bool esTranslucida = (piece->GetMaterial() != NULL && piece->GetMaterial()->GetDissolved() < 1.0f);
		if (!esTranslucida)
			piece->DrawShadow(program, shadowMatrix, model);
	}
}
