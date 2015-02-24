//
//  skeleton.h
//  cse169
//
//  Created by yang liu on 1/7/15.
//
//

#ifndef cse169_skeleton_h
#define cse169_skeleton_h
#include "token.h"
#include <vector>
#include <string>
#include "vector3.h"
#include "matrix34.h"
#include "DOF.h"
#include "Pose.h"
using namespace std;

class Skeleton{
private:
public:
	std::vector <joint*> joints;
    Skeleton();
    joint* root;
	void update(GLUI* glui);
	bool load(const char*filename);
    void draw();
	void setPose(Pose p);
};

#endif
