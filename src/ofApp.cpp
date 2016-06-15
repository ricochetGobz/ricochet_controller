#include "ofApp.h"


// SETUP --------------------------------------------------------------
void ofApp::setup(){
    cout << "ofApp::setup" << endl;

    //// OF INIT ////
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetCircleResolution(60);
    ofSetVerticalSync(true);

    //// SERVER INIT ////
    server_receive.setup(RECEIVER_PORT);
    server = *new ServerController();

    //// KINECT INIT ////
    kinectCtrl.init();

    //// SOUND INIT ////
    vector< ofSoundPlayer>::iterator itSounds = sounds.begin();

    //// GUI INIT ////
    gui.setup("Ricochet - DEBUG", 0, 0, ofGetWidth(), ofGetHeight());
    // --------- PANEL 1 : DEBUG
    gui.setWhichPanel(0);
    // ----- Column 1
    gui.setWhichColumn(0);
    gui.addDrawableRect("Kinect Video", &kinectCtrl.colorImg, OC_WIDTH, OC_HEIGHT);

    // !!TEMP!! //
    //gui.addDrawableRect("Kinect Depth", &kinectCtrl.depthImg, OC_WIDTH, OC_HEIGHT);
    gui.addDrawableRect("Kinect Depth", &kinectCtrl.depthImg, OC_WIDTH, OC_HEIGHT);
    // !!TEMP!! //
    // ----- Column 2
    gui.setWhichColumn(1);
    gui.addDrawableRect("OpenCV Threshold", &kinectCtrl.thresholdImg, OC_WIDTH, OC_HEIGHT);
    gui.addDrawableRect("OpenCV Render", &kinectCtrl.reworkImg, OC_WIDTH*2, OC_HEIGHT*2);
    // ----- Column 3
    gui.setWhichColumn(2);
    // Stats
    gui.addChartPlotter(appFrameRate, 30, 80);
    stats.setName("Stats");
    stats.add( nBlobs.set("Blobs founds", 0) );
    stats.add( nCubes.set("Cubes founds", 0) );
    gui.addVariableLister(stats);
    //Threshold controls
    thresholdControls.setName("OpenCV threshold");
    thresholdControls.add(bDebugVideo.set("Debug Video", false));
    thresholdControls.add(nearThreshold.set("nearThreshold", 255.0, 1.0, 255.0));
    thresholdControls.add(farThreshold.set("farThreshold", 182.0, 1.0, 255.0)); // 182 // 174

    gui.addGroup(thresholdControls);
    // Render controls
    renderControls.setName("OpenCV render");
    renderControls.add(bBlur.set("Blur", false));
    renderControls.add(threshold.set("threshold", 0.0, 0.0, 255.0));

    renderControls.add(minArea.set("minArea", 300.0, 1.0, 3000.0)); // ( sauv : 300.0) // 554
    renderControls.add(maxArea.set("maxArea", 1500.0, 1.0, (OC_WIDTH*OC_HEIGHT)));
    gui.addGroup(renderControls);
    cubeDetection.setName("Cube detection");
    cubeDetection.add(sizeCaptured.set("sizeCaptured", 33, 10, 60));
    cubeDetection.add(dilationTolerance.set("dilationTolerance", 8, 0, 15));
    cubeDetection.add(sizeTolerance.set("sizeTolerance", 7, 0, 30));
    gui.addGroup(cubeDetection);
}


// UPDATE --------------------------------------------------------------
// CHECK RECEIVED ADDRESS --------------------------------------------------------------
void ofApp::update(){
    //// GUI UPDATE /////
    gui.update();
    appFrameRate = ofGetFrameRate();
    nBlobs = kinectCtrl.contourFinder.nBlobs;

    //// SERVER UPDATE : OSC MESSAGE RECEIVED ////
    while(server_receive.hasWaitingMessages()){
        ofxOscMessage m;
        server_receive.getNextMessage(&m);
        string address = m.getAddress();
        cout << address << endl;

        // switch not support string
        checkReceivedAddress(address, m);

    }

    //// KINECT UPDATE ////
    kinectCtrl.update(bDebugVideo,
                      bBlur,
                      gui.getValueI("OpenCV_threshold:nearThreshold"),
                      gui.getValueI("OpenCV_threshold:farThreshold"),
                      gui.getValueI("OpenCV_render:threshold"),
                      gui.getValueI("OpenCV_render:minArea"),
                      gui.getValueI("OpenCV_render:maxArea"));

    // Check kinect connection and send information to the server
    if(kinectConnected != kinectCtrl.kinectIsConnected()){
        kinectConnected = kinectCtrl.kinectIsConnected();
        server.sendKinectStatusChange(kinectConnected);
    }

    //// CHECK CUBE DETECTED BY THE KINECT AND UPDATE CUBES LIFES ////
    cubeManager.update(kinectCtrl.contourFinder,
                       gui.getValueI("Cube_detection:dilationTolerance"),
                       gui.getValueI("Cube_detection:sizeTolerance"),
                       gui.getValueI("Cube_detection:sizeCaptured"));

    // check chrono activation
    if(cubeManager.chronoIsActivate()){
        cout << "chrono activate" << endl;
        server.sendStartChono();
    }

    // TEMPS
    checkNbrOfCubeFound();
}

