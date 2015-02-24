#pragma once
#include <string>
using namespace std;
class Keyframe
{
private:
public:
	Keyframe();
	~Keyframe();
	float Time;
	float Value;
	float TangentIn, TangentOut;
	float A, B, C, D; // Cubic coefficients
	string RuleIn, RuleOut; // Tangent rules
};

