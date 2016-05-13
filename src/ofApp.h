#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ServerController.h"
#include "KinectController.h"
#include "CubeManager.h"
#include "ofxControlPanel.h"

//// CONST ////
// Communication
#define RECEIVER_PORT 5555

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
    ServerController server;
    ofxOscReceiver server_receive;  // !!!! THE RECEIVER DOESN'T WORK
                                    // !!!! INTO NODEBRIGDE
    KinectController kinectCtrl;

    // Components
    CubeManager cubeManager = *new CubeManager(server);
    
    // Audio
    vector<ofSoundPlayer> sounds;
    
    // GUI
    ofxControlPanel gui;
    // Infos
    ofParameterGroup stats;
    ofParameter <float> appFrameRate;
    ofParameter <int> nBlobs;
    ofParameter <int> nCubes;
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
};
