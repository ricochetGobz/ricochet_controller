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
#define OF_CONNECTED "/OF_Connected"
#define OF_DISCONNECTED "/OF_Disconnected"
#define KINECT_CONNECTED "/K_Connected"
#define KINECT_DISCONNECTED "/K_Disconnected"
#define PLAY_CUBE "/C_Played"

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
