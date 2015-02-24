//
//  skeleton.cpp
//  cse169
//
//  Created by yang liu on 1/7/15.
//
//
#include <iostream>
#include "skeleton.h"
#include "core.h"
using namespace std;
Skeleton::Skeleton(){
    root = 0;
}
void Skeleton::update( GLUI* glui){
    Matrix34 id;
    id.Identity();
	//cerr << "root has name" << root->name.c_str() << endl;
	//if (root->parent) {cerr << "root has parent"<<endl;cerr << "root parent: " << root->parent->name.c_str() << endl;}
	//else cerr << "root do not have parent"<<endl;
    root->update(id,glui);
}
bool Skeleton::load(const char* filename){
    Tokenizer token;
    token.Open(filename);
    token.FindToken("balljoint");
	std::vector<char> chars = token.getString();
	std::string s = std::string(chars.begin(), chars.end());
	
    root= new joint;
	root->name = s;
	joints.push_back(root);
    root->load(token,joints);
    token.Close();
    return true;
}
void Skeleton::draw(){
    root->draw();
}

void Skeleton::setPose(Pose p){
	Vector3 off(p.poseArr[0], p.poseArr[1], p.poseArr[2]);
	joints[0]->offset = off;
	for (int i = 0; i < joints.size(); i++){
		joints[i]->dof[0].set(p.poseArr[i*3+3]);
		joints[i]->dof[1].set(p.poseArr[i * 3+4]);
		joints[i]->dof[2].set(p.poseArr[i * 3+5]);
	}
}