//
//  Dissonance.h
//  GraphScore
//
//  Created by Hunter McCurry on 2/7/12.
//  Copyright (c) 2012 Hunter McCurry. All rights reserved.
//

#ifndef GraphScore_Dissonance_h
#define GraphScore_Dissonance_h

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


class Dissonance {
public:
    Dissonance(std::string diss_matrix);
    void Parse(std::string diss_matrix);
  
    void Set(int c, int r, float val);
    float Get(int c, int r);
    
private:
    // column major matrix
    float data[48][48];
    
};


#endif
