//
//  Note.h
//  GraphScore
//
//  Created by phunter on 11/3/11.
//  Copyright 2011 Hunter McCurry. All rights reserved.
//

#ifndef GraphScore_Note_h
#define GraphScore_Note_h

#include "Framework.h"
//#include "stgl.h"
//#include "stglut.h"
#include <vector>

//#include "STPoint2.h"
//#include "STPoint3.h"
//#include "STVector3.h"
//#include "STColor4f.h"

// #include "Connection.h"

using namespace std;

class Note;

struct Connection {
    Note *next_note;
    float ideal_dist;
    float time_count;
};

struct Spelling {
    int staff_offset;
    int accidental;
};


class Note {
public:
    Note(float x, float y, float z, int mapped_midi_num, float s, int t);
    
    float getExcite();
    void setExcite(float e);
    
    void addTwoWayConnection(Note *note, float dist);
    void addConnection(Note *note, float dist);
    void updateConnection(int mapped_midi_num, float dist);
    void updateTwoWayConnection(int mapped_midi_num, float dist);
    void DeleteConnection(int mapped_midi_num);
    void DeleteTwoWayConnection(int mapped_midi_num);
    
    void ResetTimeCount(int mapped_midi_num);
    void IncrementTimeCount(float delta);
    void TrimOldConnections();
    
    aiVector3D * getLocation();
    int getMappedMidi();
    Spelling SpellNote(int mapped_midi_num);
    
    float easeRamp(float input);
    float easeBump(float input);
    
    void Nudge(aiVector3D displacement);
    void MoveFromConnections(float delta);
    void MoveFromDissonance(Note *other, float diss_val, float delta);
    void AttractFromDissonance(Note *other, float diss_val, float delta);
    void RepelFrom(Note *other, float delta);
    void AttractToZ(float delta);
    
    void DisplayNotes(float h);
    void DisplayConnections(float h);
    bool IsConnectedTo(int mapped_midi_num);
    void DrawConnections();
    void DrawStaffLines();
    void NoteHead();
    void DrawFlat();
    void DrawSharp();
    void DrawCircle(aiVector3D cent, float rad, int numVerts, bool filled);
    
    void DrawCylinder(aiVector3D endOne, aiVector3D endTwo, float radius, int slices);
    void DrawTorus(aiVector3D center, float centerRadius, float edgeRadius, int segments, int segmentFaces);
    
    bool maybe();
    
private:
    // constant
    aiColor4D color;
    aiVector3D centerPosition;
    float radius;
    float width_max;
    int max_connections;
    float max_connection_time;
    
    int mapped_midi;
    // for now, 0 is middle line, +or- 4 is top/bottom line
    // maximum range is -13 <= note_space <= 13
    int note_space; 
    // shows where relative to the bubble center notehead is drawn (calculated from note_space
    int rel_space;  
    int accidental; // 0 is non, -1 is flat, +1 is shar
    
    float speed;
    
    float cam_height;
    
    // varying
    float excitement;
    
    // connections
//    vector<Note*> next_notes;
//    vector<float> ideal_dists;
    
    vector<Connection*> connection_list;
    };

#endif
