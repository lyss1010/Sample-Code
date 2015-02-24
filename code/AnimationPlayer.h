#pragma once
#include "Animation.h"
class AnimationPlayer {
public:
	AnimationPlayer();
	~AnimationPlayer();
	float Time;
	Animation *Anim;
	Pose p;
	void SetClip(Animation &clip);
	const Pose &GetPose();
	void Update();
};