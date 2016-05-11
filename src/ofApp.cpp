#include "ofApp.h"

// SETUP --------------------------------------------------------------
void ofApp::setup(){
    cout << "ofApp::setup" << endl;

    //// OF INIT ////
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetCircleResolution(60);

    //// SERVER INIT ////
    server_receive.setup(RECEIVER_PORT);
    server = *new ServerController();

    //// KINECT INIT ////
    kinectCtrl.init();

    //// SOUND INIT ////
    vector< ofSoundPlayer>::iterator itSounds = sounds.begin();
}


// UPDATE --------------------------------------------------------------
void ofApp::update(){

    //// SERVER UPDATE : OSC MESSAGE RECEIVED ////
    while(server_receive.hasWaitingMessages()){
        ofxOscMessage m;
        server_receive.getNextMessage(&m);
        string address = m.getAddress();
        cout << address << endl;

        server.checkAddress(address);
    }

    //// KINECT UPDATE ////
    kinectCtrl.update();
    // Check kinect connection and send information to the server
    if(server.kinectIsConnected() != kinectCtrl.kinectIsConnected()){
        server.setKinectStatus(kinectCtrl.kinectIsConnected());
        server.sendKinectStatusChange(server.kinectIsConnected());
    }

    //// CHECK CUBE DETECTED BY THE KINECT AND UPDATE CUBES LIFES ////
    cubeManager.update(kinectCtrl.detectedCubes);

}

// DRAW --------------------------------------------------------------
void ofApp::draw(){
    ofBackground(100, 100, 100);

    //// DRAW KINECT RENDER && OPENCV RESULT ////
    switch(mode) {
        case NORMAL_MODE:
            renderZone.set(2, 2, ofGetWidth() - 4, ofGetHeight() - 4);
            
            kinectCtrl.drawRender(renderZone);
            cubeManager.draw(renderZone);
            break;
        case CALIBRATION_MODE:
            renderZone.set(227, 246, OC_WIDTH*2, OC_HEIGHT*2);
            
            kinectCtrl.drawControlPanel(renderZone);
            cubeManager.draw(renderZone);
            break;
        case CLOUD_MODE:
            kinectCtrl.drawPointCloud();
            break;
        default:
            break;
    }
    

    //// CONNECTION INFORMATION ////
    ofSetColor(255, 255, 255);
    stringstream reportStream;
    reportStream << "Node.js Server: " << ((server.isStarted())?"ON":"OFF") << endl
    << "Web Render: " << ((server.webRenderIsConnected())?"ON":"OFF") << endl
    << "Kinect: " << ((server.kinectIsConnected())?"ON":"OFF - press (o) :: try to connect the kinect.") << endl << endl
    << "press (m) :: switch between modes" << endl;
    ofDrawBitmapString(reportStream.str(), 10, ofGetHeight() - 100);

    ofFill();
}

// EVENTS --------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'm') {
        mode++;
        if(mode == 3){
            mode = 0;
        }
    } else if(key == 'o'){
        kinectCtrl.openKinect();
    }
}

// !!TEMP!! //
// MOUSE RELEASED --------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    cubeManager.onClick(x, y);
    
}
// !!TEMP!! //


void ofApp::exit(){
    server.sendOFStatusChange(false);
}
