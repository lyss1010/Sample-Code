//
//  DOF.cpp
//  cse169
//
//  Created by yang liu on 1/20/15.
//
//

#include "DOF.h"
#include <iostream>

DOF::DOF()
{
    val = 0;
    min = -3.14;
    max = 3.14;
}

void DOF::set(float v)
{
    if (v > max)
        val = max;
    else if (v < min)
        val = min;
    else
        val = v;
}

float DOF::get()
{
    return val;
}

void DOF::setMinMax(float m, float n)
{
    min = m;
    max = n;
}

DOF::~DOF()
{}