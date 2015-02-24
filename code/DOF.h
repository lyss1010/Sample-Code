//
//  DOF.h
//  cse169
//
//  Created by yang liu on 1/20/15.
//
//

#ifndef DOF_H
#define DOF_H

#include "vector3.h"
#include "matrix34.h"

class DOF {
private:
public:
	float min, max;
    DOF();
    ~DOF();
	float val;
    float get();
    void setMinMax(float m, float n);
     void set(float v);
};

#endif