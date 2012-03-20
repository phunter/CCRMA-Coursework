//
//  Dissonance.cpp
//  GraphScore
//
//  Created by Hunter McCurry on 2/7/12.
//  Copyright (c) 2012 Hunter McCurry. All rights reserved.
//


#include "Dissonance.h"


Dissonance::Dissonance(std::string diss_matrix)
{
	Parse(diss_matrix);
}

void Dissonance::Parse(std::string diss_matrix)
{
    
	std::ifstream diss_mat(diss_matrix.c_str());

    int row = 0;
    int col = 0;
	while (!diss_mat.eof())
	{
        
        std::string cur_line;
        diss_mat >> cur_line;
                
        std::stringstream s;
        s << cur_line;
        
        
        s >> data[row][col];
              
        row++;
        
        if (row == 48) {
            row %= 48;
            col++;
        }
        
    }
    diss_mat.close();    
}

// column major
void Dissonance::Set(int c, int r, float val) {
    data[c][r] = val;
}

// column major
float Dissonance::Get(int c, int r) {
    return data[c][r];
}




