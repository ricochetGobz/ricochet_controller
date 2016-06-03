#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ServerController.h"
#include "KinectController.h"
#include "CubeManager.h"
#include "ofxControlPanel.h"

//// CONST ////

// mode
#define NORMAL_MODE 0
#define CALIBRATION_MODE 1
#define CLOUD_MODE 2

// Screen size
#define RATIO 1.33333
#define WIDTH 800
#define HEIGHT WIDTH/RATIO
#define OC_WIDTH 200
#define OC_HEIGHT OC_WIDTH/RATIO

// Communication
#define RECEIVER_PORT 5555
// Reveicers address
#define SERVER_CONNECTED "/server/connected"
#define SERVER_DISCONNECTED "/server/disconnected"
#define WEB_RENDER_CONNECTED "/webrender/connected"
#define WEB_RENDER_DISCONNECTED "/webrender/disconnected"
#define CUBE_CONNECTED "/cube/connected"
#define CUBE_DISCONNECTED "/cube/disconnected"
#define CUBE_TOUCHED "/cube/touched"
#define CUBE_DRAGGED "/cube/dragged"
#define CUBE_DRAG_END "/cube/draggend"
#define CUBE_FACE_CHANGED "/cube/facechanged"


class ofApp : public ofBaseApp{

public:
    //// VARIABLES ////
    //// METHODES ////
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void mouseReleased(int x, int y, int button);

private:

    //// VARIABLES ////
    // Mode
    int mode;
    ofRectangle renderZone;

    // Controllers
    ServerController server ;
    ofxOscReceiver server_receive;  // !!!! THE RECEIVER DOESN'T WORK
                                    // !!!! INTO NODEBRIGDE
    KinectController kinectCtrl;

    // Components
    CubeManager cubeManager = *new CubeManager(&server, &ServerController::sendPlayCube);
    
    // Audio
    vector<ofSoundPlayer> sounds;
    
    // Status
    bool serverStarted = false;
    bool webRenderConnected = false;
    bool kinectConnected = false;
    
    // GUI
    ofxControlPanel gui;
    // Infos
    ofParameterGroup stats;
    ofParameter <float> appFrameRate;
    ofParameter <int> nBlobs;
    ofParameter <int> nCubes;
    ofParameter <bool> bDebugVideo;
    // threshold controls
    ofParameterGroup thresholdControls;
    ofParameter <int> nearThreshold;
    ofParameter <int> farThreshold;
    // contour finder render controls
    ofParameterGroup renderControls;
    ofParameter <bool> bBlur;
    ofParameter <int> threshold;
    ofParameter <int> minArea;
    ofParameter <int> maxArea;
    // cube detection controls
    ofParameterGroup cubeDetection;
    ofParameter <int> sizeCaptured;
    ofParameter <int> dilationTolerance;
    ofParameter <int> sizeTolerance;


    //// METHODES ////
    void checkReceivedAddress(string _address, ofxOscMessage _m);
    void checkNbrOfCubeFound();
};
