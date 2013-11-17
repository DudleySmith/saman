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
    m_oXbees.setupANode(m_lbRecep01);
    m_oXbees.setupANode(m_lbRecep02);
    m_oXbees.setupANode(m_lbRecep03);
    m_oXbees.setupANode(m_lbRecep04);
    m_oXbees.setupANode(m_lbRecep05);
    m_oXbees.setupANode(m_lbRecep06);
    m_oXbees.setupANode(m_lbRecep07);
    m_oXbees.setupANode(m_lbRecep08);
    
    // OSC and its gui
    m_oOsc.loadParameters();
    m_pnGuiOscSettings.setup(m_oOsc.getSettings());
    m_pnGuiOscSettings.add(m_lbHost.setup("Host OSC", m_oOsc.getHost()));
    m_pnGuiOscSettings.add(m_lbSet.setup("Set OSC",m_oOsc.getSet()));
    m_pnGuiOscSettings.add(m_pxMaxMessagesOsc.set("MaxMessagesOsc", 64, 0, 128));
    m_pnGuiOscSettings.add(m_pxMaxMessagesXbee.set("MaxMessagesXbee", 64, 0, 128));
    
    m_pnGuiOscSettings.setPosition(10, 650);
    m_pnGuiOscSettings.loadFromFile("settings.xml");
    m_oOsc.setup(EASYOSC_IN);
    
    ofSetFrameRate(30);
    
}


//--------------------------------------------------------------
void testApp::setupGui(){
    
    m_bDisplayGui = false;
    
    // SETTINGS ---------------------------------------------------------------------------
    m_pnSettings.setup("others", "settings.xml", 10, 10);
    
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

    m_pnSettings.loadFromFile("settings.xml");
    
    // TEST ---------------------------------------------------------------------------
    m_pnTest.setup("Test","settings.xml", 10, 250);

    m_pnTest.add(m_slCardID.setup("Card ID", 1, 1, 10));
    
    m_pnTest.add(m_lbAll.setup("All pins", ""));
    m_pnTest.add(m_btAllDrop.setup("Drop All"));
    m_pnTest.add(m_btAllWholeStrip.setup("Full All"));
    
    m_pnTest.add(m_lbOne.setup("One Pin only", ""));
    m_pnTest.add(m_slPinNumber.setup("Pin number", 2, 2, 13));
    m_pnTest.add(m_btOneDrop.setup("Drop One pin"));
    m_pnTest.add(m_btOneWholeStrip.setup("Full One pin"));
    
    m_pnTest.add(m_lbMousePos.setup("Mouse", ""));
    
    // Devices -----------------------------------
    m_pnDevices.setup("Devices","devices.xml", 10, 450);

    m_pnDevices.add(m_lbEmitter.setup("Emitter", "xxxx"));
    
    m_pnDevices.add(m_lbRecep01.setup("Recep 01", "0001"));
    m_pnDevices.add(m_lbRecep02.setup("Recep 02", "0002"));
    m_pnDevices.add(m_lbRecep03.setup("Recep 03", "0003"));
    m_pnDevices.add(m_lbRecep04.setup("Recep 04", "0004"));
    m_pnDevices.add(m_lbRecep05.setup("Recep 05", "0005"));
    m_pnDevices.add(m_lbRecep06.setup("Recep 06", "0006"));
    m_pnDevices.add(m_lbRecep07.setup("Recep 07", "0007"));
    m_pnDevices.add(m_lbRecep08.setup("Recep 08", "0008"));
    
    m_pnDevices.loadFromFile("devices.xml");
    // -------------------------------------------------
    
}


//--------------------------------------------------------------
void testApp::update(){
    
    // GUI --
    m_lbHost = m_oOsc.getHost();
    m_lbSet = m_oOsc.getSet();
    updateGui();
    
    // OSC
    updateOscInput();
    
    // Update network : Send / Read
    m_oXbees.update(m_pxMaxMessagesXbee);
    
    // Dispaly Mouse position
    m_lbMousePos = ofToString(ofGetMouseX()) + ":" + ofToString(ofGetMouseY());
    
}

