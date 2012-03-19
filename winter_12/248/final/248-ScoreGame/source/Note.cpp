//
//  Note.cpp
//  GraphScore
//
//  Created by phunter on 11/3/11.
//  Copyright 2011 Hunter McCurry. All rights reserved.
//

#include "Note.h"

// position: (x,y), relative midi note: mapped_midi_num, speed s, max_connection_time: t
Note::Note(float x, float y, float z, int mapped_midi_num, float s, int t, std::vector<Shader*> *shaders_)
{
    color = aiColor4D((float)rand()/RAND_MAX,(float)rand()/RAND_MAX,(float)rand()/RAND_MAX,1.0);
    centerPosition = aiVector3D(x,y,z);
    
//    printf("centerPosition.x = %f, centerPosition.y = %f\n", centerPosition.x, centerPosition.y);

    default_radius = .2;
    radius = default_radius;
    //line_thickness = .03;
    line_thickness = .02;
    width_max = (10 * radius) / .2;
    
    excitement = 0.0;
    speed = s;
    max_connection_time = t;
    
    globalScale = aiVector3D(1.0, 1.0, 1.0);//.001);
    
    mapped_midi = mapped_midi_num;
    Spelling my_spell = SpellNote(mapped_midi_num);
    note_space = my_spell.staff_offset;
    accidental = my_spell.accidental;
    
    max_connections = 50;
    
    connection_list.resize(max_connections);
    
    shaders = shaders_;
    
    rel_space = note_space;
    // for notes with ledger lines
    if (note_space > 5) {
        rel_space = 4 + (note_space%2);
    }
    if (note_space < -5) {
        rel_space = -4 + (note_space%2);
    }
    
    t_outerSegments = 50;
    t_innerSegments = 12;
    t_num_vertices = 6 * t_outerSegments * t_innerSegments;
    
    // set up materials
    outer_material = new CustomMaterial;
    fill_material = new CustomMaterial;
    
    fill_material->amb_color[0] = .3 + .4 * color.r;
    fill_material->amb_color[1] = .3 + .4 * color.g;
    fill_material->amb_color[2] = .3 + .4 * color.b;
    fill_material->diff_color[0] = .3 * color.r;
    fill_material->diff_color[1] = .3 * color.g;
    fill_material->diff_color[2] = .3 * color.b;
    fill_material->spec_color[0] = fill_material->spec_color[1] = fill_material->spec_color[2] = .1;
    fill_material->shiny = 10;
    
    // set up geometry
    torus_vertices = new CustomVertex[t_num_vertices];
    ConstructTorus(centerPosition, default_radius, line_thickness);
}

Note::~Note()
{
    delete [] torus_vertices;
    delete outer_material;
    delete fill_material;
}


float Note::getExcite() {
    return excitement;
}

void Note::setExcite(float e) {
    excitement = e;
}

void Note::FadeExcite() {
    excitement = max(excitement - .001, 0.0);
    //radius = default_radius + .1 * excitement;
}

