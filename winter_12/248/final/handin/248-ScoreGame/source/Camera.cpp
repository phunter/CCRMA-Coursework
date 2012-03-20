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
	
	default_setback = 1.0;
	
	default_vantage = aiVector3D(0.0, -default_setback, default_height); // ideal offset from phantom position
	
	aiVector3D default_angle = default_angle; // to start
    
    phantom_position = *target_note->getLocation();
    start_position = phantom_position;
    look_position = phantom_position;
	
	up_dir = aiVector3D(0.0, 2.0, 1.0);
    
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
	default_vantage.z = default_height;
}

void Camera::setTargetNote(Note * t)
{
	if (t != target_note) {
    	target_note = t;
	}
	
    start_position = phantom_position; // save phantom position so we know where we started this move
    computeTravelDist();
}

aiVector3D * Camera::getTargetNotePosition() {
    return target_note->getLocation();
}

aiVector3D * Camera::getPosition() {
    return &cam_position;
}

aiVector3D * Camera::getLookAt() {
    return &look_position;
}

aiVector3D * Camera::getUpDir() {
    return &up_dir;
}

void Camera::CallLookAt(float aspectRatio) {

//	GLfloat nearClip = .1f;
//	GLfloat farClip = 50.f;
//	GLfloat fieldOfView = 45.0f;
//	
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(fieldOfView, aspectRatio, nearClip, farClip);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
	
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glFrustum (-.5*aspectRatio, .5*aspectRatio, -.5, .5, 1.0, 30.0);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt (cam_position.x, cam_position.y, cam_position.z, look_position.x, look_position.y, look_position.z, up_dir.x, up_dir.y, up_dir.z);
}

void Camera::computeTravelDist(){
    target_position = aiVector3D(*target_note->getLocation());
    
    //printf("First target_position is (%f,%f,%f)\n",target_position.x,target_position.y,target_position.z);
    
    cam_travel_dist = (target_position - start_position).Length();
}


void Camera::updateCam(float delta) {
    computeTravelDist();
    
    slewLookAt(delta);
    slewPhantomCam(delta);
	
	//cam_position = phantom_position + default_vantage;
	//slewStretchyCameraAngle(delta);
	
	positionRealCam(delta);
}


void Camera::slewLookAt(float delta) {

    aiVector3D remaining_look_path = target_position - look_position;
    
    float pcnt_left = remaining_look_path.Length()/(cam_travel_dist + .00001); // no divide by 0!
        
    float easeVal = raisedCos(pcnt_left);
    look_position += .01 * (easeVal + .2) * remaining_look_path; // * pow(remaining_path.Length(),2) * remaining_path;
}



void Camera::slewPhantomCam(float delta) {
    
    aiVector3D remaining_phantom_path = target_position - phantom_position;
    //remaining_path.z = 0.0; // so camera height doesn't factor in
        
    float pcnt_left = remaining_phantom_path.Length()/(cam_travel_dist + .00001); // no divide by 0!
        
    float easeVal = raisedCos(pcnt_left);
    phantom_position += .005 * (easeVal + .2) * remaining_phantom_path;
}

void Camera::slewVantageStretch(float delta) {

	// try to get back to no stretch
	aiVector3D toGo = aiVector3D(0.0,0.0,0.0) - vantage_stretch;
    vantage_stretch += .05 * delta * toGo;

}

void Camera::positionRealCam(float delta) {
	

	// first set default position
	//cam_position = phantom_position + aiVector3D(0.0, default_setback, default_height);
	cam_position = phantom_position + default_vantage;
	
	float cam_to_look = (look_position - cam_position).Length();
	// in case that is not enough, add extra
	//if ( cam_to_target < 1.3) {
   		cam_position += (.5/(cam_to_look + .1)) * default_vantage;
	//}
	
	// check look direction
//	aiVector3D look_dir = look_position - cam_position;
//	look_dir.Normalize();
	
//	// set "stretched" position: 
//	//current_vantage.y -= max(1.0/look_dir.y, 1.0);
//	//current_vantage.z += ();
//	aiVector3D move_in_dir = look_dir - default_vantage;
//	cam_position += .05 * move_in_dir;
//	
//	// get the NEW look direction
//	aiVector3D final_look_dir = look_position - cam_position;
//	final_look_dir.Normalize();
//	
//	// then handle up direction
//	aiVector2D XY_look_projection = aiVector2D((look_position - cam_position).x, (look_position - cam_position).y);
//	XY_look_projection.Normalize();
//
//	aiVector2D YZ_look_projection = aiVector2D((look_position - cam_position).y, (look_position - cam_position).z);
//	YZ_look_projection.Normalize();
//
//	aiVector2D Y_forward = aiVector2D(0.0, 1.0); // already normalized
//
//	float horiz_dot = XY_look_projection.x * Y_forward.x + XY_look_projection.y + Y_forward.y;	
//	float vertic_dot = YZ_look_projection.x * Y_forward.x + YZ_look_projection.y + Y_forward.y;
//
//	up_dir = aiVector3D(0.0, 1.0/(vertic_dot+ .00001), 1.0/(horiz_dot+ .00001));
//	//up_dir = aiVector3D(0.0, fabs(look_dir.x) + .00001, fabs(look_dir.z) + .00001);
//	up_dir.Normalize();
}



