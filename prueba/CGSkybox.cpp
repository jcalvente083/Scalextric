#include "CGSkybox.h"
#include <GL/glew.h>
#include <FreeImage.h>
#include <string>
#include <iostream>
#include "CGFigure.h"


CGSkybox::CGSkybox(const char* folder)
{
	InitCubemap(folder);
	InitCube();
}

CGSkybox::~CGSkybox()
{
	glDeleteBuffers(2, VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(1, &cubemap);
}


void CGSkybox::InitCube()
{
	GLfloat vertices[12] = {
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f
	};

	GLushort indexes[6] = {
		0,1,2,
		0,2,3
	};

	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	
	glGenBuffers(2, VBO);

	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_DATA]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_DATA]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, indexes, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_DATA]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}


void CGSkybox::InitCubemap(const char* folder)
{
	std::string path = folder;

	glActiveTexture(GL_TEXTURE1);

	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	InitTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, (path + "posx.jpg").c_str());
	InitTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, (path + "negx.jpg").c_str());
	InitTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, (path + "posy.jpg").c_str());
	InitTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, (path + "negy.jpg").c_str());
	InitTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, (path + "posz.jpg").c_str());
	InitTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, (path + "negz.jpg").c_str());

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}


void CGSkybox::InitTexture(GLuint target, const char* filename)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
	if (format == FIF_UNKNOWN)
		format = FreeImage_GetFIFFromFilename(filename);
	FIBITMAP* bitmap = NULL;
	if (format != FIF_UNKNOWN)
		bitmap = FreeImage_Load(format, filename, 0);
	if (bitmap == NULL)
	{
		std::cout << "No se puede cargar la imagen del skybox: " << filename << std::endl;
		return;
	}
	FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
	FreeImage_Unload(bitmap);
	FreeImage_FlipVertical(pImage);
	int nWidth = FreeImage_GetWidth(pImage);
	int nHeight = FreeImage_GetHeight(pImage);

	glTexImage2D(target, 0, GL_RGBA8, nWidth, nHeight,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

	FreeImage_Unload(pImage);
}


void CGSkybox::Draw(CGShaderProgram* program, glm::mat4 projection, glm::mat4 view)
{
	glm::mat3 rot3 = glm::mat3(view); 
	glm::mat4 rot4 = glm::mat4(rot3);
	glm::mat4 mvp = projection * rot4; 
	glm::mat4 inv = glm::inverse(mvp); 

	program->SetUniformMatrix4("Inverse", inv);
	program->SetUniformI("CubemapTex", 0);

	glDepthMask(GL_FALSE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
	glDepthMask(GL_TRUE);
}
