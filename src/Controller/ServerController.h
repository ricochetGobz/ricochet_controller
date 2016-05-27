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
#define OF_CONNECTED "/openframeworks/connected"
#define OF_DISCONNECTED "/openframeworks/disconnected"
#define KINECT_CONNECTED "/kinect/connected"
#define KINECT_DISCONNECTED "/kinect/disconnected"
#define NBR_CUBE_FOUND "/cubes/nfound"
#define PLAY_CUBE "/cube/played"

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
    void sendNbrOfCubeFoundChange(int nCubesFound);
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
