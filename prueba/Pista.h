#pragma once

#include "CGFigure.h"

const float ANCHURA = 156.0f;
const float DIST_RANURA_BORDE = 39.0f;  


class Pista : public CGFigure {
public:
	
	virtual glm::mat4 Salida() = 0;

	
	virtual float Longitud() = 0;

	
	virtual float LongitudRanura(int carril) = 0;

	
	virtual glm::mat4 PoseRanura(int carril, float s) = 0;

protected:

	float DesplazamientoCarril(int carril)
	{
		float d = ANCHURA / 2.0f - DIST_RANURA_BORDE;  
		if (carril == 0)
			return -d;
		else
			return d;
	}
};
