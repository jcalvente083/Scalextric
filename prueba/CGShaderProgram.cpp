#include "CGShaderProgram.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>


CGShaderProgram::CGShaderProgram(const char* vs, const char* fs, const char* gs, const char* tcs, const char* tes)
{
	vertexShader = NO_SHADER;
	fragmentShader = NO_SHADER;
	geometryShader = NO_SHADER;
	tessControlShader = NO_SHADER;
	tessEvaluationShader = NO_SHADER;
	linked = GL_FALSE;


	if (vs != NULL) vertexShader = CreateShader(GL_VERTEX_SHADER, vs);
	if (fs != NULL) fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fs);
	if (gs != NULL) geometryShader = CreateShader(GL_GEOMETRY_SHADER, gs);
	if (tcs != NULL) tessControlShader = CreateShader(GL_TESS_CONTROL_SHADER, tcs);
	if (tes != NULL) tessEvaluationShader = CreateShader(GL_TESS_EVALUATION_SHADER, tes);

	
	program = glCreateProgram();
	if (vertexShader != NO_SHADER) glAttachShader(program, vertexShader);
	if (fragmentShader != NO_SHADER) glAttachShader(program, fragmentShader);
	if (geometryShader != NO_SHADER) glAttachShader(program, geometryShader);
	if (tessControlShader != NO_SHADER) glAttachShader(program, tessControlShader);
	if (tessEvaluationShader != NO_SHADER) glAttachShader(program, tessEvaluationShader);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		linked = GL_FALSE;
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		char* logInfo = (char*)malloc(sizeof(char) * (logLength + 1));
		GLsizei written;
		glGetProgramInfoLog(program, logLength, &written, logInfo);
		std::cout << logInfo << std::endl;
		free(logInfo);
		return;
	}
	linked = GL_TRUE;
}


GLuint CGShaderProgram::CreateShader(int mode, const char* filename)
{
	GLint status;
	char* code = GetShaderCodeFromFile(filename);
	if (code == NULL)
	{
		std::cout << "No se puede abrir el fichero de shader: " << filename << std::endl;
		return NO_SHADER;
	}

	GLuint shader = glCreateShader(mode);
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);
	free(code);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		char* logInfo = (char*)malloc(sizeof(char) * (logLength + 1));
		GLsizei written;
		glGetShaderInfoLog(shader, logLength, &written, logInfo);
		std::cout << filename << ": " << logInfo << std::endl;
		free(logInfo);
		return NO_SHADER;
	}
	return shader;
}


char* CGShaderProgram::GetShaderCodeFromFile(const char* filename)
{
	std::ifstream file(filename);
	if (!file)
		return NULL;

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string text = buffer.str();

	char* code = (char*)malloc(sizeof(char) * (text.size() + 1));
	memcpy(code, text.c_str(), text.size());
	code[text.size()] = '\0';
	return code;
}

CGShaderProgram::~CGShaderProgram()
{
	if (vertexShader != NO_SHADER) glDeleteShader(vertexShader);
	if (fragmentShader != NO_SHADER) glDeleteShader(fragmentShader);
	if (geometryShader != NO_SHADER) glDeleteShader(geometryShader);
	if (tessControlShader != NO_SHADER) glDeleteShader(tessControlShader);
	if (tessEvaluationShader != NO_SHADER) glDeleteShader(tessEvaluationShader);
	glDeleteProgram(program);
}

GLboolean CGShaderProgram::IsLinked()
{
	return linked;
}

GLvoid CGShaderProgram::Use()
{
	glUseProgram(program);
}

void CGShaderProgram::SetUniformF(const char* name, GLfloat f)
{
	GLint location = glGetUniformLocation(program, name);
	if (location >= 0) glUniform1f(location, f);
}

GLvoid CGShaderProgram::SetUniformMatrix4(const char* name, glm::mat4 m)
{
	GLint location = glGetUniformLocation(program, name);
	if (location >= 0) glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]);
}

void CGShaderProgram::SetUniformVec4(const char* name, glm::vec4 v)
{
	GLint location = glGetUniformLocation(program, name);
	if (location >= 0) glUniform4fv(location, 1, &v[0]);
}

void CGShaderProgram::SetUniformVec3(const char* name, glm::vec3 v)
{
	GLint location = glGetUniformLocation(program, name);
	if (location >= 0) glUniform3fv(location, 1, &v[0]);
}

void CGShaderProgram::SetUniformI(const char* name, GLint i)
{
	GLint location = glGetUniformLocation(program, name);
	if (location >= 0) glUniform1i(location, i);
}
