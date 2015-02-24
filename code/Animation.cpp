#include "Animation.h"
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>

using namespace std;

Animation::Animation()
{
}


Animation::~Animation()
{
}

bool Animation::load(const char*filename){
	char myArr[256];
	Tokenizer token;
	token.Open(filename);
	token.FindToken("range");
	range[0]=token.GetFloat();
	range[1] = token.GetFloat(); 
	token.FindToken("numchannels");
	numchannels = token.GetInt(); 
	for (int i = 0; i < numchannels; i++){
		token.FindToken("channel");
		token.FindToken("{");
		Channel *myChannel = new Channel();
		token.FindToken("extrapolate");
		token.GetToken(myArr);
		myChannel->ep0 = myArr;
		token.GetToken(myArr);
		myChannel->ep1 = myArr;
		
		token.FindToken("keys");
		int numKeyframes;
		numKeyframes = token.GetInt();
		token.FindToken("{");
		for (int j = 0; j < numKeyframes; j++){
			Keyframe *myKeyframe = new Keyframe();
			myKeyframe->Time = token.GetFloat();
			myKeyframe->Value = token.GetFloat();
			token.GetToken(myArr);
			myKeyframe->RuleIn = myArr;
			token.GetToken(myArr);
			myKeyframe->RuleOut = myArr;
			myChannel->keyframes.push_back(myKeyframe);
		}
		token.FindToken("}");
		channels.push_back(myChannel);
	}
	token.Close();
	return true;
}

void Animation::Evaluate(float time,Pose &p){
	if (firstPass){
		for (int i = 0; i < numchannels; i++){
			channels[i]->Precompute();
		}
		firstPass = false;
	}
	for (int i = 0; i < numchannels; i++){
		p.poseArr[i] = channels[i]->Evaluate(time);
	}
}