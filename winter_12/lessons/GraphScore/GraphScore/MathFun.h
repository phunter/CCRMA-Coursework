//
//  MathFun.h
//  GraphScore
//
//  Created by Hunter McCurry on 2/4/12.
//  Copyright (c) 2012 Oberlin College. All rights reserved.
//

#ifndef GraphScore_MathFun_h
#define GraphScore_MathFun_h

#include <math.h>


float easeRamp(float input) {
    return pow(input,3)*(3 - 2*input);
}

float easeRampReverse(float input) {
    return 2*pow(input,3) - 3*pow(input,2) + 1;
}

float easeBump(float input) {
    return pow(input,3) - 2*pow(input,2) + input;
}

float easeBumpReverse(float input) {
    return pow(input,2) - pow(input,3);
}

bool maybe() {
    float rand_num = (float)rand()/RAND_MAX;
    if (rand_num > .5) {
        return true;
    }
    else return false;
}

#endif
