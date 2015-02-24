#pragma once
#include "Keyframe.h"
#include "token.h"
#include <vector>
using namespace std;
class Channel
{
private:
	float beginTime, lastTime;
	float beginValue, lastValue;
public:
	Channel();
	~Channel();
	string ep0;
	string ep1;
	float Evaluate(float time);
	bool Load(Tokenizer &token);
	vector<Keyframe*> keyframes;
	void Precompute();
};

