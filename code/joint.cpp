//
//  joint.cpp
//  cse169
//
//  Created by yang liu on 1/19/15.
//
//

#include <stdio.h>
#include "skeleton.h"
#include <iostream>
using namespace std;
joint::joint(){
    offset.Set(0, 0, 0);
    pose.Set(0, 0, 0);
    rotxlimit[0] = -3.14;
	rotxlimit[1] = 3.14;
	rotylimit[0] = -3.14;
	rotylimit[1] = 3.14;
	rotzlimit[0] = -3.14;
	rotzlimit[1] = 3.14;
    worldMat.Identity();
	initial = true;
}
joint::~joint(){
    for (std::vector<joint*>::iterator it = childrenList.begin(); it != childrenList.end(); ++it){
        delete(*it);
    }
}
void joint::addChild(joint* child){
    this->childrenList.push_back(child);
    child->addParent(this);
}
void joint::addParent(joint* parent){
    this->parent = parent;
}
bool joint::load(Tokenizer &token, vector <joint*> &joints){
    //token.FindToken("{");
    while (1) {
        char temp[256];
        token.GetToken(temp);
        if(strcmp(temp,"offset")==0) {
            offset.x=token.GetFloat();
            offset.y=token.GetFloat();
            offset.z=token.GetFloat();
        }
        else if (strcmp(temp, "boxmin") == 0){
            boxmin.x = token.GetFloat();
            boxmin.y = token.GetFloat();
            boxmin.z = token.GetFloat();
        }
        else if (strcmp(temp, "boxmax") == 0){
            boxmax.x = token.GetFloat();
            boxmax.y = token.GetFloat();
            boxmax.z = token.GetFloat();
        }
        else if (strcmp(temp, "pose") == 0){
            pose.x= token.GetFloat();
            pose.y= token.GetFloat();
            pose.z= token.GetFloat();
            dof[0].set(pose.x);
            dof[1].set(pose.y);
            dof[2].set(pose.z);
        }
        else if (strcmp(temp, "rotxlimit") == 0){
            rotxlimit[0] = token.GetFloat();
            rotxlimit[1] = token.GetFloat();
            dof[0].setMinMax(rotxlimit[0], rotxlimit[1]);
        }
        else if (strcmp(temp, "rotylimit") == 0){
            rotylimit[0] = token.GetFloat();
            rotylimit[1] = token.GetFloat();
            dof[1].setMinMax(rotylimit[0], rotylimit[1]);
        }
        else if (strcmp(temp, "rotzlimit") == 0){
            rotzlimit[0] = token.GetFloat();
            rotzlimit[1] = token.GetFloat();
            dof[2].setMinMax(rotzlimit[0], rotzlimit[1]);
        }
        else if(strcmp(temp,"balljoint")==0) {
			std::vector<char> chars = token.getString();
			std::string s = std::string(chars.begin(), chars.end());
            joint *jnt=new joint;
			jnt->name = s;
			joints.push_back(jnt);
            jnt->load(token,joints);
            addChild(jnt);
        }
        else if(strcmp(temp,"}")==0)
            return true;
        else token.SkipLine(); // Unrecognized token
    }
}
void joint::draw(){
    float minX = boxmin.x;
    float minY = boxmin.y;
    float minZ = boxmin.z;
    float maxX = boxmax.x;
    float maxY = boxmax.y;
    float maxZ = boxmax.z;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(worldMat);
    drawWireBox(minX, minY, minZ, maxX, maxY, maxZ);
    for (std::vector<joint*>::iterator it = childrenList.begin(); it != childrenList.end(); ++it){
        (*it)->draw();
    }
}
void joint::update(Matrix34 &parent, GLUI* glui){
	if (initial){
		GLUI_Rollout *obj_panel = new GLUI_Rollout(glui, this->name.c_str(), false);
		(new GLUI_Spinner(obj_panel, "rotateX:", &this->dof[0].val))
			->set_int_limits(this->dof[0].min, this->dof[0].max);
		(new GLUI_Spinner(obj_panel, "rotateY:", &this->dof[1].val))
			->set_int_limits(this->dof[1].min, this->dof[1].max);
		(new GLUI_Spinner(obj_panel, "rotateZ:", &this->dof[2].val))
			->set_int_limits(this->dof[2].min, this->dof[2].max);
		initial = false;
	}

    Matrix34 translate;
    translate.MakeTranslate(offset);
   
    Matrix34 rotationMatX;
    rotationMatX.MakeRotateX(dof[0].get());
    
    Matrix34 rotationMatY;
	rotationMatY.MakeRotateY(dof[1].get() );
    
    Matrix34 rotationMatZ;
	rotationMatZ.MakeRotateZ(dof[2].get() );
    
    Matrix34 rotationFinal;
    rotationFinal.Dot(rotationMatY, rotationMatX);
    rotationFinal.Dot(rotationMatZ,rotationFinal);
    
    LocalMat.Dot(translate,rotationFinal);
    worldMat.Dot(parent, LocalMat);
	//worldMat.Print();
    for (std::vector<joint*>::iterator it = childrenList.begin(); it != childrenList.end(); ++it){
		(*it)->update(worldMat, glui);
    }
}


Matrix34 joint::getWorldMat(){
	return this->worldMat;
}