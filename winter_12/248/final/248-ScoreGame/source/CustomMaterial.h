//
//  CustomMaterial.h
//  248-ScoreGame
//
//  Created by Hunter McCurry on 3/17/12.
//  Copyright (c) 2012 Oberlin College. All rights reserved.
//

#ifndef _48_ScoreGame_CustomMaterial_h
#define _48_ScoreGame_CustomMaterial_h

struct CustomMaterial {
    CustomMaterial() {
        diff_color[0] = diff_color[1] = diff_color[2] = .25;
        spec_color[0] = spec_color[1] = spec_color[2] = .15;
        amb_color[0] = amb_color[1] = amb_color[2] = .25;
        
        diff_color[3] = spec_color[3] = amb_color[3] = 1.0;
        
        shiny = 40.0;
    }
    
    GLfloat diff_color[4];
    GLfloat spec_color[4];
    GLfloat amb_color[4];
    GLfloat shiny;
};

#endif
