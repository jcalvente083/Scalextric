#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "CGShaderProgram.h"
#include "CGScene.h"
#include "CGSkybox.h"
#include "CGCamera.h"


const float DIST_CAMARA = 300.0f;     
const float ALTURA_CAMARA = 90.0f;    
const float DIST_MIRADA = 150.0f;     
const float ALTURA_MIRADA = 10.0f;    


const GLsizei TAM_SOMBRA = 4096;

class CGModel
{
public:
	void initialize(int w, int h);
	void finalize();
	void render();
	void update();
	void key_pressed(int key);
	void mouse_button(int button, int action);
	void mouse_move(double xpos, double ypos);
	void resize(int w, int h);

private:
	CGShaderProgram* sceneProgram;
	CGShaderProgram* shadowProgram;
	CGShaderProgram* skyboxProgram;
	CGScene* scene;
	CGCamera* camera;
	CGSkybox* skybox;
	glm::mat4 projection;

	GLsizei wndWidth;
	GLsizei wndHeight;
	GLuint shadowFBO;
	GLuint depthTexId;

	int modoCamara;       
	glm::vec3 target;    
	glm::vec3 eyeFija;     
	bool listo;           

	bool InitShadowMap();
	void ActualizarProyeccion();
	void CalcularCamaraFija();
	void ActualizarCamara();
};
