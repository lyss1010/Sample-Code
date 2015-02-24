#include "AnimationPlayer.h"
#include <iostream>
AnimationPlayer::AnimationPlayer(){
	Time = 0;
}
AnimationPlayer::~AnimationPlayer(){
}
void AnimationPlayer::SetClip(Animation &clip){
	Anim = &clip;
}

const Pose& AnimationPlayer::GetPose(){
	return p;
}
void AnimationPlayer::Update(){
	Anim->Evaluate(Time, p);
	Time = Time + 0.01;
	if (Time > 4){
		Time = 0;
	}
}