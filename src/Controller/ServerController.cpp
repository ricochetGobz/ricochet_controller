//
//  ServerController.cpp
//  ricochet_test_communication
//
//  Created by Boulay Jérémie on 19/04/2016.
//
//

#include "ServerController.h"

//--------------------------------------------------------------
ServerController::ServerController(){
    //receive.setup(RECEIVER_PORT);
    sender.setup( HOST, SENDER_PORT );
    sendOFStatusChange(true);
    sendKinectStatusChange(kinectConnected);
}
void ServerController::checkAddress(string address) {
    
    if(address == SERVER_STARTED ) {
        cout << "Node server started" << endl;
        if(!serverStarted){
            serverStarted = true;
            sendOFStatusChange(true);
            sendKinectStatusChange(kinectConnected);
        }
    } else if ( address == SERVER_DOWN ) {
        cout << "Node server down" << endl;
        serverStarted = false;
        webRenderConnected = false;
    } else if( address == WEB_RENDER_CONNECTED ){
        cout << "web render connected" << endl;
        webRenderConnected = true;
    } else if ( address == WEB_RENDER_DISCONNECTED ){
        cout << "web render disconnected" << endl;
        webRenderConnected = false;

    }
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

void ServerController::sendPlayCube(int cubeId, int soundId, int x, int y) {
    send(PLAY_CUBE, "{cubeId:"+ofToString(cubeId)+", soundId:"+ofToString(soundId)+", x:"+ofToString(x)+", y:"+ofToString(y)+"}");
}

void ServerController::send(string address, string arg) {
    ofxOscMessage m;
    m.setAddress( address );
    if ( arg != "" ){
        m.addStringArg( arg );
    }
    sender.sendMessage( m );
}

bool ServerController::isStarted() {
    return serverStarted;
}

bool ServerController::webRenderIsConnected() {
    return webRenderConnected;
}

bool ServerController::kinectIsConnected() {
    return kinectConnected;
}

void ServerController::setKinectStatus(bool status){
    kinectConnected = status;
}
