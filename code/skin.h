#pragma once
#include <vector>
#include <string>
#include "vector3.h"
#include "matrix34.h"
#include "token.h"
#include "vertex.h"
#include "triangle.h"
#include "skeleton.h"
using namespace std;
class Skin
{
public:
	Skin();
	~Skin();
	bool load(const char*filename);
	void draw();
	int numOfVertex;
	int numOfTriangle;
	int numOfBindings;
	void update(vector <joint*> &joints);
	vector<Vertex*> vertexs;
	vector<triangle*> triangles;
	vector<Matrix34> bindings;
};

