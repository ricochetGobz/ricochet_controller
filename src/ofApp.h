#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ServerController.h"
#include "KinectController.h"
#include "Cube.h"
#include "EchoContainer.h"


#define RECEIVER_PORT 5555

#define NORMAL_MODE 0
#define CALIBRATION_MODE 1
#define CLOUD_MODE 2

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
private:
    // UX
    int mode;

    // CONTROLLERS
    ServerController server;
    // !!!! THE RECEIVER DOESN'T WORK INTO NODEBRIGDE
    ofxOscReceiver server_receive;
    
    KinectController kinect;
    
    // TEMPS
    int cubeDragged;
    int nCube = 8;
    bool mouseDown = false;
    bool mouseMove = false;
    bool bSetupArduino = false;
    
    // COMPONENTS
    vector<Cube> cubes;
    vector<EchoContainer> echoContainers;
    
    void createEchoContainer(Cube _cube);
    
    // AUDIO PART
    vector<ofSoundPlayer> sounds;

    
};
