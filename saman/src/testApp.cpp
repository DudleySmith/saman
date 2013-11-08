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
    m_pnSettings.add(m_btVerbose.ofxToggle::setup("Verbose", true));

    m_pnSettings.add(m_lblAnimParams.setup("Animations", ""));
    m_pnSettings.add(m_pxDropDurationMin.set("DropDuration Min", 2, 0, 10));
    m_pnSettings.add(m_pxDropDurationMax.set("DropDuration Max", 2, 0, 10));
    m_pnSettings.add(m_pxDropSmoothness.set("DropSmoothness", 0.1, 0, 1));
    m_pnSettings.add(m_pxDispGenCorrection.set("DispGenCorrection", 0.05, 0, 0.1));
    m_pnSettings.add(m_pxDispNodeCorrection.set("DispNodeCorrection", 0.05, 0, 0.1));

    m_pnSettings.setWidthElements(300);
    m_pnSettings.loadFromFile("settings.xml");
    
    // TEST ---------------------------------------------------------------------------
    m_pnTest.setup("Test","settings.xml", ofGetWidth() - 220, 250);

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
    
    // GUI --
    m_lbHost = m_oOsc.getHost();
    m_lbSet = m_oOsc.getSet();
    updateGui();
    
    // Update network : Send / Read
    m_oXbees.update();
    
    // OSC
    updateOscInput();
    
    // Dispaly Mouse position
    m_lbMousePos = ofToString(ofGetMouseX()) + ":" + ofToString(ofGetMouseY());
    
}

//--------------------------------------------------------------
void testApp::updateOscInput(){
    
    string nameKey = "";
    double value0 = 0;
    double duration = 0;
    
    // --
    m_oOsc.update();
    
    // --
    float genIntensity = m_oOsc.getConstValue("/params/general/intensity");
    float genCenter = m_oOsc.getConstValue("/params/general/center");
    float genDispersion = m_oOsc.getConstValue("/params/general/disp");
    float genDispersionMin = m_oOsc.getConstValue("/params/general/dispMin");
    
    genDispersion = ofMap(genDispersion, 0, 1, m_pxDispGenCorrection, 1);
    
    int idxNodePin = 0;
    int idxGeneralPin = 0;

    // --
    map<string, ofxXbeeNode>::iterator oneNode;
    map<string, ofxXbeeNode>           nodes = m_oXbees.getNodes();
    
    for (oneNode=nodes.begin(); oneNode!=nodes.end(); oneNode++) {
        
        // --
        float nodeMix = m_oOsc.getConstValue("/params/"+(*oneNode).first+"/mix");
        float nodeIntensity = m_oOsc.getConstValue("/params/"+(*oneNode).first+"/intensity");
        float nodeCenter = m_oOsc.getConstValue("/params/"+(*oneNode).first+"/center");
        float nodeDispersion = m_oOsc.getConstValue("/params/"+(*oneNode).first+"/disp");
        float nodeDispersionMin = m_oOsc.getConstValue("/params/"+(*oneNode).first+"/dispMin");
        
        nodeDispersion = ofMap(nodeDispersion, 0, 1, m_pxDispNodeCorrection, 1);
        
        // Check all messages --
        map<int, ofxXbeeNodePin>::iterator onePin;
        map<int, ofxXbeeNodePin>           pins = m_oXbees.getNodes()[(*oneNode).first].getPins();
        idxNodePin = 0;
        
        for (onePin=pins.begin(); onePin!=pins.end(); onePin++) {
            
            float nodePinRatio = (float)idxNodePin/(float)pins.size();
            float generalPinRatio = (float)idxGeneralPin/(float)(pins.size()*nodes.size());

            
            // --
            nameKey = (*oneNode).first+"/"+ofToString((*onePin).first);
            //ofLogVerbose() << nameKey << "node Ratio : " << ofToString(nodePinRatio) << "general Ratio : " << ofToString(generalPinRatio);
            // --
            value0 = m_oOsc.getEvent("drops", nameKey);
            if (value0>0) {
                // ----------------------------------------------------
                // DROP ANIMATION -------------------------------------
                // --
                duration = ofMap(value0, 0, 1, m_pxDropDurationMax, m_pxDropDurationMin);
                // --
                //ofLogVerbose() << " :" << duration;
                m_oXbees.animateDrop((*oneNode).first, (*onePin).first, duration);
                
            }else{
                // ----------------------------------------------------
                // GLOBAL COMMAND -------------------------------------
                // --
                float nodePinValue = 0;
                float genPinValue = 0;
                
                // --
                if(nodePinRatio>=nodeCenter && nodePinRatio<=nodeCenter+nodeDispersion){
                    nodePinValue = ofMap(nodePinRatio, nodeCenter, nodeCenter+nodeDispersion, 1, nodeDispersionMin, true);
                    
                }else if(nodePinRatio>=nodeCenter-nodeDispersion && nodePinRatio<=nodeCenter){
                    nodePinValue = ofMap(nodePinRatio, nodeCenter, nodeCenter-nodeDispersion, 1, nodeDispersionMin, true);
                    
                }else{
                    nodePinValue = 0;
                    
                }
                nodePinValue *= nodeIntensity;
                //--
                
                // --
                if(generalPinRatio>=genCenter && generalPinRatio<=genCenter+genDispersion){
                    genPinValue = ofMap(generalPinRatio, genCenter, genCenter+genDispersion, 1, genDispersionMin, true);
                    
                }else if(generalPinRatio>=genCenter-genDispersion && generalPinRatio<=genCenter){
                    genPinValue = ofMap(generalPinRatio, genCenter, genCenter-genDispersion, 1, genDispersionMin, true);
                    
                }else{
                    genPinValue = 0;
                }
                genPinValue *= genIntensity;
                //--
                
                float realValue = nodeMix*nodePinValue + (1-nodeMix)*genPinValue;
                
                //ofLogVerbose() << " : " << (*oneNode).first<< " : " <<(*onePin).first<< " : " << realValue;
                m_oXbees.setNodeAllStrip((*oneNode).first, (*onePin).first, realValue);
                
            }
            
            idxNodePin++;
            idxGeneralPin++;
            
        }
        
    }

    
    
    
    
}

