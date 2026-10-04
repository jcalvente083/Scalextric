#pragma once

#include <vector>
#include "CGShaderProgram.h"
#include "CGMaterial.h"
#include "Pista.h"

struct Tramo {
	Pista* pista;
	glm::mat4 modelo;
	float inicio[2];
	float inicioCentro;  
};


class Circuito {
public:
	Circuito();
	~Circuito();

	
	void Draw(CGShaderProgram* program, glm::mat4 projection, glm::mat4 view, glm::mat4 shadowMatrix);

	
	float LongitudCarril(int carril) { return total[carril]; }

	glm::mat4 Pose(int carril, float s);

private:
	std::vector<Tramo> tramos;
	float total[2];
	float longitudCentro;  
	glm::mat4 actual;      

	CGMaterial* matRecta;
	CGMaterial* matSalida;
	CGMaterial* matCurvaInterior;
	CGMaterial* matCurvaEstandar;
	CGMaterial* matCurvaExterior;

	CGMaterial* CrearMaterial(const char* fichero);
	void Agregar(Pista* p, CGMaterial* material);
	void CrearCircuito();
	void CerrarCircuito();
};
