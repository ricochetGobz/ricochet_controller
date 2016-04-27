#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ServerController.h"
#include "KinectController.h"
#include "Cube.h"
#include "EchoContainer.h"


#define RECEIVER_PORT 5555

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    
private:
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