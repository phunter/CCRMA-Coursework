//
//  Graph.cpp
//  GraphScore
//
//  Created by Hunter McCurry on 2/2/12.
//  Copyright (c) 2012 Hunter McCurry. All rights reserved.
//

#include "Graph.h"

Graph::Graph(int max_size_, Dissonance * diss_mat_, std::vector<Shader*> *shaders_) {
    max_size = max_size_;
    note_graph.resize(max_size);
    diss_mat = diss_mat_;
    
    cur_size = 0;
    current_note = -1;
    
    shaders = shaders_;
    //midi_offset = midi_offset_;
}

aiVector3D * Graph::GetLocation(int mapped_midi) {
    return note_graph[mapped_midi]->getLocation();
}

Note * Graph::GetNote(int mapped_midi) {
    return note_graph[mapped_midi];
}

Note * Graph::GetCurrentNote() {
    return note_graph[current_note];
}

void Graph::SetCurrentNote(int mapped_midi) {
    current_note = mapped_midi;
}

void Graph::GetCurConnections(vector<int> * note_list) {
    if (current_note != -1 && note_graph[current_note] != NULL) {
        note_graph[current_note]->GetCurConnections(note_list);
    }
}

void Graph::GetConnections(int note_number, vector<int> * connection_list) {
    if (note_number != -1 && note_graph[note_number] != NULL) {
        note_graph[note_number]->GetCurConnections(connection_list);
    }
}

void Graph::AddConnectExcite(int mapped_midi, float dist) {
    
    //printf("distance is %f\n",dist);
    
    // Ff the note doesn't exist yet, add it
    if (note_graph[mapped_midi] == NULL) {
        
        // Calculate a starting position for new note
        float xdir = (float)rand()/RAND_MAX - .5;
        float ydir = (float)rand()/RAND_MAX - .5;
        float zdir = (float)rand()/RAND_MAX - .5;        
        
        aiVector3D new_dir = aiVector3D(xdir, ydir, zdir);
        new_dir.Normalize();
        
        //STVector3 new_dir = STVector3((float)rand()/RAND_MAX - .5, (float)rand()/RAND_MAX - .5, 0.0);
        
        aiVector3D new_pos;
        if (cur_size == 0 || current_note == -1) {
            new_pos = new_dir * dist;
            //new_pos = aiVector3D(0.0,0.0,0.0);
        }
        else {
            new_pos = *note_graph[cur_note]->getLocation() + .5 * dist * new_dir;
        }
            
        AddNote(mapped_midi, new_pos);
    }

    // if it's a new note, make it the current note
    if (cur_note != mapped_midi) {
        prev_note = cur_note;
        cur_note = mapped_midi;
    }
    
    // check to see if the new current note is connected to the previous note
    // and if not, connect the two notes
    if (cur_size > 1 && prev_note != -1) {
        if (!note_graph[cur_note]->IsConnectedTo(prev_note)) {
            note_graph[cur_note]->addTwoWayConnection(note_graph[prev_note], dist);
            //note_graph[cur_note]->addConnection(note_graph[prev_note], dist);
            
        }
		else if ( prev_note == -1) {
			
		}
        else { // update connection length between current and previous note
            //note_graph[cur_note]->updateConnection(prev_note, dist);
            note_graph[cur_note]->updateTwoWayConnection(prev_note, dist);
        }
    }
    
    ExciteNote();
}

void Graph::AddNote(int mapped_midi, aiVector3D start_pos) {
    
	if (cur_note == -1) {
  		cur_note = mapped_midi; // hack to get back to start
	}
    prev_note = cur_note;
    cur_note = mapped_midi;
    
    note_graph[cur_note] = new Note(start_pos.x,
                                    start_pos.y,
                                    start_pos.z,
                                    mapped_midi,
                                    1.0,  // speed
                                    6.0, // connection time
                                    shaders);
    // set new current note's excite from prev note's excite
    if (cur_size > 0 && current_note != -1) {
        note_graph[cur_note]->setExcite(note_graph[prev_note]->getExcite());
    }    
    cur_size++;
}

void Graph::TriggerConnectedAudio() {
}


void Graph::UpdateGraph(float delta) {
    MoveAllFromConnections(delta);
    
    //MoveFromDissonance(delta);
    AttractFromDissonance(delta);
    
    RepelAll(delta);
    
    AttractToXYPlane(delta);
    
    FadeColors();
	IncreaseDeadness(); // (sap life!)
    
    IncrementTimeCounts(delta);
    TrimOldConnections();
}

