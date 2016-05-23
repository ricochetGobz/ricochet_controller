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
    //gui.addDrawableRect("Kinect Depth", &_depthImg, OC_WIDTH, OC_HEIGHT);
    // !!TEMP!! //
    gui.addDrawableRect("Kinect Depth", &kinectCtrl.tempVidPlayer, OC_WIDTH, OC_HEIGHT);
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
    thresholdControls.add(nearThreshold.set("nearThreshold", 255.0, 1.0, 255.0)); // (sauv : 165.0)
    thresholdControls.add(farThreshold.set("farThreshold", 146.0, 1.0, 255.0)); // (sauv : 158.0)
    gui.addGroup(thresholdControls);
    // Render controls
    renderControls.setName("OpenCV render");
    renderControls.add(bBlur.set("Blur", false));
    renderControls.add(threshold.set("threshold", 0.0, 0.0, 255.0));
    renderControls.add(minArea.set("minArea", 231.0, 1.0, 3000.0)); // ( sauv : 300.0)
    renderControls.add(maxArea.set("maxArea", 1040.0, 1.0, (OC_WIDTH*OC_HEIGHT)));
    gui.addGroup(renderControls);
    cubeDetection.setName("Cube detection");
    cubeDetection.add(sizeCaptured.set("sizeCaptured", 27, 10, 60));
    cubeDetection.add(dilationTolerance.set("dilationTolerance", 6, 0, 15));
    cubeDetection.add(sizeTolerance.set("sizeTolerance", 9, 0, 30));
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
        checkReceivedAddress(address);
       
    }

    //// KINECT UPDATE ////
    kinectCtrl.update(bBlur,
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
}

void ofApp::checkReceivedAddress(string _address) {
    if(_address == SERVER_CONNECTED ) {
        cout << "Node server started" << endl;
        if(!serverStarted){
            cout << "err" << endl;
            serverStarted = true;
            server.sendOFStatusChange(true);
            server.sendKinectStatusChange(kinectConnected);
        }
        
    } else if ( _address == SERVER_DISCONNECTED ) {
        cout << "Node server down" << endl;
        serverStarted = false;
        webRenderConnected = false;
        
    } else if( _address == WEB_RENDER_CONNECTED ){
        cout << "web render connected" << endl;
        webRenderConnected = true;
        
    } else if ( _address == WEB_RENDER_DISCONNECTED ){
        cout << "web render disconnected" << endl;
        webRenderConnected = false;
        
    } else if( _address == CUBE_CONNECTED){
        // TODO
        // Add this cube ID into the list of cube connected
        
    } else if( _address == CUBE_DISCONNECTED){
        // TODO
        // Remove this cube ID into the list of cube connected
        
    } else if( _address == CUBE_TOUCHED){
        // TODO
        // get cube id
        // send cubeManager.cubeTouched(id)
        
    } else if( _address == CUBE_DRAGGED){
        // TODO
        // get cube id
        // send cubeManager.cubeDragged(id)
        
    } else if( _address == CUBE_DRAG_END){
        // TODO
        // get cube id
        // send cubeManager.cubeDragEnd(id)
        
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


    //// CONNECTION INFORMATION ////
    ofSetColor(255, 255, 255);
    stringstream reportStream;
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
