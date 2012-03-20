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
//#include "Connection.h"
#include <vector>

//#include "STPoint2.h"
//#include "STPoint3.h"
//#include "STVector3.h"
//#include "STColor4f.h"

#include "CustomMaterial.h"
#include "CustomVertex.h"
#include "Shader.h"

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
    Note(float x, float y, float z, int mapped_midi_num, float s, int t, std::vector<Shader*> *shaders_);
    ~Note();
    
    float getExcite();
    void setExcite(float e);
    void FadeExcite();
	void IncreaseDeadness();
    
    void GetCurConnections(vector<int> * note_list);
	void BackToLife();
    
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
    float getRadius();
    
    int getMappedMidi();
    Spelling SpellNote(int mapped_midi_num);
    
    float easeRamp(float input);
    float easeBump(float input);
    
    void Nudge(aiVector3D displacement);
    void MoveFromConnections(float delta);
    void MoveFromDissonance(Note *other, float diss_val, float delta);
    void AttractFromDissonance(Note *other, float diss_val, float delta);
    void RepelFrom(Note *other, float delta);
    void AttractToXY(float delta);
    
    void DisplayNotes(float h);
    void DisplayConnections();
    
    void ApplyScale();
    void RenderNote();
    void RenderConnections();
    
    bool IsConnectedTo(int mapped_midi_num);

    void NoteHead();
    void RenderNoteHead();
    
    void DrawFlat();
    void RenderFlat();
    void DrawSharp();
    void RenderSharp();
    
    void DrawCircle(aiVector3D cent, float rad, int numVerts, bool filled);
    void RenderCircle(aiVector3D cent, float rad, float roundness, int numCorners);
    void RenderCircle2(aiVector3D cent, float rad, float roundness, int numCorners);
    
    void DrawStaffLines();
    void RenderStaffLines();
    
    void DrawCylinder(aiVector3D endOne, aiVector3D endTwo, float radius, int slices);
    void RenderCylinder(aiVector3D endOne, aiVector3D endTwo, float radius, int slices);
    
    void DrawTorus(aiVector3D center, float centerRadius, float edgeRadius, int outerSegments, int innerSegments);
    void RenderTorus(aiVector3D center, float centerRadius, float edgeRadius, int outerSegments, int innerSegments);
    
    void ConstructTorus(aiVector3D center, float centerRadius, float edgeRadius);
    void RenderPremadeTorus();
    
    void AttachMaterial(CustomMaterial mat, int shaderNum);
    void AttachVertices(CustomVertex * my_vertices, int num_vertices, int shaderNum);
    
    void RenderVertices(CustomVertex * my_vertices, int num_vertices, CustomMaterial material,int shaderNum);
    bool maybe();
    
private:
    // constant
    aiColor4D color;
    aiVector3D centerPosition;
    float default_radius;
    float radius;
    float line_thickness;
    float width_max;
	int num_connections;
    int max_connections;
    float max_connection_time;
    
	int deadThresh;
	int deadness;
	
    aiVector3D globalScale;
    
    int mapped_midi;
    // for now, 0 is middle line, +or- 4 is top/bottom line
    // maximum range is -13 <= note_space <= 13
    int note_space; 
    // shows where relative to the bubble center notehead is drawn (calculated from note_space
    int rel_space;  
    int accidental; // 0 is non, -1 is flat, +1 is shar
    
	float default_speed;
    float speed;
    
    float cam_height;
    
    // varying
    float excitement;
    
    std::vector<Shader*> *shaders;
    
    // connections
//    vector<Note*> next_notes;
//    vector<float> ideal_dists;
    
    vector<Connection*> connection_list;
    
    // materials
    CustomMaterial default_fill_material;
    CustomMaterial default_outer_material;
	CustomMaterial current_fill_material;
    CustomMaterial current_outer_material;
	
    
    // geometry
    int t_num_vertices;
    int t_outerSegments;
    int t_innerSegments;
    CustomVertex * torus_vertices;
    
    };

#endif
