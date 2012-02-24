//
//  Note.cpp
//  GraphScore
//
//  Created by phunter on 11/3/11.
//  Copyright 2011 Hunter McCurry. All rights reserved.
//

#include "Note.h"
#include "stgl.h"

// position: (x,y), relative midi note: mapped_midi_num, speed s, max_connection_time: t
Note::Note(float x, float y, float z, int mapped_midi_num, float s, int t)
{
    color = STColor4f((float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX,1.0);
    centerPosition = STPoint3(x,y,z);
    printf("centerPosition.x = %f, centerPosition.y = %f\n", centerPosition.x, centerPosition.y);
    radius = .2;
    width_max = (10 * radius) / .2;
    
    excitement = 0.0;
    speed = s;
    max_connection_time = t;
    
    mapped_midi = mapped_midi_num;
    Spelling my_spell = SpellNote(mapped_midi_num);
    note_space = my_spell.staff_offset;
    accidental = my_spell.accidental;
    
    max_connections = 50;
    
    connection_list.resize(max_connections);
    
    rel_space = note_space;
    // for notes with ledger lines
    if (note_space > 5) {
        rel_space = 4 + (note_space%2);
    }
    if (note_space < -5) {
        rel_space = -4 + (note_space%2);
    }
}

float Note::getExcite() {
    return excitement;
}

void Note::setExcite(float e) {
    excitement = e;
}

void Note::addConnection(Note * note, float dist)
{
    Connection *new_connect = new Connection();
    new_connect->next_note = note;
    new_connect->ideal_dist = dist;
    new_connect->time_count = 0;
    
    connection_list[note->getMappedMidi()] = new_connect;    
}

void Note::addTwoWayConnection(Note *note, float dist)
{
    
    Connection *new_connect = new Connection();
    new_connect->next_note = note;
    new_connect->ideal_dist = dist;
    new_connect->time_count = 0;
    
    connection_list[note->getMappedMidi()] = new_connect;
    
    note->addConnection(this, dist);
}

void Note::updateConnection(int mapped_midi_num, float dist)
{
    if (mapped_midi != mapped_midi_num) {
        connection_list[mapped_midi_num]->ideal_dist = dist;
        
        ResetTimeCount(mapped_midi_num);
    }
}

void Note::updateTwoWayConnection(int mapped_midi_num, float dist)
{
    if (mapped_midi != mapped_midi_num) {
        connection_list[mapped_midi_num]->ideal_dist = dist;
        connection_list[mapped_midi_num]->next_note->updateConnection(mapped_midi, dist);
        
        ResetTimeCount(mapped_midi_num);
    }
}

void Note::DeleteConnection(int mapped_midi_num) {
    // TODO: clean up
    connection_list[mapped_midi_num] = NULL;
}

void Note::DeleteTwoWayConnection(int mapped_midi_num) {
    // TODO: clean up
    connection_list[mapped_midi_num]->next_note->DeleteConnection(mapped_midi);
    connection_list[mapped_midi_num] = NULL;
}

void Note::ResetTimeCount(int mapped_midi_num) {
    connection_list[mapped_midi_num]->time_count = 0;
}

void Note::IncrementTimeCount() {
    for (int i = 0; i < connection_list.size(); i++) {
        if (connection_list[i] != NULL) {
            connection_list[i]->time_count++;
        }
    }
}

void Note::TrimOldConnections() {
    for (int i = 0; i < connection_list.size(); i++) {
        if (connection_list[i] != NULL) {
            if (connection_list[i]->time_count > max_connection_time) {
                DeleteTwoWayConnection(i);
            }
        }
    }
}

STPoint3 * Note::getLocation()
{
    return &centerPosition;
}

int Note::getMappedMidi() {
    return mapped_midi;
}

Spelling Note::SpellNote(int mapped_midi) {
    
    // Convert to pitch class, octave represnetation
    int pitch_class_mult = (mapped_midi - 1);
    int pitch_class = pitch_class_mult%12;
    // Octave from Middle C to higher C considered octave 0
    int octave = (int)(pitch_class_mult / 12) - 1;
    
    int space;
    int accid = 0;
    
    // for only sharp representation
    
    // first special end cases:
    if ( mapped_midi == 47) {
        space = 13;
        accid = 1;
    }
    else if ( mapped_midi == 0) {
        space = -14;
        accid = -1;
    }
    else if (pitch_class == 0) {
        space = 0;
    }
    else if (pitch_class == 1) {
        if (maybe() || maybe()) {
            space = 0;
            accid = 1;
        }
        else {
            space = 1;
            accid = -1;
        }
    }
    else if (pitch_class == 2) {
        space = 1;
    }
    else if (pitch_class == 3) {
        if (maybe() && maybe()) {
            space = 1;
            accid = 1;
        }
        else {
            space = 2;
            accid = -1;
        }
    }
    else if (pitch_class == 4) {
        space = 2;
    }
    else if (pitch_class == 5) {
        space = 3;
    }
    else if (pitch_class == 6) {
        if (maybe() || maybe() || maybe()) {
            space = 3;
            accid = 1;
        }
        else {
            space = 4;
            accid = -1;
        }
    }
    else if (pitch_class == 7) {
        space = 4;
    }
    else if (pitch_class == 8) {
        if (maybe()) {
            space = 4;
            accid = 1;
        }
        else {
            space = 5;
            accid = -1;
        }
    }
    else if (pitch_class == 9) {
        space = 5;
    }
    else if (pitch_class == 10) {
        if (maybe() && maybe() && maybe()) {
            space = 5;
            accid = 1;
        }
        else {
            space = 6;
            accid = -1;
        }
    }
    else {
        space = 6;
    }
    
    
    Spelling note_spelling;
    printf("octave is %d, space is %d, accidental is %d\n", octave, space, accid);
    
    note_spelling.staff_offset = (7 * octave) + space - 6;
    note_spelling.accidental = accid;
    
    return note_spelling;
}

void Note::Nudge(STVector3 displacement)
{
    centerPosition += displacement;
}

void Note::MoveFromConnections()
{
    for (int i = 0; i < max_connections; i++) {
        
        if (connection_list[i] != NULL) {
            STPoint3 me = centerPosition;
            STPoint3 him = *connection_list[i]->next_note->getLocation();
            STVector3 path = him - me;
            float mag = path.Length();
            STVector3 dir = path / mag;
            
            STPoint3 newPos = me + ((him - me) * (mag - connection_list[i]->ideal_dist))*speed;
            centerPosition = newPos;
        }
    }
}

float Note::easeRamp(float input) {
    return pow(input,3)*(3 - 2*input);
}

float Note::easeBump(float input) {
    return pow(input,3) - 2*pow(input,2) + input;
}

void Note::MoveFromDissonance(Note *other, float diss_val) {
    
    STPoint3 me = centerPosition;
    STPoint3 him = *other->getLocation();
    STVector3 path = him - me;
    float mag = path.Length();
    STVector3 dir = path / mag;
    
    STVector3 nudgeVec;
    int note_distance = abs(mapped_midi - other->getMappedMidi());
    
    float from_max_midi_num = 0; //.12 * min(mapped_midi, other->getMappedMidi());
    float from_midi_diff = .1 * (float)note_distance; // .05 * pow((float)note_distance,2);
    float from_diss_val = 20.0 * easeRamp(diss_val) ;//(3.0 * log(diss_val+2.0)); //12.0 * pow(1-easeBump(diss_val),7.0);
    
    float speed_due_to_dist = 1.0; //1.0/(pow(mag,20)+ 1.0); // speed is inversely porportional to mag
    float how_fast = .0005 * speed_due_to_dist; //* (1.0/(mag * mag * mag * mag * mag * mag * mag * mag * mag * mag * mag * mag + 1.0));
    
    STPoint3 newPos = me + ((him - me) * (mag - (from_max_midi_num + from_midi_diff + from_diss_val))) * how_fast;
    
    nudgeVec = me - newPos;
    
    centerPosition = me - nudgeVec/2;
    other->Nudge(nudgeVec/2);
}

void Note::AttractFromDissonance(Note *other, float diss_val) {
    
    STPoint3 me = centerPosition;
    STPoint3 him = *other->getLocation();
    STVector3 path = him - me;
    float mag = path.Length();
    STVector3 dir = path / mag;
        
    // determines at which point dissonance transfers from attract to repel
    float crossing_val = .1;
    
    float how_fast = .005; //* (1.0/(mag * mag * mag * mag * mag * mag * mag * mag * mag * mag * mag * mag + 1.0));
    
    int note_distance = abs(mapped_midi - other->getMappedMidi());
    float from_midi_diff = .1 * pow((float)note_distance,.5);
    float from_max_midi_num = .008 * min(mapped_midi, other->getMappedMidi());
    
    float summed_diss = easeRamp(diss_val) - from_midi_diff + from_max_midi_num;
    
    STVector3 nudgeVec;
    
    float eps = .01;
    STPoint3 newPos = me + dir * (how_fast/*/(pow(mag,1)+eps)*/) * (crossing_val - summed_diss);
    
    nudgeVec = me - newPos;
    
    centerPosition = me - nudgeVec/2;
    other->Nudge(nudgeVec/2);
}


void Note::RepelFrom(Note *other) {
    
    STPoint3 me = centerPosition;
    STPoint3 him = *other->getLocation();
    STVector3 path = him - me;
    float mag = path.Length();
    STVector3 dir = path / mag;
    
    //STPoint3 newPos = me + ((him - me) * (mag - ideal_dists[i]))*speed;
    
    STVector3 nudgeVec;
    
    float overlap = mag - 2*radius;
    
    // strong repulsion for radius collisions
    if (overlap < 0.0) {
        nudgeVec = -(overlap/2)*dir;
    }
    // general repulsion
    nudgeVec += (1.0 / (pow(mag,4) + .3)) * dir * .005; // repulsion diminishes with square of distance
        
    centerPosition = me - nudgeVec/2;
    other->Nudge(nudgeVec/2);
}

void Note::AttractToZ() {
    
    STPoint3 me = centerPosition;
    STPoint3 ground = STPoint3(me.x, me.y, 0.0);
    STVector3 path = ground - me;
    float mag = path.Length();
    STVector3 dir = path / mag;
    
    // this parameter goes (~ exponentially) from 0.0 = 3D to 1.0 = 2D
    float dimensionality = 0.2; //.06;
    
    centerPosition = me + (ground - me) * dimensionality;
}


void Note::DisplayNotes(float h)
{
    cam_height = h - centerPosition.z;
    width_max = ((10 * radius) / .2) / h;
            
    glEnable( GL_LINE_SMOOTH );
    
    // color inside of in bubble
    glColor4f(color.r, color.g, color.b, excitement);
    DrawCircle(centerPosition, radius, 24, true);    
    
    // draw outer bubble
    glColor4f(0.0f, 0.0f, 0.0f, 1.0); 
    glLineWidth(.8*width_max);
    DrawCircle(centerPosition, radius, 50, false);
    
    // draw staff lines
    glPushMatrix();
    StaffLines();
    glPopMatrix();
    
    // draw note head
    glPushMatrix();    
    NoteHead();
    glPopMatrix();
    
    glDisable( GL_LINE_SMOOTH );
}

void Note::DisplayConnections(float h) {
    
    glEnable( GL_LINE_SMOOTH );
    
    // Draw connections
    DrawConnections();
    
    glDisable( GL_LINE_SMOOTH );
}

bool Note::IsConnectedTo(int mapped_midi_num) {
    if (connection_list[mapped_midi_num] == NULL) {
        return false;
    }
    else {
        return true;
    }
        
    
//    for (int i = 0; i < next_notes.size(); i++) {
//        if (other == next_notes[i]) {
//            return true;
//        }
//    }
//    return false;
}

void Note::DrawConnections()
{
    glLineWidth(.8*width_max);
    glColor4f(0.0,0.0,0.0,1.0);
    glBegin(GL_LINES);
    
    for (int i = 0; i < max_connections; i++) {
        if (connection_list[i] != NULL) {
            STPoint3 orig = centerPosition;
            STPoint3 goal = *connection_list[i]->next_note->getLocation();
            STVector3 path = goal - orig;
            STVector3 dir = path / path.Length();
            
            STPoint3 start = orig + dir*radius;
            STPoint3 finish = goal - dir*radius;
            
            glVertex3f(start.x, start.y, start.z);
            glVertex3f(finish.x, finish.y, finish.z);
        }
    }
    
    glEnd();
}

void Note::StaffLines()
{
    float staffLen = (radius/1.8);
    float ledgerLen = staffLen/3.0;
    float lineHeight = (radius/4.0);
    
    int note_outness = note_space / 2;
    
    glLineWidth(.3*width_max);
    glColor4f(0.0,0.0,0.0,1.0);
    
    glTranslatef(centerPosition.x, centerPosition.y, centerPosition.z);
                
    glBegin(GL_LINES);

    for (int i = -2; i < 3; i++) {
        float tmpLineLen = staffLen;

        // bruteForce
        if (i == -2 && (note_outness <= -3)) {
            tmpLineLen = ledgerLen;
        }
        if (i == -1 && (note_outness <= -4 || note_outness >= 6)) {
            tmpLineLen = ledgerLen;
        }
        if (i == 0 && (note_outness <= -5 || note_outness >= 5)) {
            tmpLineLen = ledgerLen;
        }
        if (i == 1 && (note_outness >= 4 || note_outness <= -6)) {
            tmpLineLen = ledgerLen;
        }
        if (i == 2 && (note_outness >= 3)) {
            tmpLineLen = ledgerLen;
        }
        
        glVertex3f(-tmpLineLen, i * lineHeight, 0.0);
        glVertex3f(tmpLineLen, i * lineHeight, 0.0);
    }    
        
    glEnd();
}

void Note::NoteHead()
{
    glTranslatef(centerPosition.x, centerPosition.y, centerPosition.z); 
    glTranslatef(0.0, rel_space * (radius/8.0), 0.0);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0); 
    //glBegin(GL_TRIANGLE_FAN);
    //glVertex3f(0.0, 0.0, 0.0);
    DrawCircle(STPoint3(0.0,0.0,0.0), radius*0.1, 40, 1);
    //glEnd();
    glLineWidth(.3*width_max);
    //glBegin(GL_LINE_STRIP);
    DrawCircle(STPoint3(0.0,0.0,0.), radius*0.1, 40, 0);
    //glEnd();
    if (accidental == 1) {
        DrawSharp();
    }
    else if (accidental == -1) {
        DrawFlat();
    }
}

