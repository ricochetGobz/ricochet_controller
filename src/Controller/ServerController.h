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
// Senders address
#define OF_CONNECTED "/OPConnected"
#define OF_DISCONNECTED "/OPDisconnected"
#define KINECT_CONNECTED "/KConnected"
#define KINECT_DISCONNECTED "/KDisconnected"
#define PLAY_CUBE "/playCube"

class ServerController {

public:
    //// VARIABLES ////
    //// METHODES ////
    ServerController();

    // senders
    void sendOPConnected();
    void sendOPDisconnected();
    void sendOFStatusChange(bool isConnected);
    void sendKinectStatusChange(bool isConnected);
    void sendPlayCube(int cubeId, int soundId, ofPoint pos);

private:
    //// VARIABLES ////

    // sender / receiver
    //ofxOscReceiver receive; // DOESN T WORK HERE
    ofxOscSender sender;

    //// METHODES ////
    void send(string address, string arg);
};


#endif /* ServerController_h */
