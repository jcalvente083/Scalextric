#include "CGModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "CGCamera.h"
#include "CGScene.h"
#include "CGSkybox.h"

const char* RUTA_SKYBOX = "textures/Skyboxes/Meadow/";


void CGModel::initialize(int w, int h)
{
	listo = false;
	sceneProgram = NULL;
	shadowProgram = NULL;
	skyboxProgram = NULL;
	camera = NULL;
	skybox = NULL;
	scene = NULL;
	wndWidth = w;
	wndHeight = h;
	modoCamara = 1;

	skyboxProgram = new CGShaderProgram("shaders/SkyboxVertexShader.glsl",
		"shaders/SkyboxFragmentShader.glsl", NULL, NULL, NULL);
	if (skyboxProgram->IsLinked() == GL_FALSE) return;

	shadowProgram = new CGShaderProgram("shaders/ShadowVertexShader.glsl",
		"shaders/ShadowFragmentShader.glsl", NULL, NULL, NULL);
	if (shadowProgram->IsLinked() == GL_FALSE) return;

	sceneProgram = new CGShaderProgram("shaders/VertexShader.glsl",
		"shaders/FragmentShader.glsl", NULL, NULL, NULL);
	if (sceneProgram->IsLinked() == GL_FALSE) return;

	camera = new CGCamera();

	skybox = new CGSkybox(RUTA_SKYBOX);

	scene = new CGScene();

	bool frameBufferStatus = InitShadowMap();
	if (!frameBufferStatus) return;

	resize(w, h);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	listo = true;
}


bool CGModel::InitShadowMap()
{
	GLfloat border[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLsizei shadowMapWidth = TAM_SOMBRA;
	GLsizei shadowMapHeight = TAM_SOMBRA;

	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glGenTextures(1, &depthTexId);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowMapWidth,
		shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, depthTexId, 0);

	glDrawBuffer(GL_NONE);

	bool result = true;
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		result = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return result;
}

void CGModel::finalize()
{
	if (listo)
	{
		glDeleteFramebuffers(1, &shadowFBO);
		glDeleteTextures(1, &depthTexId);
	}
	delete camera;
	delete scene;
	delete skybox;
	delete sceneProgram;
	delete shadowProgram;
	delete skyboxProgram;
}

void CGModel::resize(int w, int h)
{
	wndWidth = w;
	wndHeight = h;
	if (h == 0) wndHeight = 1;

	glViewport(0, 0, wndWidth, wndHeight);
	ActualizarProyeccion();

	if (scene != NULL)
		CalcularCamaraFija();
}


void CGModel::ActualizarProyeccion()
{
	GLfloat cerca = 10.0f;
	GLfloat lejos = 20000.0f;
	if (modoCamara == 1)
	{
		cerca = 500.0f;
		lejos = 30000.0f;
	}

	double fov = glm::radians(22.5);   
	double sin_fov = sin(fov);
	double cos_fov = cos(fov);
	GLfloat aspectRatio = (GLfloat)wndWidth / (GLfloat)wndHeight;
	GLfloat wHeight = (GLfloat)(sin_fov * cerca / cos_fov);
	GLfloat wWidth = wHeight * aspectRatio;

	projection = glm::frustum(-wWidth, wWidth, -wHeight, wHeight, cerca, lejos);
}

