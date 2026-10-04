#pragma once
#include "Curva.h"

class CurvaExterior : public Curva {
public:
	CurvaExterior(bool izq) : Curva(370.0f, 526.0f, 22.5f, izq) {}
};
