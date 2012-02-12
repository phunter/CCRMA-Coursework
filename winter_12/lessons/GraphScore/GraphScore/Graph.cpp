//
//  Graph.cpp
//  GraphScore
//
//  Created by Hunter McCurry on 2/2/12.
//  Copyright (c) 2012 Hunter McCurry. All rights reserved.
//

#include "Graph.h"

Graph::Graph(int max_size_, Dissonance * diss_mat_) {
    max_size = max_size_;
    note_graph.resize(max_size);
    diss_mat = diss_mat_;
    
    cur_size = 0;
    
    //midi_offset = midi_offset_;
}

STPoint3 * Graph::GetLocation(int mapped_midi) {
    return note_graph[mapped_midi]->getLocation();
}

Note * Graph::GetNote(int mapped_midi) {
    return note_graph[mapped_midi];
}

void Graph::AddConnectExcite(int mapped_midi, float dist) {
    
    if (note_graph[mapped_midi] == NULL) { // if the note doesn't exist yet, add it
        
        // Calculate starting position for new note
        STVector3 new_dir = STVector3((float)rand()/RAND_MAX - .5,(float)rand()/RAND_MAX - .5,0.0);
        new_dir.Normalize();
        
        STPoint3 new_pos;
        if (cur_size == 0) {
            new_pos = STPoint3(0.0,0.0,0.0);
        }
        else {
            new_pos = *note_graph[cur_note]->getLocation() + .1 * dist * new_dir;
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
    if (cur_size > 1) {
        if (!note_graph[cur_note]->IsConnectedTo(prev_note)) {
            note_graph[cur_note]->addTwoWayConnection(note_graph[prev_note], dist);
            
        }
        else { // update connection length between current and previous note
            note_graph[cur_note]->updateConnection(prev_note, dist);
            //note_graph[cur_note]->updateTwoWayConnection(prev_note, dist);
        }
    }
    
    ExciteNote();
}

void Graph::AddNote(int mapped_midi, STPoint3 start_pos) {
    
    prev_note = cur_note;
    cur_note = mapped_midi;
    
//    Spelling spelling = SpellNote(mapped_midi);
    
    note_graph[cur_note] = new Note(start_pos.x,
                                    start_pos.y,
                                    mapped_midi,
                                    .03,
                                    1000);
    cur_size++;
}

void Graph::UpdateGraph() {
    MoveAllFromConnections();
    MoveFromDissonance();
    RepelAll();
    
    IncrementTimeCounts();
    TrimOldConnections();
}

void Graph::MoveAllFromConnections() {
    for (int i = 0; i < max_size; i++) {
        if (note_graph[i] != NULL) {
            note_graph[i]->MoveFromConnections();
        }
    }
}

void Graph::MoveFromDissonance() {
    for (int i = 0; i < max_size - 1; i++) {
        for (int j = i+1; j < max_size; j++) {
            if (note_graph[i] != NULL && note_graph[j] != NULL) {
                note_graph[i]->MoveFromDissonance(note_graph[j], diss_mat->Get(i, j));
            }
        }
    }
}

void Graph::RepelAll() {
    for (int i = 0; i < max_size - 1; i++) {
        for (int j = i+1; j < max_size; j++) {
            if (note_graph[i] != NULL && note_graph[j] != NULL) {
                note_graph[i]->RepelFrom(note_graph[j]);
            }
        }
    }
}

void Graph::IncrementTimeCounts() {
    for (int i = 0; i < max_size; i++) {
        if (note_graph[i] != NULL) {
            note_graph[i]->IncrementTimeCount();
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
            note_graph[i]->DisplayConnections(cam_height);
        }
    }
    // then draw the notes
    for (int i = 0; i < max_size; i++) {
        if (note_graph[i] != NULL) {
            note_graph[i]->DisplayNotes(cam_height);
        }
    }
}

void Graph::ExciteNote() {
    Note *excite_note = note_graph[cur_note];
    
    excite_note->setExcite(min(excite_note->getExcite() + .4, 1.0));
    //g_notes[1]->Nudge(STVector3( .02*(((float)rand()/RAND_MAX)-.5), .02*(((float)rand()/RAND_MAX)-.5), 0.0));

    //cam->setTarget(g_notes[1]->getLocation());
    
    //computeTravelDist();
    
    
}

void Graph::FadeColors() {
    for (int i = 0; i < max_size; i++) {
        if (note_graph[i] != NULL) {
            note_graph[i]->setExcite(max(note_graph[i]->getExcite() - .01, 0.0));
        }
    }
}

void Graph::Clear() {
    cur_note = NULL;
    prev_note = NULL;
    cur_size = 0;
    note_graph.clear();
    note_graph.resize(max_size);
}
