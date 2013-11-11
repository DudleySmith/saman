//
//  samanNetwork.cpp
//  saman
//
//  Created by Dudley Smith on 22/10/13.
//
//

#include "samanNetwork.h"

// -------------------------------------------------
void samanNetwork::draw(bool _drawNodes, bool _drawControl){
    if(_drawNodes)      drawNodes();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, 10);
        if(_drawControl)    drawControl();
    ofPopMatrix();
}

// ------------------------------------------
void samanNetwork::drawNodes(){
    
    // Arbres à gauche
    drawANode(m_aNodes["40A1"], ofPoint(10,100), 200, 340);
    drawANode(m_aNodes["0002"], ofPoint(130,182), 150, 300);
    drawANode(m_aNodes["0003"], ofPoint(210,266), 130, 240);
    drawANode(m_aNodes["0004"], ofPoint(280,310), 130, 200);
    drawANode(m_aNodes["0005"], ofPoint(350,385), 110, 160);
    
    // Arbres à droite
    drawANode(m_aNodes["0006"], ofPoint(505,350), 190, 240);
    drawANode(m_aNodes["0007"], ofPoint(710,290), 250, 300);
    drawANode(m_aNodes["0008"], ofPoint(900,130), 320, 450);
    
}

// ------------------------------------------
void samanNetwork::drawANode(ofxXbeeNode _nodeToDraw, ofPoint _pos, float _xSize, float _ySize){
    
    ofPoint startPt, endPt;
    
    // Base drawing
    _nodeToDraw.draw(_pos, _xSize, _ySize);
    // Additive drawing
    ofPushMatrix();
    ofPushStyle();
    ofSetLineWidth(3);
    
    ofTranslate(_pos);
    
    for(int idxPin=2; idxPin<=11; idxPin++){
        switch (idxPin) {
            case 2:
                // PIN 2 --------------------------------------------------------------------
                startPt.set(_xSize*0.45f, _ySize*1.0f);
                endPt.set(_xSize*0.43f, _ySize*0.6f);
                break;
                
            case 3:
                // PIN 3 --------------------------------------------------------------------
                startPt.set(_xSize*0.50f, _ySize*0.85f);
                endPt.set(_xSize*0.55f, _ySize*0.48f);
                break;
                
            case 4:
                // PIN 4 --------------------------------------------------------------------
                startPt.set(_xSize*0.15f, _ySize*0.7f);
                endPt.set(_xSize*0.4f, _ySize*0.9f);
                break;
                
            case 5:
                // PIN 5 --------------------------------------------------------------------
                startPt.set(_xSize*0.9f, _ySize*0.48f);
                endPt.set(_xSize*0.52f, _ySize*0.7f);
                break;
                
            case 6:
                // PIN 6 --------------------------------------------------------------------
                startPt.set(_xSize*0.0f, _ySize*0.35f);
                endPt.set(_xSize*0.46f, _ySize*0.51f);
                break;
                
            case 7:
                // PIN 7 --------------------------------------------------------------------
                startPt.set(_xSize*0.05f, _ySize*0.15f);
                endPt.set(_xSize*0.40f, _ySize*0.47f);
                break;
                
            case 8:
                // PIN 8 --------------------------------------------------------------------
                startPt.set(_xSize*0.25f, _ySize*0.15f);
                endPt.set(_xSize*0.52f, _ySize*0.47f);
                break;
                
            case 9:
                // PIN 9 --------------------------------------------------------------------
                startPt.set(_xSize*0.30f, _ySize*0.1f);
                endPt.set(_xSize*0.52f, _ySize*0.42f);
                break;
                
            case 10:
                // PIN 10 --------------------------------------------------------------------
                startPt.set(_xSize*0.75f, _ySize*0.08f);
                endPt.set(_xSize*0.47f, _ySize*0.38f);
                break;
                
            case 11:
                // PIN 11 --------------------------------------------------------------------
                startPt.set(_xSize*0.82f, _ySize*0.36f);
                endPt.set(_xSize*0.54f, _ySize*0.52f);
                
                break;
                
            default:
                break;
        }
        
        // --------------------------------------------------------------------
        ofSetColor(ofColor::black, 50);
        ofLine(startPt, endPt);
        // --------------------------------------------------------------------

        if (_nodeToDraw.getPins()[idxPin].getMode() == pinModePwm) {
            // --------------------------------------------------------------------
            ofSetColor(ofColor::white, 255*_nodeToDraw.getPins()[idxPin].getAValue(0));
            ofLine(startPt, endPt);
            // --------------------------------------------------------------------
            
        } else if (_nodeToDraw.getPins()[idxPin].getMode() == pinModeDrop) {
            
            float   dropRatio = _nodeToDraw.getPins()[idxPin].getAValue(0);
            ofPoint dropPosition = dropRatio*startPt + (1-dropRatio)*endPt;
            
            // --------------------------------------------------------------------
            ofSetColor(ofColor::white);
            if(dropRatio<1) ofCircle(dropPosition, 10);
            // --------------------------------------------------------------------
            
        } else {
            // --------------------------------------------------------------------
        }
        
        
    }
    
    
    ofPopStyle();
    ofPopMatrix();
    
    
}

