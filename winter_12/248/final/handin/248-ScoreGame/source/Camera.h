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
#include "Note.h"
//#include "MathFun.h"

class Camera {
public:
    Camera(float x, float y, float def_height);
    
    float easeRamp(float input);
    float easeRampReverse(float input);    
    float easeBump(float input);    
    float easeBumpReverse(float input);
    float raisedCos(float input);
    
    float GetDefaultHeight();
    void UpdateDefaultHeight(float def_height);
    
    void setTargetNote(Note * t);
    aiVector3D * getTargetNotePosition();
    //void setTarget(STPoint3 * t);
    aiVector3D * getPosition();
    aiVector3D * getLookAt();
	aiVector3D * getUpDir();
	
	void CallLookAt(float aspectRatio);
    
//    void calculateAccelteration();
//    void calculateSpeed();
    
    void updateCam(float delta);
    void nudgeCam();
    void slewPhantomCam(float delta);
	void slewVantageStretch(float delta);
	
	void positionRealCam(float delta);
    void slewLookAt(float delta);
    void slewRealCam();
    void computeTravelDist();
    

private:
    
    float default_height;
	float current_height;
    float default_setback;
	float current_setback;
	
	aiVector3D default_vantage; // ideal viewing angle (as offset from look_position)
	aiVector3D vantage_stretch; // should attempt to be 0
	
    Note * target_note;
    aiVector3D target_position;
    
    aiVector3D phantom_position;
    
    aiVector3D cam_position;
    aiVector3D look_position;
	aiVector3D up_dir;
    
    aiVector3D start_position;
    
    aiVector3D last_position;
    
    float speed;
    float last_speed;
    
//    float acceleration;
    
    float cam_travel_dist;
};

#endif