#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAnimatableFloat.h"
#include "samanNetwork.h"

class testApp : public ofBaseApp{
	
public:
    void setup();
    void setupGui();
    void setupAnims();
    
    void update();
    void updateGui();
    void updateAnims();
    
    void draw();
    void drawXbeeNodes();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    // PARAMETERS --------------------------------------
    ofxPanel    m_pnSettings;
    
    ofxLabel    m_lbConnections;
    ofParameter<string> m_pxDataPath;
    ofParameter<string> m_pxConnection;
    ofParameter<string> m_pxOscPort;
    
    ofxLabel   m_lblAnimParams;
    ofParameter<float>  m_pxDropDuration;
    ofParameter<float>  m_pxDropSmoothness;
    
    // TEST --------------------------------------
    ofxPanel        m_pnTest;
    ofxIntSlider    m_slCardID;
    
    ofxLabel       m_lbAll;
    ofxButton       m_btAllDrop;
    ofxButton       m_btAllWholeStrip;
    ofxButton       m_btBlinks;
    
    ofxLabel       m_lbOne;
    ofxIntSlider    m_slPinNumber;
    ofxButton       m_btOneDrop;
    ofxButton       m_btOneWholeStrip;
    
    // Animations object to drive boards -------------------
    map<string, ofxAnimatableFloat> m_aAnims;
    // Networks of arduino boards
    samanNetwork                  m_oXbees;
    
};