//--------------------------------------------------------------
void testApp::updateOscInput(){
    
    string nameKey = "";
    float value0 = 0;
    double duration = 0;
    
    // --
    m_oOsc.update(m_pxMaxMessagesOsc);
    
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
    
    float stripLightness = 0;
    
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
        map<int, ofxXbeeNodePin>           pins = (*oneNode).second.getPins();
        idxNodePin = 0;
        
        for (onePin=pins.begin(); onePin!=pins.end(); onePin++) {
            
            stripLightness = 0;
            
            float nodePinRatio = (float)idxNodePin/(float)pins.size();
            float generalPinRatio = (float)idxGeneralPin/(float)(pins.size()*nodes.size());
            
            // --
            nameKey = (*oneNode).first+"/"+ofToString((*onePin).first);
            // --
            bool isDropEvent = m_oOsc.getEvent("drops", nameKey, value0);
            if (isDropEvent) {
                // ----------------------------------------------------
                // DROP ANIMATION -------------------------------------
                // --
                duration = ofMap(value0, 0, 1, m_pxDropDurationMax, m_pxDropDurationMin);
                // --
                ofLogVerbose() << " :" << duration;
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
                
                stripLightness = nodeMix*nodePinValue + (1-nodeMix)*genPinValue;
                ofLogVerbose() << " : " << (*oneNode).first<< " : " <<(*onePin).first<< " : " << stripLightness;
                
            }
            
            // On envoie alors le message ---------------------------------
            // Si une animation est en cours, c'est de la goutte de pluie, sinon c'est whole strip
            string keyAnim = (*oneNode).first + ":" + ofToString((*onePin).first, 0, 2, '0') + ":Drop";
            map<string, ofxAnimatableFloat>::iterator   oneAnim;
            
            oneAnim = m_oXbees.m_aAnims.find(keyAnim);
            
            if(oneAnim != m_oXbees.m_aAnims.end()){
                if((*oneAnim).second.isAnimating()){
                    m_oXbees.sendNodeDrop((*oneNode).first, (*onePin).first, (*oneAnim).second.getCurrentValue());
                    m_oXbees.setNodeDrop((*oneNode).first, (*onePin).first, (*oneAnim).second.getCurrentValue());
                }else{
                    m_oXbees.sendNodePwm((*oneNode).first, (*onePin).first, stripLightness);
                    m_oXbees.setNodeAllStrip((*oneNode).first, (*onePin).first, stripLightness);
                }
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
    
    // Verbose log ?
    if(m_btVerbose==true){
        ofSetLogLevel(OF_LOG_VERBOSE);
    }else{
        ofSetLogLevel(OF_LOG_ERROR);
    }

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

}

//--------------------------------------------------------------
void testApp::draw(){
    
    m_oBackgroundImage.draw(0, 0);
    
    // GUI --
    if (m_bDisplayGui==true) {
        // Xbee network drawing
        m_oXbees.draw(false, true);
        
        // Display OSC messages
        list<string> messages = m_oOsc.getRoughMessages();
        list<string>::iterator oneMessage;
        int idxMessage = 0;
        
        ofPushMatrix();
        ofTranslate(ofGetWidth()*0.5, 10);
        
        ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);
        for(oneMessage=messages.begin(); oneMessage!=messages.end(); oneMessage++){
            ofDrawBitmapString((*oneMessage), 100, 0.5*ofGetHeight() + 10*(1+idxMessage++));
            ofLogVerbose() << "OSC Message [" << idxMessage << "] : " << ofToString((*oneMessage), 0, 2, '0');
        }
        
        ofPopMatrix();
        
        m_pnSettings.draw();
        m_pnGuiOscSettings.draw();
        m_pnTest.draw();
        m_pnDevices.draw();
        
    }else{
        
        ofDrawBitmapString("$ to show GUI", 10, ofGetHeight() - 10);
    }
    
    // Arbres � gauche
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep01), ofPoint(10,100), 200, 340);
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep02), ofPoint(130,182), 150, 300);
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep03), ofPoint(210,266), 130, 240);
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep04), ofPoint(280,310), 130, 200);
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep05), ofPoint(350,385), 110, 160);
    
    // Arbres � droite
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep06), ofPoint(505,350), 190, 240);
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep07), ofPoint(710,290), 250, 300);
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep08), ofPoint(900,130), 320, 450);
    
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