void Note::DrawFlat()
{
    glTranslatef(-radius*(8.0/25), 0.0, 0.0);
    // vertical line
    glLineWidth(.3*width_max);
    glBegin(GL_LINES);
    glVertex3f(-radius*.075, -radius*0.17, 0.0);
    glVertex3f(-radius*.075, radius*0.34, 0.0);
    glEnd();
    
    float angle, angle2;
    int numVerts = 32;
    float radi = 0.0;
    float radMax = radius*0.232;
    STPoint2 cent = STPoint2(-radius*0.06, radius*0.075);
    
    
    for (int i = 0; i <= numVerts; i++) {
        
        glLineWidth(.2*width_max + .4*width_max*fabs(sin(i*M_PI/numVerts)));
        
        glBegin(GL_LINES);
        angle = (i-1)*M_PI/numVerts; 
        glVertex3f(cent.x + (sin(angle) * (radi-(radMax/numVerts))), cent.y + (cos(angle) * (radi-(radMax/numVerts))), 0.0);
        angle2 = (i+1)*M_PI/numVerts;
        glVertex3f(cent.x + (sin(angle2) * (radi+(radMax/numVerts))), cent.y + (cos(angle2) * (radi+(radMax/numVerts))), 0.0);
        glEnd();
        
        radi = i * (radMax/numVerts);
    }
}

