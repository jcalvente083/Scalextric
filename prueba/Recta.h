#pragma once

#include "Pista.h"

class Recta : public Pista {
public:
	Recta(float l);
	glm::mat4 Salida();
	float Longitud() { return largo; }
	float LongitudRanura(int carril) { return largo; }
	glm::mat4 PoseRanura(int carril, float s);

private:
	float largo;
};
