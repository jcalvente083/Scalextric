#pragma once

#include "Pista.h"


class Curva : public Pista {
public:
	Curva(float radioInterior, float radioExterior, float anguloGrados, bool izq);
	glm::mat4 Salida();
	float Longitud() { return radioCentral * angulo; }
	float LongitudRanura(int carril);
	glm::mat4 PoseRanura(int carril, float s);

private:
	float radioCentral;
	float angulo;       
	bool izquierda;

	
	float Radio(float d);

	glm::vec2 Punto(float d, float t);
};