//--------------------------------------------------------------
void samanNetwork::setupANode(string _nodeID){
    
    addNode(_nodeID);

    setupANodePin(_nodeID, 2);
    setupANodePin(_nodeID, 3);
    setupANodePin(_nodeID, 4);
    setupANodePin(_nodeID, 5);
    setupANodePin(_nodeID, 6);
    setupANodePin(_nodeID, 7);
    setupANodePin(_nodeID, 8);
    setupANodePin(_nodeID, 9);
    setupANodePin(_nodeID, 10);
    setupANodePin(_nodeID, 11);

}


// ----------------------------------------------------------------
void samanNetwork::setupANodePin(string _nodeID, int _pinNumber){
    
    string  animKey = _nodeID + ":" + ofToString(_pinNumber, 0, 2, '0') + ":Drop";
    
    // Setup a pin
    registerNodePin(_nodeID, _pinNumber, pinModePwm);
    
    m_aAnims[animKey] = ofxAnimatableFloat();
    m_aAnims[animKey].setRepeatType(PLAY_ONCE);
    m_aAnims[animKey].setCurve(EASE_IN);
    
}

// --------------------------------------------------------------------
void samanNetwork::update(){
    
    float dt = 1.0f / ofGetFrameRate();
    
    map<string, ofxAnimatableFloat>::iterator oneAnim;
    for (oneAnim = m_aAnims.begin(); oneAnim != m_aAnims.end(); oneAnim++) {
        
        // Update aniamtions
        (*oneAnim).second.update(dt);
        
        if((*oneAnim).second.isAnimating()){
            vector<string> wholeKey = ofSplitString((*oneAnim).first, ":");
            string  ID = wholeKey[0];
            int     pin = ofToInt(wholeKey[1]);
            
            setNodeDropPosition(ID, pin, (*oneAnim).second.val());
            
        }
        
    }
    
    // Old
    ofxXbeeNetwork::update();
    
    
}

// -----------------------------------------------------------------
void samanNetwork::animateDrop(string _nodeID, int _pinNumber, float _dropDuration){
    m_aAnims[_nodeID+":"+ofToString(_pinNumber, 0, 2, '0')+":Drop"].setDuration(_dropDuration);
    m_aAnims[_nodeID+":"+ofToString(_pinNumber, 0, 2, '0')+":Drop"].animateFromTo(0, 1);
}

// -----------------------------------------------------------------
ofxXbeeNode samanNetwork::getANode(string _nodeID){
    

    map<string, ofxXbeeNode>            nodes = getNodes();
    map<string, ofxXbeeNode>::iterator  nodeToReturn = nodes.find(_nodeID);
    
    if (nodeToReturn != nodes.end()) {
        return (*nodeToReturn).second;
    }else{
        return ofxXbeeNode();
    }
    
    
}

