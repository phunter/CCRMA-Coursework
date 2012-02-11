//
//  Connection.h
//  GraphScore
//
//  Created by Hunter McCurry on 2/5/12.
//  Copyright (c) 2012 Hunter McCurry. All rights reserved.
//

#ifndef GraphScore_Connection_h
#define GraphScore_Connection_h

#include "Note.h"

struct Connection {
    Note *next_note;
    float ideal_dist;
};

#endif
