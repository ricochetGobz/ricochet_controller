//
//  ServerController.h
//  ricochet_test_communication
//
//  Created by Boulay Jérémie on 19/04/2016.
//
//

#ifndef ServerController_h
#define ServerController_h

#include "ofxOsc.h"

//// CONST ////
// Host
#define HOST "localhost"
// Ports
#define SENDER_PORT 4444
#define RECEIVER_PORT 5555
// Senders address
#define OF_CONNECTED "/OPConnected"
#define OF_DISCONNECTED "/OPDisconnected"
#define KINECT_CONNECTED "/KConnected"
#define KINECT_DISCONNECTED "/KDisconnected"
#define PLAY_CUBE "/playCube"

// Reveicers address
#define SERVER_STARTED "/serverStarted"
#define SERVER_DOWN "/serverDown"
#define WEB_RENDER_CONNECTED "/WRConnected"
#define WEB_RENDER_DISCONNECTED "/WRDisconnected"
#define NEW_CUBE_CONNECTED "/newCubeConnected"

class ServerController {

public:
    //// VARIABLES ////
    //// METHODES ////
    ServerController();

    // receive address
    void checkAddress(string address);
    // senders
    void sendOPConnected();
    void sendOPDisconnected();
    void sendOFStatusChange(bool isConnected);
    void sendKinectStatusChange(bool isConnected);
    void sendPlayCube(int cubeId, int soundId, int x, int y);
    // checking
    bool isStarted();
    bool webRenderIsConnected();
    bool kinectIsConnected();
    // setters
    void setKinectStatus(bool status);


private:
    //// VARIABLES ////
    // status
    bool serverStarted = false;
    bool webRenderConnected = false;
    bool kinectConnected = false;

    // sender / receiver
    //ofxOscReceiver receive; // DOESN T WORK HERE
    ofxOscSender sender;

    //// METHODES ////
    void send(string address, string arg);
};


#endif /* ServerController_h */
