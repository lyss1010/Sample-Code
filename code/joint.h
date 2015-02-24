//
//  joint.h
//  cse169
//
//  Created by yang liu on 1/7/15.
//
//

#ifndef cse169_joint_h
#define cse169_joint_h
#include "token.h"
#include <vector>
#include <string>
#include "vector3.h"
#include "matrix34.h"
#include "DOF.h"
#include "Pose.h"
using namespace std;
class joint{
public:
    joint();
    ~joint();
   // jointData data;
    joint* parent;
    std::vector<joint*> childrenList; 
	bool load(Tokenizer &token, vector <joint*> &joints);
    void addChild(joint* child);
    void addParent(joint* parent);
    Matrix34 LocalMat;
	Matrix34 worldMat;
	Matrix34 getWorldMat();
    std::string name;
    Vector3 offset;
    Vector3 boxmin;
    Vector3 boxmax;
    float rotxlimit[2];
    float rotylimit[2];
    float rotzlimit[2];
    Vector3 pose;
	void update(Matrix34 &parent, GLUI* glui);
    void draw();
    DOF dof[3];
	bool initial;
};
