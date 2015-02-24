#include "token.h"
#include "vertex.h"
#pragma once
class Morph
{
public:
	Morph();
	~Morph();
	bool load(const char*filename,std::vector<Vertex*> &verts);
	int num;
};

