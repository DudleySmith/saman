//
//  samanNetwork.cpp
//  saman
//
//  Created by Dudley Smith on 22/10/13.
//
//

#include "samanNetwork.h"

void samanNetwork::draw(){
    
    // Xbee State
    string xBeeFullState = getSerialFullState();
    
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(20, 20);
    
    if(getSerialStatus() == STATUS_Connected){
        ofSetColor(ofColor::green);
    }else if (getSerialStatus() == ""){
        ofSetColor(ofColor::orange);
    }else if (getSerialStatus() == STATUS_NotConnected){
        ofSetColor(ofColor::red);
    }else{
        ofSetColor(ofColor::black);
    }
    
    ofDrawBitmapString(xBeeFullState, 0, 0);
    
    ofPopStyle();
    ofPopMatrix();
    
    for (int idxNode=0; idxNode<m_aNodes.size(); idxNode++) {
        // ---------
    }

    
}