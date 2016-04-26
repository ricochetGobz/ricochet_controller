#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    cout << "setup" << endl;
    
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetCircleResolution(60);
    
    server_receive.setup(RECEIVER_PORT);
    server = *new ServerController();
    
    // Init the kinect after OSC servers
    kinect.init();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //NODE BRIDGE CHECK MESSAGE RECEIVE
    while(server_receive.hasWaitingMessages()){
        ofxOscMessage m;
        server_receive.getNextMessage(&m);
        string address = m.getAddress();
        cout << address << endl;
        
        server.checkAddress(address);
    }
    
    // Kinect update
    if(server.kinectIsConnected() != kinect.kinectIsConnected()){
        server.setKinectStatus(kinect.kinectIsConnected());
        server.sendKinectStatusChange(server.kinectIsConnected());
    }
    
    kinect.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(100, 100, 100);
    ofSetColor(255, 255, 255);
    kinect.draw();
    
    // COMMUNICATION INFORMATION
    stringstream reportStream;
    reportStream << "Node.js Server: " << ((server.isStarted())?"ON":"OFF") << endl
    << "Web Render: " << ((server.webRenderIsConnected())?"ON":"OFF") << endl
    << "Kinect: " << ((server.kinectIsConnected())?"ON":"OFF - press (o) :: try to connect the kinect.") << endl;
    ofDrawBitmapString(reportStream.str(), 10, 20);
}

void ofApp::exit(){
    server.sendOFStatusChange(false);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    kinect.onKeyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    server.sendPlayCube(-1, -1, x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
