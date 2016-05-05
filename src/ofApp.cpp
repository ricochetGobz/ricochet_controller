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

    // !!TEMP!! //
    //// CUBE INIT ////
    /* Pushback, pour entrer un objet dans mon tableaux d'objet "cube".     */
    for(int i = 0; i < nCube; i++) {
        cubes.push_back(*new Cube(ofPoint((ofGetWidth()*i/nCube) + 50, 50.0), i, server));
        cubes[i].loadSound("./sounds/note_" + std::to_string((i%6)+1) +".mp3");
    }
    // !!TEMP!! //
}


// UPDATE --------------------------------------------------------------
// - SHOULDREMOVE (for ofRemove) --------------------------------------------------------------
bool shouldRemove(EchoContainer &c) {
    if(c.echoes.size() == 0) {
        return true;
    }
    return false;
}
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
    kinectCtrl.update(mode);
    // Check kinect connection and send information to the server
    if(server.kinectIsConnected() != kinectCtrl.kinectIsConnected()){
        server.setKinectStatus(kinectCtrl.kinectIsConnected());
        server.sendKinectStatusChange(server.kinectIsConnected());
    }

    //// BLOBS CHECKING ////
    // TODO tester les blobs détectés par la kinect

    //// CUBES UPDATE ////
    // TODO désincrémenter le livecicle

    //// ECHOES UPDATE ////
    // check if echoContainers are alive
    ofRemove(echoContainers, shouldRemove);
    for (vector<EchoContainer>::iterator itWave = echoContainers.begin(); itWave != echoContainers.end(); ++itWave) {
        (*itWave).update();
        for(vector<Cube>::iterator it = cubes.begin(); it != cubes.end(); ++it){
            (*itWave).checkEchoCollision((*it));
        }
    }
}

// DRAW --------------------------------------------------------------
void ofApp::draw(){
    ofBackground(100, 100, 100);

    //// KINECT RENDER DRAW ////
    kinectCtrl.draw();

    //// CUBES INFORMATIONS ////
    if(mode == NORMAL_MODE) {
        // Cubes detected
        for(vector<Cube>::iterator it = cubes.begin(); it != cubes.end(); ++it){
            (*it).draw();
        }
        // echoes
        for (vector<EchoContainer>::iterator it = echoContainers.begin(); it != echoContainers.end(); ++it) {
            (*it).draw();
        }

        // !!TEMP!! //
        if(cubeDragged >= 0 && mouseDown){
            cubes[cubeDragged].contactZoneShowed = true;
        }
        // !!TEMP!! //
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
    } else if(key == 'o'){
        kinectCtrl.open();
    }
}

// !!TEMP!! //
// MOUSE DRAGGED --------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(mouseDown) {
        mouseMove = true;
        cout << " Cube Draged" << endl;
        cubes[cubeDragged].moveTo(ofPoint(x, y));
    }
}
// MOUSE PRESSED --------------------------------------------------------------
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
// MOUSE RELEASED --------------------------------------------------------------
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
// !!TEMP!! //


void ofApp::exit(){
    server.sendOFStatusChange(false);
}