void Note::DrawSharp()
{
    glTranslatef(-radius*(8.0/25), 0.0, 0.0);
    // vertical lines
    glLineWidth(.3*width_max);
    glBegin(GL_LINES);
    glVertex3f(-radius*0.055,  -radius*0.345, 0.0);
    glVertex3f(-radius*0.055,  radius*0.27, 0.0);
    glVertex3f(radius*0.055,   -radius*0.27, 0.0);
    glVertex3f(radius*0.055,   radius*0.345, 0.0);
    glEnd();
    // horizontal lines
    glLineWidth(.7*width_max);
    glBegin(GL_LINES);
    glVertex3f(-radius*0.12, radius*0.1, 0.0);
    glVertex3f(radius*0.12,  radius*0.175, 0.0);
    glVertex3f(-radius*0.12, -radius*0.175, 0.0);
    glVertex3f(radius*0.12,  -radius*0.1, 0.0);
    glEnd();
}

void Note::DrawCircle(STPoint3 cent, float rad, int numVerts, bool filled) { 
    float angle, angle2;
    if (filled) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(cent.x, cent.y, cent.z);
        for(int i = 0; i <= numVerts; i+=2) { 
            angle = i*2*M_PI/numVerts; 
            glVertex3f(cent.x + (cos(angle) * rad), cent.y + (sin(angle) * rad), cent.z);
        } 
        glEnd();
    }
    else {
        for(int i = 0; i <= numVerts; i++) { 
            glBegin(GL_LINES);
            angle = (i-1)*2*M_PI/numVerts; 
            glVertex3f(cent.x + (cos(angle) * rad), cent.y + (sin(angle) * rad), cent.z);
            angle2 = (i+1)*2*M_PI/numVerts;
            glVertex3f(cent.x + (cos(angle2) * rad), cent.y + (sin(angle2) * rad), cent.z);
            glEnd();
        }
    }
}

bool Note::maybe() {
    float rand_num = (float)rand()/RAND_MAX;
    if (rand_num > .5) {
        return true;
    }
    else return false;
}