void CGModel::CalcularCamaraFija()
{
	std::vector<glm::vec3>& puntos = scene->GetPoints();

	glm::vec3 minimo = glm::vec3(1e9f);
	glm::vec3 maximo = glm::vec3(-1e9f);
	for (int i = 0; i < (int)puntos.size(); i++)
	{
		minimo = glm::min(minimo, puntos[i]);
		maximo = glm::max(maximo, puntos[i]);
	}
	target = 0.5f * (minimo + maximo);

	float inclinacion = 0.9f;
	GLfloat aspectRatio = (GLfloat)wndWidth / (GLfloat)wndHeight;
	GLfloat wHeight = (GLfloat)(500.0 * tan(glm::radians(22.5)));
	glm::mat4 proj = glm::frustum(-wHeight * aspectRatio, wHeight * aspectRatio, -wHeight, wHeight, 500.0f, 30000.0f);
	float mejorDistancia = 1e9f;
	eyeFija = target + glm::vec3(0.0f, 9000.0f, 0.0f);   

	for (int k = 0; k < 24; k++)
	{
		float giro = 6.2831853f * k / 24.0f;
		glm::vec3 direccion = glm::vec3(std::cos(inclinacion) * std::sin(giro),
			std::sin(inclinacion),
			std::cos(inclinacion) * std::cos(giro));

		for (float dist = 1000.0f; dist < 30000.0f; dist = dist + 100.0f)
		{
			glm::vec3 eye = target + dist * direccion;
			glm::mat4 viewProj = proj * glm::lookAt(eye, target, glm::vec3(0.0f, 1.0f, 0.0f));

			bool caben = true;
			for (int i = 0; i < (int)puntos.size() && caben; i++)
			{
				glm::vec4 p = viewProj * glm::vec4(puntos[i], 1.0f);
				if (p.w <= 0.0f || std::fabs(p.x / p.w) > 0.88f || std::fabs(p.y / p.w) > 0.88f)
					caben = false;
			}

			if (caben)
			{
				if (dist < mejorDistancia)
				{
					mejorDistancia = dist;
					eyeFija = eye;
				}
				break;
			}
		}
	}
}

void CGModel::ActualizarCamara()
{
	glm::vec3 eye;
	glm::vec3 mira;
	if (modoCamara == 1)
	{
		eye = eyeFija;
		mira = target;
	}
	else
	{
		
		int i = modoCamara - 2;
		glm::mat4 poseCoche = scene->GetCarPose(i);
		glm::vec3 posicion = glm::vec3(poseCoche[3]);
		glm::vec3 adelante = glm::normalize(glm::vec3(poseCoche[0]));
		eye = posicion - DIST_CAMARA * adelante + glm::vec3(0.0f, ALTURA_CAMARA, 0.0f);
		mira = posicion + DIST_MIRADA * adelante + glm::vec3(0.0f, ALTURA_MIRADA, 0.0f);
	}

	glm::vec3 dir = eye - mira;
	camera->SetPosition(eye.x, eye.y, eye.z);
	camera->SetDirection(dir.x, dir.y, dir.z, 0.0f, 1.0f, 0.0f);
}


void CGModel::render()
{
	if (!listo)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	shadowProgram->Use();

	glm::mat4 lightViewMatrix = scene->GetLightViewMatrix();
	glm::mat4 lightPerspective = scene->GetLightProjectionMatrix();
	glm::mat4 lightMVP = lightPerspective * lightViewMatrix;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);

	glViewport(0, 0, TAM_SOMBRA, TAM_SOMBRA);

	scene->DrawShadow(shadowProgram, lightMVP);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_BACK);

	glViewport(0, 0, wndWidth, wndHeight);

	ActualizarCamara();
	glm::mat4 view = camera->ViewMatrix();
	skyboxProgram->Use();
	skybox->Draw(skyboxProgram, projection, view);

	
	sceneProgram->Use();
	sceneProgram->SetUniformI("ShadowMap", 1);

	scene->Draw(sceneProgram, projection, view, lightMVP);
}


void CGModel::update()
{
	if (listo)
		scene->Update();
}

void CGModel::key_pressed(int key)
{
	if (!listo)
		return;

	switch (key)
	{
	case GLFW_KEY_F1:
		modoCamara = 1;
		ActualizarProyeccion();
		break;
	case GLFW_KEY_F2:
		modoCamara = 2;
		ActualizarProyeccion();
		break;
	case GLFW_KEY_F3:
		modoCamara = 3;
		ActualizarProyeccion();
		break;

	case GLFW_KEY_Q:
		scene->Accelerate(0);
		break;
	case GLFW_KEY_A:
		scene->Brake(0);
		break;
	case GLFW_KEY_O:
		scene->Accelerate(1);
		break;
	case GLFW_KEY_L:
		scene->Brake(1);
		break;
	}
}

void CGModel::mouse_button(int button, int action)
{
}


void CGModel::mouse_move(double xpos, double ypos)
{
}
