//
//  Graph.h
//  GraphScore
//
//  Created by Hunter McCurry on 2/2/12.
//  Copyright (c) 2012 Hunter McCurry. All rights reserved.
//

#ifndef GraphScore_Graph_h
#define GraphScore_Graph_h

#include "Framework.h"
#include <vector>

#include "Note.h"
#include "Dissonance.h"
#include "Shader.h"

using namespace std;

//struct Spelling {
//    int staff_offset;
//    int accidental;
//};

class Graph {
public:
    Graph(int max_size_, Dissonance *diss_mat_, std::vector<Shader*> *shaders_);
    
    aiVector3D * GetLocation(int mapped_midi);
    Note * GetNote(int mapped_midi);
    Note * GetCurrentNote();
    void SetCurrentNote(int mapped_midi);
    void GetCurConnections(vector<int> * note_list);
    void GetConnections(int note_number, vector<int> * connection_list);
    
    void AddConnectExcite(int mapped_midi, float dist);
    void AddNote(int mapped_midi, aiVector3D start_pos);
    void TriggerConnectedAudio();
    
    void UpdateGraph(float timeDelta);
    void RepelAll(float delta);
    void MoveAllFromConnections(float delta);
    void MoveFromDissonance(float delta);
    void AttractFromDissonance(float delta);
    void AttractToXYPlane(float delta);
    void IncrementTimeCounts(float delta);
    void TrimOldConnections();
    
    void Display(float cam_height);
    
    void Render();
    
    void ExciteNote();
    void FadeColors();
	void IncreaseDeadness();
    
	void Reset();
    void Clear();
    
private:
    
    bool empty;
    
    int max_size;
    int cur_size;
    int current_note;
    
    int cur_note;
    int prev_note;

    Dissonance * diss_mat;
    vector <Note*> note_graph;
    
    std::vector<Shader*> *shaders;
};


#endif