//--------------------------------------------------------------
void testApp::updateGui(){
    
    
    int    iCardID = m_slCardID;
    string sCardID = ofToString(iCardID, 0, 4, '0');
    /*
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
            m_oXbees.animateDrop(sCardID, (*onePin).first, m_pxDropDurationMin);
        }
    }
    
    // Drop One Pin -------------------
    if (m_btOneDrop==true) {
        m_oXbees.animateDrop(sCardID, m_slPinNumber, m_pxDropDurationMin);
    }
    */
    
    // Verbose log ?
    if(m_btVerbose==true){
        ofSetLogLevel(OF_LOG_VERBOSE);
    }else{
        ofSetLogLevel(OF_LOG_ERROR);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
    m_oBackgroundImage.draw(0, 0);
    
    // GUI --
    if (m_bDisplayGui==true) {
        // Xbee network drawing
        m_oXbees.draw(true, true);
        
        // Display OSC messages
        list<string> messages = m_oOsc.getRoughMessages();
        list<string>::iterator oneMessage;
        int idxMessage = 0;
        
        ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);
        for(oneMessage=messages.begin(); oneMessage!=messages.end(); oneMessage++){
            ofDrawBitmapString((*oneMessage), 0.5*ofGetWidth(), 0.5*ofGetHeight() + 10*(1+idxMessage++));
            ofLogVerbose() << "OSC Message [" << idxMessage << "] : " << ofToString((*oneMessage), 0, 2, '0');
        }

        m_pnSettings.draw();
        m_pnGuiOscSettings.draw();
        m_pnTest.draw();
        
    }else{
        
        m_oXbees.draw(true, false);
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