void ofApp::checkNbrOfCubeFound() {
    int _nCubesFound = cubeManager.getNbrCubesFound();
    if(_nCubesFound != nCubes) {
        // TODO send to the server
        nCubes = _nCubesFound;
        server.sendNbrOfCubeFoundChange(nCubes);
    }
}

void ofApp::checkReceivedAddress(string _address, ofxOscMessage _m) {
    if(_address == SERVER_CONNECTED ) {
        cout << "Node server started" << endl;
        if(!serverStarted){
            cout << "err" << endl;
            serverStarted = true;
            server.sendOFStatusChange(true);
            server.sendKinectStatusChange(kinectConnected);
        }
    } else if ( _address == SERVER_DISCONNECTED ) {
        cout << "Node server down : " << endl;
        serverStarted = false;
        webRenderConnected = false;
        cubeManager.removeAllConnectedCubes();
    } else if( _address == WEB_RENDER_CONNECTED ){
        cout << "web render connected : " << endl;
        webRenderConnected = true;
    } else if ( _address == WEB_RENDER_DISCONNECTED ){
        cout << "web render disconnected : " << endl;
        webRenderConnected = false;
    } else if( _address == CUBE_CONNECTED){
        cout << "cube connected : " << _m.getArgAsInt(0) << ", " << _m.getArgAsInt(1) << endl;
        cubeManager.cubeConnected(_m.getArgAsInt(0), _m.getArgAsInt(1));
    } else if( _address == CUBE_DISCONNECTED) {
        cout << "cube disconnected : " << _m.getArgAsInt(0) << endl;
        cubeManager.cubeDisconnected(_m.getArgAsInt(0));
    } else if( _address == CUBE_TOUCHED) {
        cout << "cube touched : " << _m.getArgAsInt(0) << ", " << _m.getArgAsInt(1) << endl;
        cubeManager.cubeTouched(_m.getArgAsInt(0), _m.getArgAsInt(1));
    } else if( _address == CUBE_DRAGGED){
        cout << "cube dragged : " << _m.getArgAsInt(0) << endl;
        cubeManager.cubeDragged(_m.getArgAsInt(0));
    } else if( _address == CUBE_DRAG_END){
        cout << "cube drag end : "  << _m.getArgAsInt(0) << ", " << _m.getArgAsInt(1) << endl;
        cubeManager.cubeDragEnd(_m.getArgAsInt(0), _m.getArgAsInt(1));

        // Check nbr for cube found to the tutorial
        checkNbrOfCubeFound();
    } else if( _address == CUBE_FACE_CHANGED){
        cout << "cube face changed : "  << _m.getArgAsInt(0) << ", " << _m.getArgAsInt(1) << endl;
        cubeManager.cubeFaceChanged(_m.getArgAsInt(0), _m.getArgAsInt(1));
    }
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
            gui.draw();
            kinectCtrl.drawControlPanel(renderZone);
            cubeManager.draw(renderZone);
            break;
        case CLOUD_MODE:
            kinectCtrl.drawPointCloud();
            break;
        default:
            break;
    }

    ofSetColor(255, 255, 255);
    //// CUBES CONNECTED INFORMATION ////
    stringstream cubeConnectedStream = cubeManager.getConnectedCubesStatus();
    ofDrawBitmapString(cubeConnectedStream.str(), 10, ofGetHeight() - 360);
    //// CONNECTION INFORMATION ////
    stringstream reportStream;
    if(bDebugVideo) reportStream << "DEBUG MODE" << endl;
    reportStream << "Node.js Server: " << ((serverStarted)?"ON":"OFF") << endl
    << "Web Render: " << ((webRenderConnected)?"ON":"OFF") << endl
    << "Kinect: " << ((kinectConnected)?"ON":"OFF - press (o) :: try to connect the kinect.") << endl << endl
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
    cubeManager.mouseReleased(x, y);
}
// !!TEMP!! //


void ofApp::exit(){
    server.sendOFStatusChange(false);
}
