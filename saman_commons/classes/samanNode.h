//
//  samanNode.h
//  saman
//
//  Created by Dudley Smith on 22/10/13.
//
//

#pragma once

#include "ofMain.h"
#include "ofxXbeeNode.h"

class samanNode : protected ofxXbeeNode{
public:
    samanNode();
    virtual ~samanNode();
    
public:
    void draw(ofPoint _pos, float _size);
};
