//
//  Note.h
//  GraphScore
//
//  Created by phunter on 11/3/11.
//  Copyright 2011 Hunter McCurry. All rights reserved.
//

#ifndef GraphScore_Note_h
#define GraphScore_Note_h

#include "stgl.h"
#include "stglut.h"
#include <vector>

#include "STPoint2.h"
#include "STPoint3.h"
#include "STVector3.h"
#include "STColor4f.h"

// #include "Connection.h"

using namespace std;

class Note;

struct Connection {
    Note *next_note;
    float ideal_dist;
    int time_count;
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
    void IncrementTimeCount();
    void TrimOldConnections();
    
    STPoint3 * getLocation();
    int getMappedMidi();
    Spelling SpellNote(int mapped_midi_num);
    
    float easeRamp(float input);
    float easeBump(float input);
    
    void Nudge(STVector3 displacement);
    void MoveFromConnections();
    void MoveFromDissonance(Note *other, float diss_val);
    void AttractFromDissonance(Note *other, float diss_val);
    void RepelFrom(Note *other);
    void AttractToZ();
    
    void DisplayNotes(float h);
    void DisplayConnections(float h);
    bool IsConnectedTo(int mapped_midi_num);
    void DrawConnections();
    void StaffLines();
    void NoteHead();
    void DrawFlat();
    void DrawSharp();
    void DrawCircle(STPoint3 cent, float rad, int numVerts, bool filled);
    
    bool maybe();
    
private:
    // constant
    STColor4f color;
    STPoint3 centerPosition;
    float radius;
    float width_max;
    int max_connections;
    int max_connection_time;
    
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