void Note::GetCurConnections(vector<int> * note_list) {
    for (int i = 0; i < connection_list.size(); i++) {
        if (connection_list[i] != NULL) {
            note_list->push_back(connection_list[i]->next_note->getMappedMidi());
        }
    }
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

void Note::IncrementTimeCount(float delta) {
    for (int i = 0; i < connection_list.size(); i++) {
        if (connection_list[i] != NULL) {
            connection_list[i]->time_count += delta;
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

aiVector3D * Note::getLocation()
{
    return &centerPosition;
}

float Note::getRadius() {
    return radius;
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

void Note::Nudge(aiVector3D displacement)
{
    centerPosition += displacement;
}

void Note::MoveFromConnections(float delta)
{
    for (int i = 0; i < max_connections; i++) {
        
        if (connection_list[i] != NULL) {
            aiVector3D me = centerPosition;
            aiVector3D him = *connection_list[i]->next_note->getLocation();
            aiVector3D path = him - me;
            float mag = path.Length();
            aiVector3D dir = path / mag;
            
            aiVector3D newPos = me + ((him - me) * (mag - connection_list[i]->ideal_dist))*speed*delta;
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

void Note::MoveFromDissonance(Note *other, float diss_val, float delta) {
    
    aiVector3D me = centerPosition;
    aiVector3D him = *other->getLocation();
    aiVector3D path = him - me;
    float mag = path.Length();
    aiVector3D dir = path / mag;
    
    aiVector3D nudgeVec;
    int note_distance = abs(mapped_midi - other->getMappedMidi());
    
    float from_max_midi_num = 0; //.12 * min(mapped_midi, other->getMappedMidi());
    float from_midi_diff = .1 * (float)note_distance; // .05 * pow((float)note_distance,2);
    float from_diss_val = 20.0 * easeRamp(diss_val) ;//(3.0 * log(diss_val+2.0)); //12.0 * pow(1-easeBump(diss_val),7.0);
    
    float speed_due_to_dist = 2.0; //1.0/(pow(mag,20)+ 1.0); // speed is inversely porportional to mag
    float how_fast = speed_due_to_dist * speed * delta;
    
    aiVector3D newPos = me + ((him - me) * (mag - (from_max_midi_num + from_midi_diff + from_diss_val))) * how_fast;
    
    nudgeVec = me - newPos;
    
    centerPosition = me - nudgeVec/2;
    other->Nudge(nudgeVec/2);
}

void Note::AttractFromDissonance(Note *other, float diss_val, float delta) {
    
    aiVector3D me = centerPosition;
    aiVector3D him = *other->getLocation();
    aiVector3D path = him - me;
    float mag = path.Length();
    aiVector3D dir = path / mag;
        
    // determines at which point dissonance transfers from attract to repel
    float crossing_val = .4;
    
    float how_fast = (4/(pow(mag,1) + .00001)) * speed * delta;
    
    int note_distance = abs(mapped_midi - other->getMappedMidi());
    float from_midi_diff = .1 * pow((float)note_distance,.5);
    float from_max_midi_num = .008 * min(mapped_midi, other->getMappedMidi());
    
    float summed_diss = easeRamp(diss_val) - from_midi_diff + from_max_midi_num;
    
    aiVector3D nudgeVec;
    
    //float eps = .01;
    aiVector3D newPos = me + dir * how_fast * (crossing_val - summed_diss);
    
    nudgeVec = me - newPos;
    
    centerPosition = me - nudgeVec/2;
    other->Nudge(nudgeVec/2);
}


void Note::RepelFrom(Note *other, float delta) {
    
    aiVector3D me = centerPosition;
    aiVector3D him = *other->getLocation();
    aiVector3D path = him - me;
    float mag = path.Length();
    aiVector3D dir = path / mag;
    
    //STPoint3 newPos = me + ((him - me) * (mag - ideal_dists[i]))*speed;
    
    aiVector3D nudgeVec;
    
    float overlap = mag - 2*radius;
    
    // strong repulsion for radius collisions
    if (overlap < 0.0) {
        nudgeVec = -(overlap/2)*dir;
    }
    // general repulsion
    nudgeVec += (1.0 / (pow(mag,2) + .3)) * dir * 5 * speed * delta; // repulsion diminishes with square of distance
        
    centerPosition = me - nudgeVec/2;
    other->Nudge(nudgeVec/2);
}

void Note::AttractToXY(float delta) {
    
    aiVector3D me = centerPosition;
    
    if (me.z < line_thickness) {
        centerPosition = aiVector3D(me.x, me.y, line_thickness);
    }
    else {
        aiVector3D projection = aiVector3D(me.x, me.y, line_thickness + 3.0*excitement);
        aiVector3D path = projection - me;
        float mag = path.Length();
        aiVector3D dir = path / mag;
        
        // this parameter goes (~ exponentially) from 0.0 = 3D to 1.0 = 2D
        float dimensionality = 0.002; //.06;
        float how_fast = 1000.0 * speed * delta;
        
        centerPosition = me + (projection - me) * dimensionality * how_fast;
    }
}


void Note::DisplayNotes(float h)
{
    cam_height = h - centerPosition.z;
    width_max = ((10 * radius) / .2) / h;
            
    
    glEnable( GL_LINE_SMOOTH );

    // color inside of in bubble
    glColor4f(color.r, color.g, color.b, excitement);
    DrawCircle(centerPosition, radius, 50, true);
    
    // draw outer bubble

    glColor4f(0.0f, 0.0f, 0.0f, 1.0); 

    glLineWidth(.8*width_max);
    //DrawCircle(centerPosition, radius, 100, false);
    glDisable(GL_LINE_SMOOTH);
    DrawTorus(centerPosition, radius, line_thickness, 26, 8);
    glEnable(GL_LINE_SMOOTH);
    
    // draw staff lines
    glPushMatrix();
    DrawStaffLines();
    glPopMatrix();
    
    // draw note head
    glPushMatrix();    
    NoteHead();
    glPopMatrix();
    
    glDisable( GL_LINE_SMOOTH );
}

void Note::ApplyGlobalScale() {
    glScalef(globalScale.x, globalScale.y, globalScale.z);
}

void Note::RenderNote()
{
    glPushMatrix();
    
    ApplyGlobalScale();
    
    // why do this? //
    int shaderNum = 01;
    glUseProgram((*shaders)[shaderNum]->programID());
    //////////////////
    
    RenderCircle2(centerPosition, radius, .9, 20);

    shaderNum = 0;
    glUseProgram((*shaders)[shaderNum]->programID());

    //RenderTorus(centerPosition, radius, line_thickness, 50, 12);
    RenderPremadeTorus();
    
    RenderStaffLines();

    RenderNoteHead();

    glPopMatrix();
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

void Note::DisplayConnections()
{
    
    for (int i = 0; i < max_connections; i++) {
        if (connection_list[i] != NULL) {
            aiVector3D orig = centerPosition;
            aiVector3D goal = *connection_list[i]->next_note->getLocation();

            aiVector3D direction = goal - orig;

            aiVector3D XY_direction = aiVector3D(direction.x, direction.y, 0.0); // ignore Z
            
            XY_direction.Normalize();
            
            aiVector3D start = orig + XY_direction*radius;
            aiVector3D finish = goal - XY_direction*radius;
//            printf("orig at (%f,%f,%f)\n",orig.x,orig.y,orig.z);
//            printf("goal at (%f,%f,%f)\n",goal.x,goal.y,goal.z);
//            printf("start at (%f,%f,%f)\n",start.x,start.y,start.z);
//            printf("finish at (%f,%f,%f)\n",finish.x,finish.y,finish.z);

            
            float connectionOpac = 1.0 - .9 * (connection_list[i]->time_count / max_connection_time);
            glColor4f(0.0, 0.0, 0.0, connectionOpac);
            
            DrawCylinder(start, finish, line_thickness, 11);
//            glVertex3f(start.x, start.y, start.z);
//            glVertex3f(finish.x, finish.y, finish.z);
        }
    }
}


void Note::RenderConnections()
{
    glPushMatrix();
    ApplyGlobalScale();
    
    for (int i = 0; i < max_connections; i++) {
        if (connection_list[i] != NULL) {
            aiVector3D orig = centerPosition;
            aiVector3D goal = *connection_list[i]->next_note->getLocation();
            
            aiVector3D direction = goal - orig;
            
            aiVector3D XY_direction = aiVector3D(direction.x, direction.y, 0.0); // ignore Z
            
            XY_direction.Normalize();
            
            aiVector3D start = orig + XY_direction*radius;
            aiVector3D finish = goal - XY_direction*connection_list[i]->next_note->getRadius();
            //            printf("orig at (%f,%f,%f)\n",orig.x,orig.y,orig.z);
            //            printf("goal at (%f,%f,%f)\n",goal.x,goal.y,goal.z);
            //            printf("start at (%f,%f,%f)\n",start.x,start.y,start.z);
            //            printf("finish at (%f,%f,%f)\n",finish.x,finish.y,finish.z);
            
            
            float connectionOpac = 1.0 - .9 * (connection_list[i]->time_count / max_connection_time);
            glColor4f(0.0, 0.0, 0.0, connectionOpac);
            
            //float my_thickness = fmax(.005, line_thickness - .002 * direction.Length());
            //RenderCylinder(start, finish, my_thickness, 8);
            
            RenderCylinder(start, finish, line_thickness, 16);
            
            //            glVertex3f(start.x, start.y, start.z)
            //            glVertex3f(finish.x, finish.y, finish.z);
        }
    }
    glPopMatrix();
}


void Note::DrawStaffLines()
{
    float staffLen = (radius/1.8);
    float ledgerLen = staffLen/3.0;
    float lineHeight = (radius/4.0);
    
    int note_outness = note_space / 2;
    
    glLineWidth(.3*width_max);
    glColor4f(0.0,0.0,0.0,1.0);
    
    glTranslatef(centerPosition.x, centerPosition.y, centerPosition.z);
                

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
        
        DrawCylinder(aiVector3D(-tmpLineLen, i * lineHeight, 0.0),
                     aiVector3D(tmpLineLen, i * lineHeight, 0.0),
                     .005, 4);
    }    
}

void Note::RenderStaffLines()
{
    glPushMatrix();
    float staffLen = (radius/1.8);
    float ledgerLen = staffLen/3.0;
    float lineHeight = (radius/4.0);
    
    int note_outness = note_space / 2;
        
    glTranslatef(centerPosition.x, centerPosition.y, centerPosition.z);
    
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
        
        RenderCylinder(aiVector3D(-tmpLineLen, i * lineHeight, 0.0),
                       aiVector3D(tmpLineLen, i * lineHeight, 0.0),
                       .005, 4); //.005, 4
    }
    glPopMatrix();
}

void Note::NoteHead()
{
    glTranslatef(centerPosition.x, centerPosition.y, centerPosition.z); 
    glTranslatef(0.0, rel_space * (radius/8.0), 0.01);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0); 

    DrawCircle(aiVector3D(0.0,0.0,0.0), radius*0.1, 40, 1);

    glLineWidth(.3*width_max);

    DrawCircle(aiVector3D(0.0,0.0,0.), radius*0.1, 40, 0);

    if (accidental == 1) {
        DrawSharp();
    }
    else if (accidental == -1) {
        DrawFlat();
    }
}

void Note::RenderNoteHead()
{
    glTranslatef(centerPosition.x, centerPosition.y, centerPosition.z); 
    glTranslatef(0.0, rel_space * (radius/8.0), 0.0051); // above staff lines
//    glColor4f(0.0f, 0.0f, 0.0f, 1.0); 
//    
    RenderCircle(aiVector3D(0.0,0.0,0.0), radius*0.12, .9, 20);
//    
//    glLineWidth(.3*width_max);
//    
//    DrawCircle(aiVector3D(0.0,0.0,0.), radius*0.1, 40, 0);
//    
    if (accidental == 1) {
        RenderSharp();
    }
    else if (accidental == -1) {
        RenderFlat();
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
    aiVector2D cent = aiVector2D(-radius*0.06, radius*0.075);
    
    
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

void Note::RenderFlat() {
    
    glTranslatef(-radius*(8.0/25), 0.0, 0.0051); // above staff lines
    // vertical line
    
    RenderCylinder(aiVector3D(-radius*.075, -radius*0.17, 0.0),
                   aiVector3D(-radius*.075, radius*0.34, 0.0), .003, 4);
//    glVertex3f(-radius*.075, -radius*0.17, 0.0);
//    glVertex3f(-radius*.075, radius*0.34, 0.0);

    float roundness = 25.0;
    int numCurveSegments = 12;
    int num_vertices = 14 * numCurveSegments;
    CustomVertex * my_vertices;
    my_vertices = new CustomVertex[num_vertices];
    
    
    float angle, angle2;
    float radi = 0.0;
    float radMax = radius*0.232;
    float radInc = radMax/numCurveSegments;
    float thickness = radius * .05;
    aiVector3D cent = aiVector3D(-radius*0.06, radius*0.075, 0.0);
    
    for (int i = 0; i < numCurveSegments; i++) {
        
        //glLineWidth(.2*width_max + .4*width_max*fabs(sin(i*M_PI/numVerts)));
        
        angle = ((i)*M_PI)/numCurveSegments;
        angle2 = ((i+1)*M_PI)/numCurveSegments;
        
        aiVector3D dir1 = aiVector3D(sin(angle), cos(angle), 0.0);
        aiVector3D dir2 = aiVector3D(sin(angle2), cos(angle2), 0.0);
        
        aiVector3D middle1  = cent + i*radInc * dir1;
        aiVector3D middle2  = cent + (i+1)*radInc * dir2;
        
        float innerRad1 = .2*thickness + .66*thickness * sin(i*M_PI/numCurveSegments);
        float innerRad2 = .2*thickness + .66*thickness * sin((i+1)*M_PI/numCurveSegments);
        
        aiVector3D inner1 = middle1 - innerRad1 * dir1;
        aiVector3D inner2 = middle2 - innerRad2 * dir2;
        aiVector3D outer2 = middle2 + innerRad2 * dir2;
        aiVector3D outer1 = middle1 + innerRad1 * dir1;
        
//        glVertex3f(cent.x + (sin(angle) * (radi-(radMax/numCurveSegments))), cent.y + (cos(angle) * (radi-(radMax/numCurveSegments))), 0.0);
//        glVertex3f(cent.x + (sin(angle2) * (radi+(radMax/numCurveSegments))), cent.y + (cos(angle2) * (radi+(radMax/numCurveSegments))), 0.0);
        
        radi = i * radInc;
        
//        my_vertices[6*i + 0].position = inner1;
//        my_vertices[6*i + 1].position = inner2;
//        my_vertices[6*i + 2].position = outer2;
//        my_vertices[6*i + 3].position = outer2;
//        my_vertices[6*i + 4].position = outer1;
//        my_vertices[6*i + 5].position = inner1;

        my_vertices[12*i + 0].position = inner1;
        my_vertices[12*i + 1].position = inner2;
        my_vertices[12*i + 2].position = middle2;
        my_vertices[12*i + 3].position = middle2;
        my_vertices[12*i + 4].position = middle1;
        my_vertices[12*i + 5].position = inner1;
        
        my_vertices[12*i + 6].position = middle1;
        my_vertices[12*i + 7].position = middle2;
        my_vertices[12*i + 8].position = outer2;
        my_vertices[12*i + 9].position = outer2;
        my_vertices[12*i + 10].position = outer1;
        my_vertices[12*i + 11].position = middle1;
        
        // aiVector3D(cos(angle), sin(angle), 0.0) - cent
        my_vertices[12*i + 0].normal = inner1 - middle1;
        my_vertices[12*i + 1].normal = inner2 - middle2;
        my_vertices[12*i + 2].normal = aiVector3D(0.0, 0.0, 1.0);
        my_vertices[12*i + 3].normal = aiVector3D(0.0, 0.0, 1.0);
        my_vertices[12*i + 4].normal = aiVector3D(0.0, 0.0, 1.0);
        my_vertices[12*i + 5].normal = inner1 - middle1;
        
        my_vertices[12*i + 6].normal = aiVector3D(0.0, 0.0, 1.0);
        my_vertices[12*i + 7].normal = aiVector3D(0.0, 0.0, 1.0);
        my_vertices[12*i + 8].normal = outer2 - middle2;
        my_vertices[12*i + 9].normal = outer2 - middle2;
        my_vertices[12*i + 10].normal = outer1 - middle1;
        my_vertices[12*i + 11].normal = aiVector3D(0.0, 0.0, 1.0);

//        my_vertices[12*i + 0].normal = aiVector3D(0.0, 0.0, -1.0) + roundness * (inner1 - middle1);
//        my_vertices[12*i + 1].normal = aiVector3D(0.0, 0.0, -1.0) + roundness * (inner2 - middle2);
//        my_vertices[12*i + 2].normal = aiVector3D(0.0, 0.0, 1.0);
//        my_vertices[12*i + 3].normal = aiVector3D(0.0, 0.0, 1.0);
//        my_vertices[12*i + 4].normal = aiVector3D(0.0, 0.0, 1.0);
//        my_vertices[12*i + 5].normal = aiVector3D(0.0, 0.0, -1.0) + roundness * (inner1 - middle1);
//        
//        my_vertices[12*i + 6].normal = aiVector3D(0.0, 0.0, 1.0);
//        my_vertices[12*i + 7].normal = aiVector3D(0.0, 0.0, 1.0);
//        my_vertices[12*i + 8].normal = aiVector3D(0.0, 0.0, -1.0) + roundness * (outer2 - middle2);
//        my_vertices[12*i + 9].normal = aiVector3D(0.0, 0.0, -1.0) + roundness * (outer2 - middle2);
//        my_vertices[12*i + 10].normal = aiVector3D(0.0, 0.0, -1.0) + roundness * (outer1 - middle1);
//        my_vertices[12*i + 11].normal = aiVector3D(0.0, 0.0, 1.0);
        
        for (int k = 0 ; k < 12; k++) {
            my_vertices[12*i + k].normal.Normalize();
        }
    }
    
    int shaderNum = 0;
    RenderVertices(my_vertices, num_vertices, outer_material, shaderNum);    
    delete [] my_vertices;
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

void Note::RenderSharp()
{
    glTranslatef(-radius*(8.0/25), 0.0, 0.0051); // above staff lines
    // vertical lines
    RenderCylinder(aiVector3D(-radius*0.055,  -radius*0.345, 0.0),
                   aiVector3D(-radius*0.055,    radius*0.27, 0.0), .003, 4);
//    glVertex3f(-radius*0.055,  -radius*0.345, 0.0);
//    glVertex3f(-radius*0.055,  radius*0.27, 0.0);
    RenderCylinder(aiVector3D(radius*0.055,   -radius*0.27, 0.0),
                   aiVector3D(radius*0.055,   radius*0.345, 0.0), .003, 4);
//    glVertex3f(radius*0.055,   -radius*0.27, 0.0);
//    glVertex3f(radius*0.055,   radius*0.345, 0.0);

    // horizontal lines
    glScalef(1.0, 3.0, 1.0);
    RenderCylinder(aiVector3D(-radius*0.12, .33*radius*0.1, 0.0),
                   aiVector3D(radius*0.12,  .33*radius*0.175, 0.0), .003, 4);
//    glVertex3f(-radius*0.12, radius*0.1, 0.0);
//    glVertex3f(radius*0.12,  radius*0.175, 0.0);
    RenderCylinder(aiVector3D(-radius*0.12, .33*-radius*0.175, 0.0),
                   aiVector3D(radius*0.12,  .33*-radius*0.1, 0.0), .003, 4);
//    glVertex3f(-radius*0.12, -radius*0.175, 0.0);
//    glVertex3f(radius*0.12,  -radius*0.1, 0.0);

}


void Note::DrawCircle(aiVector3D cent, float rad, int numVerts, bool filled) { 
    float angle, angle2;
    
    if (filled) {
        glBegin(GL_TRIANGLE_FAN);
        //glNormal3f(	0.0, 0.0, 1.0);
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
            //glNormal3f(	0.0, 0.0, 1.0);
            angle = (i-1)*2*M_PI/numVerts; 
            glVertex3f(cent.x + (cos(angle) * rad), cent.y + (sin(angle) * rad), cent.z);
            angle2 = (i+1)*2*M_PI/numVerts;
            glVertex3f(cent.x + (cos(angle2) * rad), cent.y + (sin(angle2) * rad), cent.z);
            glEnd();
        }
    }
}

void Note::RenderCircle(aiVector3D cent, float rad, float roundness, int numCorners) { 
    int num_vertices = 3 * numCorners;
    CustomVertex * my_vertices;
    my_vertices = new CustomVertex[num_vertices];
    

    float angle, angle2;
        
    for(int i = 0; i < numCorners; i++) {
        angle = i*2*M_PI/numCorners;
        angle2 = (i+1)*2*M_PI/numCorners;
        
        my_vertices[3*i + 0].position = cent;
        my_vertices[3*i + 1].position = cent + rad * aiVector3D(cos(angle), sin(angle), 0.0);
        my_vertices[3*i + 2].position = cent + rad * aiVector3D(cos(angle2), sin(angle2), 0.0);
        
        // specify normal directions
//        my_vertices[3*i + 0].normal = aiVector3D(0.0, 0.0, 1.0);
//        my_vertices[3*i + 1].normal = aiVector3D(0.0, 0.0, 1.0) + roundness * aiVector3D(cos(angle), sin(angle), 0.0);
//        my_vertices[3*i + 1].normal.Normalize();
//        my_vertices[3*i + 2].normal = aiVector3D(0.0, 0.0, 1.0) + roundness * aiVector3D(cos(angle2), sin(angle2), 0.0);
//        my_vertices[3*i + 2].normal.Normalize();
        
        my_vertices[3*i + 0].normal = aiVector3D(0.0, 0.0, 1.0);
        my_vertices[3*i + 1].normal = aiVector3D(cos(angle), sin(angle), 0.0) - cent;
        my_vertices[3*i + 1].normal.Normalize();
        my_vertices[3*i + 2].normal = aiVector3D(cos(angle2), sin(angle2), 0.0) - cent;
        my_vertices[3*i + 2].normal.Normalize();

        
    } 
    
    int shaderNum = 0;
    //AttachMaterial(outer_material, shaderNum);
    RenderVertices(my_vertices, num_vertices, outer_material, shaderNum);    
    delete [] my_vertices;
}

void Note::RenderCircle2(aiVector3D cent, float rad, float roundness, int numCorners) { 
    int num_vertices = 3 * numCorners;
    CustomVertex * my_vertices;
    my_vertices = new CustomVertex[num_vertices];
    
    float angle, angle2;
    
    for(int i = 0; i < numCorners; i++) {
        angle = i*2*M_PI/numCorners;
        angle2 = (i+1)*2*M_PI/numCorners;
        
        my_vertices[3*i + 0].position = cent;
        my_vertices[3*i + 1].position = cent + rad * aiVector3D(cos(angle), sin(angle), 0.0);
        my_vertices[3*i + 2].position = cent + rad * aiVector3D(cos(angle2), sin(angle2), 0.0);
        
        // specify normal directions
        my_vertices[3*i + 0].normal = aiVector3D(0.0, 0.0, 1.0);
        my_vertices[3*i + 1].normal = aiVector3D(0.0, 0.0, 1.0) + roundness * aiVector3D(cos(angle), sin(angle), 0.0);
        my_vertices[3*i + 1].normal.Normalize();
        my_vertices[3*i + 2].normal = aiVector3D(0.0, 0.0, 1.0) + roundness * aiVector3D(cos(angle2), sin(angle2), 0.0);
        my_vertices[3*i + 2].normal.Normalize();
        
    } 
    
    int shaderNum = 1;
    //AttachMaterial(fill_material, shaderNum);
    
    RenderVertices(my_vertices, num_vertices, fill_material, shaderNum);    
    delete [] my_vertices;
}


void Note::DrawCylinder(aiVector3D endOne, aiVector3D endTwo, float radius, int slices) {
    // 1) translate to endOne
    // 2) rotate to angle(endOne,endTwo)
    // 3) cylinder bottom centered at (0,0,0), top at (0,0,length(endTwo - endOne))
    
    // DRAW
    // top & bottom circle
    aiVector3D connectionVector = endTwo - endOne;
    float length = connectionVector.Length();
    float angle, angle2;
    
    aiVector3D rotationVector = aiVector3D(-connectionVector.y, connectionVector.x, 0.0);
    
    glPushMatrix();
    
    glTranslatef(endOne.x, endOne.y, endOne.z);
    
    // put x-y distance in one dimension of 2D vector
    aiVector2D xyDirection = aiVector2D(connectionVector.x, connectionVector.y);
    aiVector2D heightTriangle = aiVector2D(xyDirection.Length(), connectionVector.z);
    
    heightTriangle.Normalize();
    float rotateDegrees = ((M_PI/2.0)-asin(heightTriangle.y)) * 180.0 / M_PI;
    
    glRotatef(rotateDegrees, rotationVector.x, rotationVector.y, rotationVector.z);

    aiVector3D bottomCenter = aiVector3D(0.0,0.0,0.0);
    aiVector3D topCenter = aiVector3D(0.0,0.0,length);
    
    // draw bottom circle
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0,0.0,-1.0);
    glVertex3f(bottomCenter.x, bottomCenter.y, bottomCenter.z);
    for(int i = 0; i <= slices; i++) { 
        angle = i*2*M_PI/slices; 
        glVertex3f(bottomCenter.x + (cos(angle) * radius), bottomCenter.y + (sin(angle) * radius), bottomCenter.z);
    } 
    glEnd();
    
    // draw top circle
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0,0.0,1.0);
    glVertex3f(topCenter.x, topCenter.y, topCenter.z);
    for(int i = 0; i <= slices; i++) { 
        angle = i*2*M_PI/slices; 
        glVertex3f(topCenter.x + (cos(angle) * radius), topCenter.y + (sin(angle) * radius), topCenter.z);
    } 
    glEnd();
    
    // draw sides of cylinder

    aiVector3D outerPos;
    aiVector3D normalDir;
    glBegin(GL_QUADS);
    for(int i = 0; i < slices; i++) { 
        angle = i*2*M_PI/slices; 
        angle2 = (i+1)*2*M_PI/slices;
        
        outerPos = aiVector3D(bottomCenter.x + (cos(angle) * radius), bottomCenter.y + (sin(angle) * radius), bottomCenter.z);
        normalDir = outerPos - bottomCenter;
        normalDir.Normalize();
        glNormal3f(normalDir.x, normalDir.y, normalDir.z);
        glVertex3f(outerPos.x, outerPos.y, outerPos.z);
        
        outerPos = aiVector3D(bottomCenter.x + (cos(angle2) * radius), bottomCenter.y + (sin(angle2) * radius), bottomCenter.z);
        normalDir = outerPos - bottomCenter;
        normalDir.Normalize();
        glNormal3f(normalDir.x, normalDir.y, normalDir.z);
        glVertex3f(outerPos.x, outerPos.y, outerPos.z);

        
        outerPos = aiVector3D(topCenter.x + (cos(angle2) * radius), topCenter.y + (sin(angle2) * radius), topCenter.z);
        normalDir = outerPos - topCenter;
        normalDir.Normalize();
        glNormal3f(normalDir.x, normalDir.y, normalDir.z);
        glVertex3f(outerPos.x, outerPos.y, outerPos.z);
        
        outerPos = aiVector3D(topCenter.x + (cos(angle) * radius), topCenter.y + (sin(angle) * radius), topCenter.z);
        normalDir = outerPos - topCenter;
        normalDir.Normalize();
        glNormal3f(normalDir.x, normalDir.y, normalDir.z);
        glVertex3f(outerPos.x, outerPos.y, outerPos.z);

        
//        glVertex3f(bottomCenter.x + (cos(angle) * radius), bottomCenter.y + (sin(angle) * radius), bottomCenter.z);
//        glVertex3f(bottomCenter.x + (cos(angle2) * radius), bottomCenter.y + (sin(angle2) * radius), bottomCenter.z);
//        glVertex3f(topCenter.x + (cos(angle2) * radius), topCenter.y + (sin(angle2) * radius), topCenter.z);
//        glVertex3f(topCenter.x + (cos(angle) * radius), topCenter.y + (sin(angle) * radius), topCenter.z);
    } 
    glEnd();
    glPopMatrix();
}

void Note::RenderCylinder(aiVector3D endOne, aiVector3D endTwo, float radius, int slices) {
    
    glPushMatrix();
    
    // num verts = 3 * slices for top and bottom + 6 (one quad) per slice for sides
    int num_vertices = 2 * 3 * slices + 6 * slices; // or simply 12 * slices
    CustomVertex * my_vertices;
    my_vertices = new CustomVertex[num_vertices];
    
    // 1) translate to endOne
    // 2) rotate to angle(endOne,endTwo)
    // 3) cylinder bottom centered at (0,0,0), top at (0,0,length(endTwo - endOne))
    
    // RENDER
    // top & bottom circle
    aiVector3D connectionVector = endTwo - endOne;
    float length = connectionVector.Length();
    float angle, angle2;
    
    aiVector3D rotationVector = aiVector3D(-connectionVector.y, connectionVector.x, 0.0);
    
    glTranslatef(endOne.x, endOne.y, endOne.z);
    
    // put x-y distance in one dimension of 2D vector
    aiVector2D xyDirection = aiVector2D(connectionVector.x, connectionVector.y);
    aiVector2D heightTriangle = aiVector2D(xyDirection.Length(), connectionVector.z);
    
    heightTriangle.Normalize();
    float rotateDegrees = ((M_PI/2.0)-asin(heightTriangle.y)) * 180.0 / M_PI;
    
    glRotatef(rotateDegrees, rotationVector.x, rotationVector.y, rotationVector.z);
    
    
    aiVector3D bottomCenter = aiVector3D(0.0,0.0,0.0);
    aiVector3D topCenter = aiVector3D(0.0,0.0,length);
    
    // draw bottom circle
    for(int i = 0; i < slices; i++) { 
        angle = i*2*M_PI/slices;
        angle2 = (i+1)*2*M_PI/slices;
        
        // specify vertex locations
        my_vertices[3*i + 0].position = bottomCenter;
        my_vertices[3*i + 1].position = bottomCenter + radius * aiVector3D(cos(angle), sin(angle), 0.0);
        my_vertices[3*i + 2].position = bottomCenter + radius * aiVector3D(cos(angle2), sin(angle2), 0.0);
        
        // specify normal directions: all downward
        my_vertices[3*i + 0].normal = aiVector3D(0.0, 0.0, -1.0);
        my_vertices[3*i + 1].normal = aiVector3D(0.0, 0.0, -1.0);
        my_vertices[3*i + 2].normal = aiVector3D(0.0, 0.0, -1.0);

        //glVertex3f(bottomCenter.x + (cos(angle) * radius), bottomCenter.y + (sin(angle) * radius), bottomCenter.z);
    } 
    
    // draw top circle
    for(int i = 0; i < slices; i++) {
        angle = i*2*M_PI/slices;
        angle2 = (i+1)*2*M_PI/slices;
 
        // specify vertex locations
        my_vertices[3*slices + 3*i + 0].position = topCenter;
        my_vertices[3*slices + 3*i + 1].position = topCenter + radius * aiVector3D(cos(angle), sin(angle), 0.0);
        my_vertices[3*slices + 3*i + 2].position = topCenter + radius * aiVector3D(cos(angle2), sin(angle2), 0.0);
        
        // specify normal directions: all downward
        my_vertices[3*slices + 3*i + 0].normal = aiVector3D(0.0, 0.0, 1.0);
        my_vertices[3*slices + 3*i + 1].normal = aiVector3D(0.0, 0.0, 1.0);
        my_vertices[3*slices + 3*i + 2].normal = aiVector3D(0.0, 0.0, 1.0);

        //glVertex3f(topCenter.x + (cos(angle) * radius), topCenter.y + (sin(angle) * radius), topCenter.z);
    } 
    
    // draw sides of cylinder
    
    aiVector3D bottom1, bottom2, top1, top2;
    aiVector3D normalDir;
    for(int i = 0; i < slices; i++) { 
        angle = i*2*M_PI/slices; 
        angle2 = (i+1)*2*M_PI/slices;
        
        bottom1 = aiVector3D(bottomCenter.x + (cos(angle) * radius), bottomCenter.y + (sin(angle) * radius), bottomCenter.z);        
        bottom2 = aiVector3D(bottomCenter.x + (cos(angle2) * radius), bottomCenter.y + (sin(angle2) * radius), bottomCenter.z);
        top2 = aiVector3D(topCenter.x + (cos(angle2) * radius), topCenter.y + (sin(angle2) * radius), topCenter.z);
        top1 = aiVector3D(topCenter.x + (cos(angle) * radius), topCenter.y + (sin(angle) * radius), topCenter.z);
        
        // specify positions
        my_vertices[6*slices + 6*i + 0].position = bottom1;
        my_vertices[6*slices + 6*i + 1].position = bottom2;
        my_vertices[6*slices + 6*i + 2].position = top2;
        
        // specify normal directions
        my_vertices[6*slices + 6*i + 0].normal = bottom1 - bottomCenter;
        my_vertices[6*slices + 6*i + 1].normal = bottom2 - bottomCenter;
        my_vertices[6*slices + 6*i + 2].normal = top2 - topCenter;
        
        // specify positions
        my_vertices[6*slices + 6*i + 3].position = top2;
        my_vertices[6*slices + 6*i + 4].position = top1;
        my_vertices[6*slices + 6*i + 5].position = bottom1;
        
        // specify normal directions
        my_vertices[6*slices + 6*i + 3].normal = top2 - topCenter;
        my_vertices[6*slices + 6*i + 4].normal = top1 - topCenter;
        my_vertices[6*slices + 6*i + 5].normal = bottom1 - bottomCenter;
        
    } 
    
    int shaderNum = 0;
    RenderVertices(my_vertices, num_vertices, outer_material, shaderNum);
    delete [] my_vertices;
    
    glPopMatrix();
}


void Note::DrawTorus(aiVector3D center, float centerRadius, float edgeRadius, int outerSegments, int innerSegments) {
    
    glPushMatrix();
    
    glTranslatef(center.x, center.y, center.z);

    glPointSize(4);
    glBegin(GL_QUADS);
    //
    //
    //glNormal3f(	.7, .6, .5);
    //
    //
    
    //Draw order:
    // for outerSegment i, draw innerSegment j
    
    float outerAngle1, outerAngle2, innerAngle1, innerAngle2;
    
    for (int i = 0; i < outerSegments; i++) {
        outerAngle1 = i*2*M_PI/outerSegments; 
        outerAngle2 = (i+1)*2*M_PI/outerSegments;
        
        for (int j = 0; j < innerSegments; j++) {
            innerAngle1 = j*2*M_PI/innerSegments; 
            innerAngle2 = (j+1)*2*M_PI/innerSegments;

            aiVector3D outerInfluence1 = centerRadius * aiVector3D( cos(outerAngle1), sin(outerAngle1), 0.0);
            aiVector3D outerInfluence2 = centerRadius * aiVector3D( cos(outerAngle2), sin(outerAngle2), 0.0);
            
            // here x and y components vary both by inner and outer angles! z component only by inner angle.
                        
            //positon = center + outerInfluence + innerInfluence
            aiVector3D o1i1 = outerInfluence1 + edgeRadius * aiVector3D(cos(outerAngle1) * cos(innerAngle1),
                                                                        sin(outerAngle1) * cos(innerAngle1),
                                                                        sin(innerAngle1));
            aiVector3D o1i2 = outerInfluence1 + edgeRadius * aiVector3D(cos(outerAngle1) * cos(innerAngle2),
                                                                        sin(outerAngle1) * cos(innerAngle2),
                                                                        sin(innerAngle2));
            aiVector3D o2i1 = outerInfluence2 + edgeRadius * aiVector3D(cos(outerAngle2) * cos(innerAngle1),
                                                                        sin(outerAngle2) * cos(innerAngle1),
                                                                        sin(innerAngle1));
            aiVector3D o2i2 = outerInfluence2 + edgeRadius * aiVector3D(cos(outerAngle2) * cos(innerAngle2),
                                                                        sin(outerAngle2) * cos(innerAngle2),
                                                                        sin(innerAngle2));
            
            // counter-clockwise order
            //glColor4f(1, 0, 0, .8);
            glVertex3f(o1i1.x,o1i1.y,o1i1.z);
            //glColor4f(0, 1, 0, .8);
            glVertex3f(o2i1.x,o2i1.y,o2i1.z);
            //glColor4f(.3, .3, 0, .8);
            glVertex3f(o2i2.x,o2i2.y,o2i2.z);
            //glColor4f(0, 0, 1, .8);
            glVertex3f(o1i2.x,o1i2.y,o1i2.z);
            
        }
    }
    
    glEnd();
    
    glPopMatrix();
}

void Note::RenderTorus(aiVector3D center, float centerRadius, float edgeRadius, int outerSegments, int innerSegments) {
    glPushMatrix();
    // it takes 6 vertices to render a quad, and there are (outerSegments * innerSegments) quads
    int num_vertices = 6 * outerSegments * innerSegments;
    
    CustomVertex * my_vertices;
    my_vertices = new CustomVertex[num_vertices];
    
    
    glTranslatef(center.x, center.y, center.z);
    
    //Draw order:
    // for outerSegment i, draw innerSegment j
    
    float outerAngle1, outerAngle2, innerAngle1, innerAngle2;
    
    for (int i = 0; i < outerSegments; i++) {
        outerAngle1 = i*2*M_PI/outerSegments; 
        outerAngle2 = (i+1)*2*M_PI/outerSegments;
        
        for (int j = 0; j < innerSegments; j++) {
            innerAngle1 = j*2*M_PI/innerSegments; 
            innerAngle2 = (j+1)*2*M_PI/innerSegments;
            
            aiVector3D outerInfluence1 = centerRadius * aiVector3D( cos(outerAngle1), sin(outerAngle1), 0.0);
            aiVector3D outerInfluence2 = centerRadius * aiVector3D( cos(outerAngle2), sin(outerAngle2), 0.0);
            
            // here x and y components vary both by inner and outer angles! z component only by inner angle.
            
            //positon = center + outerInfluence + innerInfluence
            aiVector3D o1i1 = outerInfluence1 + edgeRadius * aiVector3D(cos(outerAngle1) * cos(innerAngle1),
                                                                        sin(outerAngle1) * cos(innerAngle1),
                                                                        sin(innerAngle1));
            aiVector3D o1i2 = outerInfluence1 + edgeRadius * aiVector3D(cos(outerAngle1) * cos(innerAngle2),
                                                                        sin(outerAngle1) * cos(innerAngle2),
                                                                        sin(innerAngle2));
            aiVector3D o2i1 = outerInfluence2 + edgeRadius * aiVector3D(cos(outerAngle2) * cos(innerAngle1),
                                                                        sin(outerAngle2) * cos(innerAngle1),
                                                                        sin(innerAngle1));
            aiVector3D o2i2 = outerInfluence2 + edgeRadius * aiVector3D(cos(outerAngle2) * cos(innerAngle2),
                                                                        sin(outerAngle2) * cos(innerAngle2),
                                                                        sin(innerAngle2));
            
            // counter-clockwise order
            // verts 1, 2, 3
            //glVertex3f(o1i1.x,o1i1.y,o1i1.z);
            //glVertex3f(o2i1.x,o2i1.y,o2i1.z);
            //glVertex3f(o2i2.x,o2i2.y,o2i2.z);
            
            my_vertices[6*(j + innerSegments*i) + 0].position = o1i1;
            my_vertices[6*(j + innerSegments*i) + 1].position = o2i1;
            my_vertices[6*(j + innerSegments*i) + 2].position = o2i2;
            
            // specify normal directions
            my_vertices[6*(j + innerSegments*i) + 0].normal = o1i1 - outerInfluence1;
            my_vertices[6*(j + innerSegments*i) + 1].normal = o2i1 - outerInfluence2;
            my_vertices[6*(j + innerSegments*i) + 2].normal = o2i2 - outerInfluence2;

            // verts 3, 4, 1
            //glVertex3f(o2i2.x,o2i2.y,o2i2.z);
            //glVertex3f(o1i2.x,o1i2.y,o1i2.z);
            //glVertex3f(o1i1.x,o1i1.y,o1i1.z);
            
            my_vertices[6*(j + innerSegments*i) + 3].position = o2i2;
            my_vertices[6*(j + innerSegments*i) + 4].position = o1i2;
            my_vertices[6*(j + innerSegments*i) + 5].position = o1i1;
            
            // specify normal directions
            my_vertices[6*(j + innerSegments*i) + 3].normal = o2i2 - outerInfluence2;
            my_vertices[6*(j + innerSegments*i) + 4].normal = o1i2 - outerInfluence1;
            my_vertices[6*(j + innerSegments*i) + 5].normal = o1i1 - outerInfluence1;
            
        }
    }
    
    int shaderNum = 0;
    //AttachMaterial(outer_material, shaderNum);
    RenderVertices(my_vertices, num_vertices, outer_material, shaderNum);    
    delete [] my_vertices;
    glPopMatrix();
}

void Note::ConstructTorus(aiVector3D center, float centerRadius, float edgeRadius) {
    glPushMatrix();
    
    glLoadIdentity();
    
    //Draw order:
    // for outerSegment i, draw innerSegment j
    
    float outerAngle1, outerAngle2, innerAngle1, innerAngle2;
    
    for (int i = 0; i < t_outerSegments; i++) {
        outerAngle1 = i*2*M_PI/t_outerSegments; 
        outerAngle2 = (i+1)*2*M_PI/t_outerSegments;
        
        for (int j = 0; j < t_innerSegments; j++) {
            innerAngle1 = j*2*M_PI/t_innerSegments; 
            innerAngle2 = (j+1)*2*M_PI/t_innerSegments;
            
            aiVector3D outerInfluence1 = centerRadius * aiVector3D( cos(outerAngle1), sin(outerAngle1), 0.0);
            aiVector3D outerInfluence2 = centerRadius * aiVector3D( cos(outerAngle2), sin(outerAngle2), 0.0);
            
            // here x and y components vary both by inner and outer angles! z component only by inner angle.
            
            //positon = center + outerInfluence + innerInfluence
            aiVector3D o1i1 = outerInfluence1 + edgeRadius * aiVector3D(cos(outerAngle1) * cos(innerAngle1),
                                                                        sin(outerAngle1) * cos(innerAngle1),
                                                                        sin(innerAngle1));
            aiVector3D o1i2 = outerInfluence1 + edgeRadius * aiVector3D(cos(outerAngle1) * cos(innerAngle2),
                                                                        sin(outerAngle1) * cos(innerAngle2),
                                                                        sin(innerAngle2));
            aiVector3D o2i1 = outerInfluence2 + edgeRadius * aiVector3D(cos(outerAngle2) * cos(innerAngle1),
                                                                        sin(outerAngle2) * cos(innerAngle1),
                                                                        sin(innerAngle1));
            aiVector3D o2i2 = outerInfluence2 + edgeRadius * aiVector3D(cos(outerAngle2) * cos(innerAngle2),
                                                                        sin(outerAngle2) * cos(innerAngle2),
                                                                        sin(innerAngle2));
            
            // counter-clockwise order
            // verts 1, 2, 3
            //glVertex3f(o1i1.x,o1i1.y,o1i1.z);
            //glVertex3f(o2i1.x,o2i1.y,o2i1.z);
            //glVertex3f(o2i2.x,o2i2.y,o2i2.z);
            
            torus_vertices[6*(j + t_innerSegments*i) + 0].position = o1i1;
            torus_vertices[6*(j + t_innerSegments*i) + 1].position = o2i1;
            torus_vertices[6*(j + t_innerSegments*i) + 2].position = o2i2;
            
            // specify normal directions
            torus_vertices[6*(j + t_innerSegments*i) + 0].normal = o1i1 - outerInfluence1;
            torus_vertices[6*(j + t_innerSegments*i) + 1].normal = o2i1 - outerInfluence2;
            torus_vertices[6*(j + t_innerSegments*i) + 2].normal = o2i2 - outerInfluence2;
            
            // verts 3, 4, 1
            //glVertex3f(o2i2.x,o2i2.y,o2i2.z);
            //glVertex3f(o1i2.x,o1i2.y,o1i2.z);
            //glVertex3f(o1i1.x,o1i1.y,o1i1.z);
            
            torus_vertices[6*(j + t_innerSegments*i) + 3].position = o2i2;
            torus_vertices[6*(j + t_innerSegments*i) + 4].position = o1i2;
            torus_vertices[6*(j + t_innerSegments*i) + 5].position = o1i1;
            
            // specify normal directions
            torus_vertices[6*(j + t_innerSegments*i) + 3].normal = o2i2 - outerInfluence2;
            torus_vertices[6*(j + t_innerSegments*i) + 4].normal = o1i2 - outerInfluence1;
            torus_vertices[6*(j + t_innerSegments*i) + 5].normal = o1i1 - outerInfluence1;
            
        }
    }
    glPopMatrix();
}

void Note::RenderPremadeTorus() {
    glPushMatrix();
    
    glTranslatef(centerPosition.x, centerPosition.y, centerPosition.z);
    
    // why these numbers?
    float scale_pcnt = radius / default_radius;
    glScalef( scale_pcnt, scale_pcnt, 1.0);
    
    int shaderNum = 0;
    //AttachMaterial(outer_material, shaderNum);
    RenderVertices(torus_vertices, t_num_vertices, outer_material, shaderNum);
    
    glPopMatrix();
}

void Note::AttachMaterial(CustomMaterial * mat, int shaderNum) {
    GLint diffuse = glGetUniformLocation((*shaders)[shaderNum]->programID(), "Kd");
    glUniform3f(diffuse, mat->diff_color[0], mat->diff_color[1], mat->diff_color[2]);
    
    // Specular material
    GLint specular = glGetUniformLocation((*shaders)[shaderNum]->programID(), "Ks");
    glUniform3f(specular, mat->spec_color[0], mat->spec_color[1], mat->spec_color[2]);
    
    // Ambient material
    GLint ambient = glGetUniformLocation((*shaders)[shaderNum]->programID(), "Ka");
    glUniform3f(ambient, mat->amb_color[0], mat->amb_color[1], mat->amb_color[2]);
    
    // Specular power
    GLint shininess = glGetUniformLocation((*shaders)[shaderNum]->programID(), "alpha");
    glUniform1f(shininess, mat->shiny);
}

void Note::AttachVertices(CustomVertex * my_vertices, int num_vertices, int shaderNum) {
    GLint position = glGetAttribLocation((*shaders)[shaderNum]->programID(), "positionIn");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, 0, sizeof(CustomVertex), &my_vertices->position);
    
    GLint normal = glGetAttribLocation((*shaders)[shaderNum]->programID(), "normalIn");
    glEnableVertexAttribArray(normal);
    glVertexAttribPointer(normal, 3, GL_FLOAT, 0, sizeof(CustomVertex), &my_vertices->normal);
}



void Note::RenderVertices(CustomVertex * my_vertices, int num_vertices, CustomMaterial * mat,int shaderNum) {
    
    AttachVertices(my_vertices, num_vertices, shaderNum);
    AttachMaterial(mat, shaderNum);

    glUseProgram((*shaders)[shaderNum]->programID());
    glDrawArrays(GL_TRIANGLES,0,num_vertices); 
}

bool Note::maybe() {
    return (float)rand()/RAND_MAX > .5;
}

