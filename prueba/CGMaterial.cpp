#include "CGMaterial.h"
#include <GL/glew.h>
#include <FreeImage.h>
#include <iostream>


CGMaterial::CGMaterial()
{
	Ka = glm::vec3(1.0f, 1.0f, 1.0f);
	Kd = glm::vec3(1.0f, 1.0f, 1.0f);
	Ks = glm::vec3(0.8f, 0.8f, 0.8f);
	Shininess = 16.0f;
	Dissolved = 1.0f;
	textureId = 0;
}


void CGMaterial::SetAmbientReflect(GLfloat r, GLfloat g, GLfloat b)
{
	Ka = glm::vec3(r, g, b);
}


void CGMaterial::SetDifusseReflect(GLfloat r, GLfloat g, GLfloat b)
{
	Kd = glm::vec3(r, g, b);
}


void CGMaterial::SetSpecularReflect(GLfloat r, GLfloat g, GLfloat b)
{
	Ks = glm::vec3(r, g, b);
}

void CGMaterial::SetShininess(GLfloat f)
{
	Shininess = f;
}


void CGMaterial::SetDissolved(GLfloat d)
{
	Dissolved = d;
}


GLfloat CGMaterial::GetDissolved()
{
	return Dissolved;
}

void CGMaterial::SetUniforms(CGShaderProgram* program)
{
	program->SetUniformVec3("Material.Ka", Ka);
	program->SetUniformVec3("Material.Kd", Kd);
	program->SetUniformVec3("Material.Ks", Ks);
	program->SetUniformF("Material.Shininess", Shininess);
	program->SetUniformF("Material.Dissolved", Dissolved);
	program->SetUniformI("BaseTex", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void CGMaterial::SetTexture(GLuint id)
{
	textureId = id;
}


GLuint CGMaterial::GetTexture()
{
	return textureId;
}


void CGMaterial::InitTexture(const char* filename)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
	if (format == FIF_UNKNOWN)
		format = FreeImage_GetFIFFromFilename(filename);
	FIBITMAP* bitmap = NULL;
	if (format != FIF_UNKNOWN)
		bitmap = FreeImage_Load(format, filename, 0);
	if (bitmap == NULL)
	{
		std::cout << "No se puede cargar la textura: " << filename << std::endl;
		return;
	}
	FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
	FreeImage_Unload(bitmap);
	int nWidth = FreeImage_GetWidth(pImage);
	int nHeight = FreeImage_GetHeight(pImage);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (GLEW_EXT_texture_filter_anisotropic)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
	glGenerateMipmap(GL_TEXTURE_2D);

	FreeImage_Unload(pImage);
}
