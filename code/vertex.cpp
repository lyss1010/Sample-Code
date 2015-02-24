#include "vertex.h"


Vertex::Vertex()
{
	texcoord[0] = -100;
	texcoord[1] = -100;
}


Vertex::~Vertex()
{
}

void Vertex::update(vector<Matrix34> bindings, vector <joint*> &joints) {
	//update position
	newPosition.Set(0,0,0);
	for (size_t i = 0; i < numOfBindings; i++){            //newPosition += w(i)*World(i)*bindings(i)*position
		Vector3 tempPosition;
		Matrix34 worldMat;
		worldMat = (joints[jointIndex[i]]->getWorldMat());
		worldMat.Dot(worldMat, bindings[jointIndex[i]]);
		worldMat.Transform(position, tempPosition);
		tempPosition=tempPosition*jointWeight[i];
		newPosition = newPosition + tempPosition;
 	}

	//update normal
	newNormal.Set(0, 0, 0);
	if (numOfBindings <= 1){
		Matrix34 worldMat;
		worldMat = (joints[jointIndex[0]]->getWorldMat());
		worldMat.Transform3x3(normal, newNormal);
	}
	else{
		for (size_t i = 0; i < numOfBindings; i++){            //newNormal += w(i)*World(i)*bindings(i)*normal
			Vector3 tempNormal;
			Matrix34 worldMat;
			worldMat = (joints[jointIndex[i]]->getWorldMat());
			worldMat.Dot(worldMat, bindings[jointIndex[i]]);
			worldMat.Transform3x3(normal, tempNormal);
			tempNormal = tempNormal*jointWeight[i];
			newNormal = newNormal + tempNormal;
		}
	}
	//normalize normal
	newNormal.Normalize();
	
}
	