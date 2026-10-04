#pragma once

#include <vector>
#include <string>
#include "CGObject.h"
#include "CGPiece.h"
#include "CGMaterial.h"

class PiezaCoche : public CGPiece {
public:
	PiezaCoche(std::vector<float>& v, std::vector<float>& n, std::vector<float>& t, std::vector<unsigned short>& idx);
};


class Coche : public CGObject {
public:
	Coche();
	~Coche();

	bool Cargar(std::string carpeta);

	int GetNumPieces();
	CGPiece* GetPiece(int i);

private:
	std::vector<CGPiece*> piezas;
	std::vector<CGMaterial*> materiales;
};
