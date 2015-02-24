#pragma once
#include "vector3.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "matrix34.h"
#include "skeleton.h"
using namespace std;
class Vertex
{
public:
	Vertex();
	~Vertex();
	Vector3 position;
	Vector3 normal;
	Vector3 newPosition;
	Vector3 newNormal;
	float texcoord[2];
	int numOfBindings;
	vector <int> jointIndex;
	vector <float> jointWeight;
	void update(vector<Matrix34> bindings, vector <joint*> &joints);
};

