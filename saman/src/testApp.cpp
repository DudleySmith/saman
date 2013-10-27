#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	
	ofBackground(255);
	ofSetLogLevel(OF_LOG_VERBOSE);
    
    // Change the data path
    ofEnableDataPath();
    ofSetDataPathRoot(dataPath);
    
    ofDirectory  dirDataPath = ofToDataPath("");
    if(dirDataPath.exists() && !dirDataPath.isDirectoryEmpty("")){
        ofLogVerbose() << dirDataPath.getAbsolutePath() << " exists and full of files" ;
    }else{
        ofLogError() << dirDataPath.getAbsolutePath() << ": It's emtpy or not exists.";
    }
    
    // GUI --
    setupGui();
    m_pxDataPath = dirDataPath.getAbsolutePath();
    
    // BACKGROUND --
    m_oBackgroundImage.loadImage(m_pxBackgroundImage);
    if(m_oBackgroundImage.isAllocated()){
        ofSetWindowShape(m_oBackgroundImage.width, m_oBackgroundImage.height);
    }
    
    // Network --
    m_oXbees.setup(m_pxConnection);
    
    m_oXbees.setupANode("0001");
    m_oXbees.setupANode("0002");
    m_oXbees.setupANode("0003");
    m_oXbees.setupANode("0004");
    m_oXbees.setupANode("0005");
    m_oXbees.setupANode("0006");
    m_oXbees.setupANode("0007");
    m_oXbees.setupANode("0008");
    
    // OSC and its gui
    m_oOsc.loadParameters();
    m_pnGuiOscSettings.setup(m_oOsc.getSettings());
    m_pnGuiOscSettings.add(m_lbHost.setup("Host OSC", m_oOsc.getHost()));
    m_pnGuiOscSettings.add(m_lbSet.setup("Set OSC",m_oOsc.getSet()));
    m_pnGuiOscSettings.setPosition(10, 60);
    m_pnGuiOscSettings.loadFromFile("settings.xml");
    m_oOsc.setup(EASYOSC_IN);
    
}


//--------------------------------------------------------------
void testApp::setupGui(){
    
    m_bDisplayGui = false;
    
    // SETTINGS ---------------------------------------------------------------------------
    m_pnSettings.setup("others", "settings.xml", ofGetWidth() - 220, 10);
    
    m_pnSettings.add(m_lbConnections.setup("Connections PXws", ""));
    m_pnSettings.add(m_pxDataPath.set("Data Path", "none, don't care"));
    m_pnSettings.add(m_pxConnection.set("USB Connection", "tty.usbserial-A600KMNU"));
    m_pnSettings.add(m_pxBackgroundImage.set("Background Image", "background.jpg"));

    m_pnSettings.add(m_lblAnimParams.setup("Animations", ""));
    m_pnSettings.add(m_pxDropDuration.set("DropDuration", 2, 0, 10));
    m_pnSettings.add(m_pxDropSmoothness.set("DropSmoothness", 0.1, 0, 1));

    m_pnSettings.setWidthElements(300);
    m_pnSettings.loadFromFile("settings.xml");
    
    // TEST ---------------------------------------------------------------------------
    m_pnTest.setup("Test","settings.xml", ofGetWidth() - 220, 180);

    m_pnTest.add(m_slCardID.setup("Card ID", 1, 1, 10));
    
    m_pnTest.add(m_lbAll.setup("All pins", ""));
    m_pnTest.add(m_btAllDrop.setup("Drop All"));
    m_pnTest.add(m_btAllWholeStrip.setup("Full All"));
    
    m_pnTest.add(m_lbOne.setup("One Pin only", ""));
    m_pnTest.add(m_slPinNumber.setup("Pin number", 2, 2, 13));
    m_pnTest.add(m_btOneDrop.setup("Drop One pin"));
    m_pnTest.add(m_btOneWholeStrip.setup("Full One pin"));
    
    m_pnTest.add(m_lbMousePos.setup("Mouse", ""));
    
}


