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
    
    // Network --
    m_oXbees.setup(m_pxConnection);
    m_oXbees.addNode("1");
    m_oXbees.addNode("2");
    m_oXbees.addNode("3");
    m_oXbees.addNode("4");
    m_oXbees.addNode("5");
    m_oXbees.addNode("6");
    m_oXbees.addNode("7");
    m_oXbees.addNode("8");
    
}

//--------------------------------------------------------------
void testApp::setupAnims(){
    
}

//--------------------------------------------------------------
void testApp::setupGui(){
    
    
    // SETTINGS ---------------------------------------------------------------------------
    m_pnSettings.setup("Settings", "settings.xml", ofGetWidth() - 220, 10);
    
    m_pnSettings.add(m_lbConnections.setup("Connections PXws", ""));
    m_pnSettings.add(m_pxDataPath.set("Data Path", "none, don't care"));
    m_pnSettings.add(m_pxConnection.set("USB Connection", "tty.usbserial-A600KMNU"));
    m_pnSettings.add(m_pxOscPort.set("OSC Port", "9000"));

    m_pnSettings.add(m_lblAnimParams.setup("Animations", ""));
    m_pnSettings.add(m_pxDropDuration.set("DropDuration", 2, 0, 10));
    m_pnSettings.add(m_pxDropSmoothness.set("DropSmoothness", 0.1, 0, 1));

    m_pnSettings.setWidthElements(300);
    m_pnSettings.loadFromFile("settings.xml");
    
    // TEST ---------------------------------------------------------------------------
    m_pnTest.setup("Test","settings.xml", ofGetWidth() - 220, 180);

    m_pnTest.add(m_slCardID.setup("Card ID", 0, 0, 10));
    
    m_pnTest.add(m_lbAll.setup("All pins", ""));
    m_pnTest.add(m_btAllDrop.setup("Drop All"));
    m_pnTest.add(m_btAllWholeStrip.setup("Full All"));
    m_pnTest.add(m_btBlinks.setup("Blink All"));
    
    m_pnTest.add(m_lbOne.setup("One Pin only", ""));
    m_pnTest.add(m_slPinNumber.setup("Pin number", 2, 2, 13));
    m_pnTest.add(m_btOneDrop.setup("Drop One pin"));
    m_pnTest.add(m_btOneWholeStrip.setup("Full One pin"));
    
}


//--------------------------------------------------------------
void testApp::update(){
    // GUI --
    updateGui();
    // Anims --
    updateAnims();
    // Update network : Send / Read
    m_oXbees.update();
}



//--------------------------------------------------------------
void testApp::updateAnims(){
    
}

//--------------------------------------------------------------
void testApp::updateGui(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    // GUI --
    m_pnSettings.draw();
    m_pnTest.draw();
    
    // Xbee network drawing
    m_oXbees.draw();
    
        
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    ofLogVerbose() << "KEY PRESSED : " << key;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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