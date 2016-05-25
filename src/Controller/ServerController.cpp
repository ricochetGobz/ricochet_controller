//
//  ServerController.cpp
//  ricochet_test_communication
//
//  Created by Boulay Jérémie on 19/04/2016.
//
//

#include "ServerController.h"

// CONSTRUCTOR -----------------------------------------------------------------
ServerController::ServerController(){
    //receive.setup(RECEIVER_PORT);
    sender.setup( HOST, SENDER_PORT );
    sendOFStatusChange(true);
    // WARNING constructor called multiple 
}

// SEND ------------------------------------------------------------------------
// - SEND OF STATUS CHANGE------------------------------------------------------
// - SEND KINECT STATUS CHANGE--------------------------------------------------
// - SEND NBR OF CUBE FOUNDED CHANGE -------------------------------------------
// - SEND PLAY CUBE ------------------------------------------------------------
void ServerController::send(string address, string arg) {
    ofxOscMessage m;
    m.setAddress( address );
    if ( arg != "" ){
        m.addStringArg( arg );
    }
    sender.sendMessage( m );
}
void ServerController::sendOFStatusChange(bool isConnected) {
    if(isConnected){
        send(OF_CONNECTED, "");
    } else {
        send(OF_DISCONNECTED, "");
    }
}
void ServerController::sendKinectStatusChange(bool isConnected) {
    if(isConnected){
        send(KINECT_CONNECTED, "");
    } else {
        send(KINECT_DISCONNECTED, "");
    }
}
void ServerController::sendNbrOfCubeFoundedChange(int nCubesFounded){
    send(NBR_CUBE_FOUNDED, ofToString(nCubesFounded));
}
void ServerController::sendPlayCube(int cubeId, int soundId, ofPoint pos) {
    send(PLAY_CUBE, "{\"cubeId\":"+ofToString(cubeId)+", \"soundId\":"+ofToString(soundId)+", \"x\":"+ofToString(pos.x)+", \"y\":"+ofToString(pos.y)+"}");
}