//--------------------------------------------------------------
void testApp::update(){
    /*
    // test update
    string sControl = "";
    map<string, ofxXbeeNode>::iterator  oneNode;
    map<string, ofxXbeeNode>            nodes = m_oXbees.getNodes();
    for (oneNode=nodes.begin(); oneNode!=nodes.end(); oneNode++) {
        // Control pins of a node
        map<int, ofxXbeeNodePin>::iterator  onePin;
        map<int, ofxXbeeNodePin>            pins = (*oneNode).second.getPins();
        
        for (onePin=pins.begin(); onePin!=pins.end(); onePin++) {
            sControl = "Node : " + (*oneNode).first + " : " +  "Pin : " + ofToString((*onePin).first) + " : " + "Value = " + ofToString((*onePin).second.getAValue(0));
            if((*onePin).second.getAValue(0) > 0){
                ofLogVerbose() << "Van de diou, c'est bon ca !";
            }
            //ofLogVerbose() << sControl;
        }
        
        //ofLogVerbose() << "end";

    }
    */
    // GUI --
    m_lbHost = m_oOsc.getHost();
    m_lbSet = m_oOsc.getSet();

    updateGui();
    // Anims --
    updateAnims();
    // Update network : Send / Read
    m_oXbees.update();
    // OSC
    m_oOsc.update();
    // Dispaly Mouse position
    m_lbMousePos = ofToString(ofGetMouseX()) + ":" + ofToString(ofGetMouseY());
    
}



//--------------------------------------------------------------
void testApp::updateAnims(){
    
}

//--------------------------------------------------------------
void testApp::updateGui(){
    
    
    int    iCardID = m_slCardID;
    string sCardID = ofToString(iCardID, 0, 4, '0');
    
    // FULL -------------------------
    if (m_btAllWholeStrip==true) {
        // Light On all pins of the node
        map<int, ofxXbeeNodePin>::iterator onePin;
        map<int, ofxXbeeNodePin>           pins = m_oXbees.getNodes()[sCardID].getPins();
        
        for (onePin=pins.begin(); onePin!=pins.end(); onePin++) {
            m_oXbees.setNodeAllStrip(sCardID, (*onePin).first, 1);
        }
    }else{
        // Light On all pins of the node
        map<int, ofxXbeeNodePin>::iterator onePin;
        map<int, ofxXbeeNodePin>           pins = m_oXbees.getNodes()[sCardID].getPins();
        
        for (onePin=pins.begin(); onePin!=pins.end(); onePin++) {
            m_oXbees.setNodeAllStrip(sCardID, (*onePin).first, 0);
        }
    }
    // FULL Strip / One Pin -------------------
    if (m_btOneWholeStrip==true) {
        m_oXbees.setNodeAllStrip(sCardID, m_slPinNumber, 1);
    }else{
        m_oXbees.setNodeAllStrip(sCardID, m_slPinNumber, 0);
    }
    
    // DROP Whole node -------------------------
    if (m_btAllDrop==true) {
        // Light On all pins of the node
        map<int, ofxXbeeNodePin>::iterator onePin;
        map<int, ofxXbeeNodePin>           pins = m_oXbees.getNodes()[sCardID].getPins();
        
        for (onePin=pins.begin(); onePin!=pins.end(); onePin++) {
            m_oXbees.animateDrop(sCardID, (*onePin).first, m_pxDropDuration);
        }
    }
    
    // Drop One Pin -------------------
    if (m_btOneDrop==true) {
        m_oXbees.animateDrop(sCardID, m_slPinNumber, m_pxDropDuration);
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    m_oBackgroundImage.draw(0, 0);
    
    // Xbee network drawing
    m_oXbees.draw(true, true);
    
    // GUI --
    if (m_bDisplayGui==true) {
        m_pnSettings.draw();
        m_pnGuiOscSettings.draw();
        m_pnTest.draw();
    }else{
        ofDrawBitmapString("$ to show GUI", 10, ofGetHeight() - 10);
    }
    
}

//--------------------------------------------------------------
void testApp::drawBackground(){

    // First Draw background
    int imageW;
    int imageH;
    float imageRatioW_H;
    
    int realW;
    int realH;
    
    imageW = m_oBackgroundImage.width;
    imageH = m_oBackgroundImage.height;
    imageRatioW_H = (float)imageW / (float)imageH;
    
    realW   = ofGetWidth();
    realH   = ofGetWidth()/imageRatioW_H;
    
    ofLogVerbose() << imageW << ":" << imageH << ":" << imageRatioW_H << ":" << realW << ":" << realH;
    

    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    ofLogVerbose() << "KEY PRESSED : " << key;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
    ofLogVerbose() << "KEY RELEASED : " << key;
    
    if (key == 93) {
        m_bDisplayGui = !m_bDisplayGui;
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}