#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ServerController.h"
#include "KinectController.h"
#include "CubeManager.h"

//// CONST ////
// Communication
#define RECEIVER_PORT 5555

// mode
#define NORMAL_MODE 0
#define CALIBRATION_MODE 1
#define CLOUD_MODE 2

// Screen size
#define RES 1.33333
#define WIDTH 800
#define HEIGHT WIDTH/RES
#define OC_WIDTH 200
#define OC_HEIGHT OC_WIDTH/RES


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


    //// METHODES ////
};
