#pragma once
#include "Channel.h"
#include <vector>
#include "vector3.h"
#include "Pose.h"
using namespace std;
class Animation
{
private:
	bool firstPass = true;
public:
	float range[2];
	int numchannels;
	Animation();
	~Animation();
	bool load(const char*filename);
	void Evaluate(float time,Pose &p);
	vector<Channel*> channels;
};