void Graph::MoveAllFromConnections(float delta) {
    for (int i = 0; i < max_size; i++) {
        if (note_graph[i] != NULL) {
            note_graph[i]->MoveFromConnections(delta);
        }
    }
}

void Graph::MoveFromDissonance(float delta) {
    for (int i = 0; i < max_size - 1; i++) {
        for (int j = i+1; j < max_size; j++) {
            if (note_graph[i] != NULL && note_graph[j] != NULL) {
                note_graph[i]->MoveFromDissonance(note_graph[j], diss_mat->Get(i, j), delta);
            }
        }
    }
}

void Graph::AttractFromDissonance(float delta) {
    for (int i = 0; i < max_size - 1; i++) {
        for (int j = i+1; j < max_size; j++) {
            if (note_graph[i] != NULL && note_graph[j] != NULL) {
                note_graph[i]->AttractFromDissonance(note_graph[j], diss_mat->Get(i, j), delta);
            }
        }
    }
}

void Graph::RepelAll(float delta) {
    for (int i = 0; i < max_size - 1; i++) {
        for (int j = i+1; j < max_size; j++) {
            if (note_graph[i] != NULL && note_graph[j] != NULL) {
                note_graph[i]->RepelFrom(note_graph[j], delta);
            }
        }
    }
}

void Graph::AttractToXYPlane(float delta) {
    for (int i = 0; i < max_size; i++) {
        if (note_graph[i] != NULL) {
            
            //aiVector3D * loc = note_graph[i]->getLocation();
            //printf("note %d's position is (%f,%f,%f)\n",i,loc->x,loc->y,loc->z);
            
            note_graph[i]->AttractToXY(delta);
        }
    }

}

void Graph::IncrementTimeCounts(float delta) {
    for (int i = 0; i < max_size; i++) {
        if (note_graph[i] != NULL) {
            note_graph[i]->IncrementTimeCount(delta);
        }
    }
}

void Graph::TrimOldConnections() {
    for (int i = 0; i < max_size; i++) {
        if (note_graph[i] != NULL) {
            note_graph[i]->TrimOldConnections();
        }
    }
}

void Graph::Display(float cam_height) {

    // first draw the connections
    for (int i = 0; i < max_size; i++) {
        if (note_graph[i] != NULL) {
            note_graph[i]->DisplayConnections();
        }
    }
    // then draw the notes
    for (int i = 0; i < max_size; i++) {
        if (note_graph[i] != NULL) {
            note_graph[i]->DisplayNotes(cam_height);
        }
    }
}

void Graph::Render() {
    
    // first draw the connections
    for (int i = 0; i < max_size; i++) {
        if (note_graph[i] != NULL) {
            note_graph[i]->RenderConnections();
        }
    }
    
    // then draw the notes
    for (int i = 0; i < max_size; i++) {
        if (note_graph[i] != NULL) {
            note_graph[i]->RenderNote();
        }
    }
}

void Graph::ExciteNote() {
    Note *excite_note = note_graph[cur_note];
    
    //excite_note->setExcite(excite_note->getExcite() + .4);
    excite_note->setExcite(min(excite_note->getExcite() + .1, 1.2));
    //g_notes[1]->Nudge(STVector3( .02*(((float)rand()/RAND_MAX)-.5), .02*(((float)rand()/RAND_MAX)-.5), 0.0));

    //cam->setTarget(g_notes[1]->getLocation());
    
    //computeTravelDist();
}

void Graph::FadeColors() {
    for (int i = 0; i < max_size; i++) {
        if (note_graph[i] != NULL) {
            note_graph[i]->FadeExcite();
        }
    }
}

void Graph::IncreaseDeadness() {
    for (int i = 0; i < max_size; i++) {
        if (note_graph[i] != NULL) {
			note_graph[i]->IncreaseDeadness();
        }
    }	
}

void Graph::Reset() {
    cur_note = -1;
    prev_note = -1;
	current_note = -1;
}

void Graph::Clear() {
    cur_note = NULL;
    prev_note = NULL;
    cur_size = 0;
    current_note = -1;
    
    note_graph.clear();
    note_graph.resize(max_size);
}
