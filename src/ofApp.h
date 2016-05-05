#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ServerController.h"
#include "KinectController.h"
#include "Cube.h"
#include "EchoContainer.h"

//// CONST ////
// Communication
#define RECEIVER_PORT 5555

// mode
#define NORMAL_MODE 0
#define CALIBRATION_MODE 1
#define CLOUD_MODE 2


class ofApp : public ofBaseApp{

public:
    //// VARIABLES ////
    //// METHODES ////
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);


private:

    //// VARIABLES ////
    // Mode
    int mode;

    // !!TEMP!! //
    int cubeDragged;
    int nCube = 8;
    bool mouseDown = false;
    bool mouseMove = false;
    // !!TEMP!! //

    // Controllers
    ServerController server;
    ofxOscReceiver server_receive;  // !!!! THE RECEIVER DOESN'T WORK
                                    // !!!! INTO NODEBRIGDE
    KinectController kinectCtrl;

    // Components
    vector<Cube> cubes;
    vector<EchoContainer> echoContainers;

    // Audio
    vector<ofSoundPlayer> sounds;


    //// METHODES ////
    void createEchoContainer(Cube _cube);

};
