#pragma once
#include "vertex.h"
class triangle
{
public:
	triangle();
	~triangle();
	Vertex *tri[3];
	void draw();
};

