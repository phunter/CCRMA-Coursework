//
//  Camera.h
//  GraphScore
//
//  Created by phunter on 11/28/11.
//  Copyright 2011 Hunter McCurry. All rights reserved.
//

#ifndef GraphScore_Camera_h
#define GraphScore_Camera_h

#include "Framework.h"
//#include "STVector3.h"
//#include "STPoint3.h"
#include "Note.h"
//#include "MathFun.h"

class Camera {
public:
    Camera(float x, float y, float z, float def_height);
    
    float easeRamp(float input);
    float easeRampReverse(float input);    
    float easeBump(float input);    
    float easeBumpReverse(float input);
    
    float GetDefaultHeight();
    void UpdateDefaultHeight(float def_height);
    
    void setTargetNote(Note * t);
    aiVector3D * getTargetNotePosition();
    //void setTarget(STPoint3 * t);
    aiVector3D * getPosition();
    
//    void calculateAccelteration();
//    void calculateSpeed();
    
    void updateCam();
    void nudgeCam();
    void slewRealCam();
    void computeTravelDist();
    

private:
    
    float default_height;
    
    Note * target_note;
    aiVector3D * target;
    aiVector3D phantom_position;
    aiVector3D position;
    
    aiVector3D last_position;
    
    float speed;
    float last_speed;
    
//    float acceleration;
    
    float cam_travel_dist;
};

#endif