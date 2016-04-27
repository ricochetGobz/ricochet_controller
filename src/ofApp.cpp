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
    
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetCircleResolution(60);
    
    server_receive.setup(RECEIVER_PORT);
    server = *new ServerController();
    
    // Init the kinect after OSC servers
    kinect.init();
    
    /// SOUND INIT ////
    //sounds.push_back(*new ofSoundPlayer);
    vector< ofSoundPlayer>::iterator itSounds = sounds.begin();
    
    ///// CUBE INIT ////
    /* Pushback, pour entrer un objet dans mon tableaux d'objet "cube".     */
    for(int i = 0; i < nCube; i++) {
        cubes.push_back(*new Cube(ofPoint((ofGetWidth()*i/nCube) + 50, 50.0), i));
        cubes[i].loadSound("./sounds/note_" + std::to_string((i%6)+1) +".mp3");
    }

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
    
    
    // Cube update
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
    kinect.draw();
    
    // COMMUNICATION INFORMATION
    stringstream reportStream;
    reportStream << "Node.js Server: " << ((server.isStarted())?"ON":"OFF") << endl
    << "Web Render: " << ((server.webRenderIsConnected())?"ON":"OFF") << endl
    << "Kinect: " << ((server.kinectIsConnected())?"ON":"OFF - press (o) :: try to connect the kinect.") << endl;
    ofDrawBitmapString(reportStream.str(), 10, 20);
    
    
    // CUBES TEMPS
    //
    
    
    //Draw table
    //ofSetColor(255, 255, 255);
    //ofDrawCircle(512, 512, 500);
    
    for(vector<Cube>::iterator it = cubes.begin(); it != cubes.end(); ++it){
        (*it).draw();
    }
    
    for (vector<EchoContainer>::iterator it = echoContainers.begin(); it != echoContainers.end(); ++it) {
        (*it).draw();
    }
    
    if(cubeDragged > 0 && mouseDown){
        cubes[cubeDragged].contactZoneShowed = true;
    }
    
    ofFill();

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
    //    for(vector<ricochetCube>::iterator it = cubes.begin(); it != cubes.end(); ++it) {
    
    if(mouseDown) {
        mouseMove = true;
        cout << " Cube Draged" << endl;
        cubes[cubeDragged].moveTo(ofPoint(x, y));
    }
    //    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //server.sendPlayCube(-1, -1, x, y);
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
    
    /* ici on peut utiliser un iterrateur dans une boucle for pour pointer vers tout les objet du tableau cube */
    /*  L'avantage des iterateurs si j'ai bien compris, c'est de pointŽ directement sans avoir ˆ assigner un int pour aller retrouvŽ la value,
     du coup c'est plus rapide */
    //    for(vector<ricochetCube>::iterator it = cubes.begin(); it != cubes.end(); ++it){
    
    if(mouseDown) {
        cout << " Cube Clicked" << endl;
        
        if(!mouseMove) createEchoContainer(cubes[cubeDragged]);
        mouseDown = false;
        mouseMove = false;
        
        // Init contactZone
        cubes[cubeDragged].contactZoneShowed = false;
        cubeDragged = -1;
    }
    //    }

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

void ofApp::createEchoContainer(Cube _cube){
    
    EchoContainer newEchoContainer = *new EchoContainer(_cube);
    _cube.play();
    
    echoContainers.push_back(newEchoContainer);
}
