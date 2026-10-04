#pragma once
#include "Curva.h"

class CurvaEstandar : public Curva {
public:
	CurvaEstandar(bool izq) : Curva(214.0f, 370.0f, 45.0f, izq) {}
};
