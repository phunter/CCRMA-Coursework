//
//  Camera.cpp
//  GraphScore
//
//  Created by phunter on 11/28/11.
//  Copyright 2011 Hunter McCurry. All rights reserved.
//

#include "Camera.h"

Camera::Camera(float x, float y, float z, float def_height)
{
    target_note = new Note(x, y, 0.0, 24, .03, 2000);
    
    default_height = def_height;
    position.x = x;
    position.y = y;
    position.z = z;
    phantom_position = position;
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

float Camera::GetDefaultHeight() {
    return default_height;
}

void Camera::UpdateDefaultHeight(float def_height) {
    default_height = def_height;
}

void Camera::setTargetNote(Note * t)
{
    target_note = t;
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
    return &position;
}

void Camera::computeTravelDist(){
    
    aiVector3D target_pos = aiVector3D(*target_note->getLocation());
    cam_travel_dist = (target_pos - aiVector3D(position)).Length();
}

//void Camera::calculateSpeed() {
//    last_speed = speed;
//    speed = (STVector3(position) - STVector3(last_position)).Length();
//}
//
//void Camera::calculateAccelteration() {
//    calculateSpeed();
//    acceleration = fabs(last_speed - speed);
//    printf("acceleration = %f\n",acceleration);
//}

void Camera::updateCam() {
    computeTravelDist();
    nudgeCam();
}

void Camera::nudgeCam() {
    
    target = target_note->getLocation();
    
    //printf("position is (%f,%f,%f)\n", target_pos.x,target_pos.y,target_pos.z);
    
    aiVector3D remaining_path = (aiVector3D(*target) - aiVector3D(phantom_position));
    remaining_path.z = 0.0; // so camera height doesn't factor in
    
    //printf("mult thing %f\n", easeBump(1.0-remaining_path.Length()/cam_travel_dist));
    
    float pcnt_there = 1.0-remaining_path.Length()/cam_travel_dist;
    last_position = phantom_position;
    phantom_position += 1.0 * easeBump(pcnt_there) * remaining_path;

    //calculateAccelteration();
    //phantom_position.z = default_height + 5*acceleration;
    
    phantom_position.z += .05 * pow(cam_travel_dist,2) * easeBump(pcnt_there);
    phantom_position.z = default_height + .9 * (phantom_position.z - default_height);
    slewRealCam();
}

void Camera::slewRealCam() {
    position += .1 * (aiVector3D(phantom_position) - aiVector3D(position));
}
