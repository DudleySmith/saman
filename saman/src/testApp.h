#pragma once

#define dataPath "../../../../../saman_commons/datas/"

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxEasyOsc.h"
#include "samanNetwork.h"

class testApp : public ofBaseApp{
	
public:
    void setup();
    void setupGui();
    
    void update();
    void updateGui();
    void updateOscInput();
    
    void draw();
    void drawBackground();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    // -------------------------------------------------
    // GUI ---------------------------------------------
    bool        m_bDisplayGui;
    
    // PARAMETERS --------------------------------------
    ofxPanel    m_pnSettings;
    
    ofxLabel    m_lbConnections;
    ofParameter<string> m_pxDataPath;
    ofParameter<string> m_pxConnection;
    ofParameter<string> m_pxBackgroundImage;
    ofxToggle           m_btVerbose;
    
    ofxLabel   m_lblAnimParams;
    ofParameter<float>  m_pxDropDurationMin;
    ofParameter<float>  m_pxDropDurationMax;
    ofParameter<float>  m_pxDropSmoothness;
    ofParameter<float>  m_pxDispGenCorrection;
    ofParameter<float>  m_pxDispNodeCorrection;
    
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
    
    ofxLabel       m_lbMousePos;
    // -------------------------------------------------
    
    // Networks of arduino boards
    samanNetwork                  m_oXbees;
    
    // Background
    ofImage m_oBackgroundImage;
    
    // Osc Shit
    ofxEasyOSc  m_oOsc;
    ofxPanel    m_pnGuiOscSettings;
    ofxLabel    m_lbHost;
    ofxLabel    m_lbSet;
    
};

