#pragma once
#include "Curva.h"


class CurvaInterior : public Curva {
public:
	CurvaInterior(bool izq) : Curva(58.0f, 214.0f, 45.0f, izq) {}
};
