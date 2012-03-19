//
//  Camera.cpp
//  GraphScore
//
//  Created by Hunter McCurry on 11/28/11.
//  Copyright 2011 Hunter McCurry. All rights reserved.
//

#include "Camera.h"

Camera::Camera(float x, float y, float def_height)
{
    target_note = new Note(x, y, 0.0, 24, .03, 2000, NULL);
    
    default_height = def_height;
    cam_position.x = x;
    cam_position.y = y;
    cam_position.z = def_height; // camera always starts at default height
    
    phantom_position = *target_note->getLocation();
    start_position = phantom_position;
    look_position = phantom_position;
    
//    printf("Initial cam_position is (%f,%f,%f)\n",cam_position.x,cam_position.y,cam_position.z);
//    printf("Initial phantom_position is (%f,%f,%f)\n",phantom_position.x,phantom_position.y,phantom_position.z);
}

float Camera::easeRamp(float input) {
    return pow(input,3)*(3 - 2*input);
}

float Camera::easeRampReverse(float input) {
    return 2*pow(input,3) - 3*pow(input,2) + 1;
}

float Camera::easeBump(float input) {
    return pow(input,3) - 2*pow(input,2) + input;
}

float Camera::easeBumpReverse(float input) {
    return pow(input,2) - pow(input,3);
}

float Camera::raisedCos(float input) {
    // input and output scaled from 0 to 1
    return 1.0 - ((cos(input * 2 * M_PI) + 1)/2.0);
}

float Camera::GetDefaultHeight() {
    return default_height;
}

void Camera::UpdateDefaultHeight(float def_height) {
    default_height = def_height;
}

void Camera::setTargetNote(Note * t)
{
    target_note = t;
    start_position = cam_position;
    computeTravelDist();
}

aiVector3D * Camera::getTargetNotePosition() {
    return target_note->getLocation();
}

//void Camera::setTarget(STPoint3 * t)
//{
//    target = t;
//    computeTravelDist();
//}

aiVector3D * Camera::getPosition() {
    return &cam_position;
}

aiVector3D * Camera::getLookAt() {
    return &look_position;
}

void Camera::computeTravelDist(){
    target_position = aiVector3D(*target_note->getLocation());
    
    printf("First target_position is (%f,%f,%f)\n",target_position.x,target_position.y,target_position.z);
    
    cam_travel_dist = (target_position - start_position).Length();
}


void Camera::updateCam(float delta) {
    computeTravelDist();
    
    slewLookAt(delta);
    newNudgeCam(delta);
}

//void Camera::nudgeCam() {
//  
//    // this changed: FIX IF USED AGAIN
////    target_position = target_note->getLocation();
//    
//    //printf("target_position is (%f,%f,%f)\n", target_position->x,target_position->y,target_position->z);
//    
//    aiVector3D remaining_path = (aiVector3D(target_position) - aiVector3D(phantom_position));
//    remaining_path.z = 0.0; // so camera height doesn't factor in
//    
//    //printf("mult thing %f\n", easeBump(1.0-remaining_path.Length()/cam_travel_dist));
//    
//    float pcnt_there = 1.0-remaining_path.Length()/cam_travel_dist;
//    last_position = phantom_position;
//    phantom_position += 1.0 * easeBump(pcnt_there) * remaining_path;
//
//    //calculateAccelteration();
//    //phantom_position.z = default_height + 5*acceleration;
//    
//    phantom_position.z += .05 * pow(cam_travel_dist,2) * easeBump(pcnt_there);
//    phantom_position.z = default_height + .9 * (phantom_position.z - default_height);
//    slewRealCam();
//}

void Camera::newNudgeCam(float delta) {
    
    aiVector3D remaining_path = target_position - phantom_position;
    //remaining_path.z = 0.0; // so camera height doesn't factor in
        
    float pcnt_there = 1.0-remaining_path.Length()/(cam_travel_dist + .00001);
        
    float easeVal = raisedCos(pcnt_there);
    phantom_position += .0005 * easeVal * pow(remaining_path.Length(),2) * remaining_path;
        
    // cam height
    cam_position = phantom_position + aiVector3D(0.0, 0.0/*-1.6*/, default_height);
}

void Camera::slewLookAt(float delta) {

    aiVector3D remaining_path = target_position - look_position;
    //remaining_path.z = 0.0; // so camera height doesn't factor in
    
    float pcnt_left = remaining_path.Length()/(cam_travel_dist + .00001);
        
    float easeVal = raisedCos(pcnt_left);
    look_position += .003 * (easeVal + .2) * pow(remaining_path.Length(),2) * remaining_path;
}


//void Camera::slewRealCam() {
//    cam_position += .01 * (aiVector3D(phantom_position) - aiVector3D(cam_position));
//}




