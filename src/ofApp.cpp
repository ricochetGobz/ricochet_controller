#include "ofApp.h"

bool shouldRemove(EchoContainer &c) {
    if(c.echoes.size() == 0) {
        return true;
    }
    return false;
}

//--------------------------------------------------------------
void ofApp::setup(){
    cout << "setup" << endl;
    
    ///// OF INIT ////
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetCircleResolution(60);
    
    ///// SERVER INIT ////
    server_receive.setup(RECEIVER_PORT);
    server = *new ServerController();
    
    ///// KINECT INIT ////
    kinect.init();
    
    /// SOUND INIT ////
    vector< ofSoundPlayer>::iterator itSounds = sounds.begin();
    
    ///// TEMP CUBE INIT ////
    /* Pushback, pour entrer un objet dans mon tableaux d'objet "cube".     */
    for(int i = 0; i < nCube; i++) {
        cubes.push_back(*new Cube(ofPoint((ofGetWidth()*i/nCube) + 50, 50.0), i, server));
        cubes[i].loadSound("./sounds/note_" + std::to_string((i%6)+1) +".mp3");
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    
    ///// SERVER UPDATE : OSC MESSAGE RECEIVED ////
    while(server_receive.hasWaitingMessages()){
        ofxOscMessage m;
        server_receive.getNextMessage(&m);
        string address = m.getAddress();
        cout << address << endl;
        
        server.checkAddress(address);
    }
    
    ///// KINECT UPDATE ////
    if(server.kinectIsConnected() != kinect.kinectIsConnected()){
        server.setKinectStatus(kinect.kinectIsConnected());
        server.sendKinectStatusChange(server.kinectIsConnected());
    }
    
    kinect.update();
    
    
    ///// CUBE UPDATE ////
    ofRemove(echoContainers, shouldRemove);
    
    for (vector<EchoContainer>::iterator itWave = echoContainers.begin(); itWave != echoContainers.end(); ++itWave) {
        (*itWave).update();
        for(vector<Cube>::iterator it = cubes.begin(); it != cubes.end(); ++it){
            (*itWave).checkEchoCollision((*it));
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(100, 100, 100);
    ofSetColor(255, 255, 255);
    
    ///// KINECT DEBUG DRAW ////
    kinect.draw(mode);
    
    ///// COMMUNICATION INFORMATION /////
    stringstream reportStream;
    reportStream << "Node.js Server: " << ((server.isStarted())?"ON":"OFF") << endl
    << "Web Render: " << ((server.webRenderIsConnected())?"ON":"OFF") << endl
    << "Kinect: " << ((server.kinectIsConnected())?"ON":"OFF - press (o) :: try to connect the kinect.") << endl;
    ofDrawBitmapString(reportStream.str(), 10, 20);
    
    
    if(mode == NORMAL_MODE) {
        ///// CUBE POSITION DETECTED /////
        for(vector<Cube>::iterator it = cubes.begin(); it != cubes.end(); ++it){
            (*it).draw();
        }
        
        ///// ECHOES /////
        for (vector<EchoContainer>::iterator it = echoContainers.begin(); it != echoContainers.end(); ++it) {
            (*it).draw();
        }
        
        
        ///// TEMPS /////
        if(cubeDragged >= 0 && mouseDown){
            cubes[cubeDragged].contactZoneShowed = true;
        }
    }

    ofFill();
}

void ofApp::exit(){
    server.sendOFStatusChange(false);
}


// CREATE ECHO CONTAINER -----------------------------------------------
void ofApp::createEchoContainer(Cube _cube){
    EchoContainer newEchoContainer = *new EchoContainer(_cube);
    echoContainers.push_back(newEchoContainer);
}

// EVENTS --------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'm') {
        mode++;
        if(mode == 3){
            mode = 0;
        }
    }
    kinect.onKeyPressed(key);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(mouseDown) {
        mouseMove = true;
        cout << " Cube Draged" << endl;
        cubes[cubeDragged].moveTo(ofPoint(x, y));
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    for(vector<Cube>::iterator it = cubes.begin(); it != cubes.end(); ++it) {
        
        if((*it).pointIsInside(ofPoint(x, y))) {
            cout << " Cube Draged" << endl;
            (*it).moveTo(ofPoint(x, y));
            mouseDown = true;
            cubeDragged = (*it).cubeId;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if(mouseDown) {
    cout << " Cube Clicked" << endl;

    if(!mouseMove) createEchoContainer(cubes[cubeDragged]);
    mouseDown = false;
    mouseMove = false;

    // Init contactZone
    cubes[cubeDragged].contactZoneShowed = false;
    cubeDragged = -1;
    }
}
