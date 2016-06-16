//
//  ConnectedCube.cpp
//  ricochet_controller
//
//  Created by Boulay Jérémie on 28/05/2016.
//
//

#include "ConnectedCube.h"
#include "ofApp.h"


// CONSTRUCTOR --------------------------------------------------------------
ConnectedCube::ConnectedCube() {
    cout << "ConnectedCube()" <<endl;
}
ConnectedCube::ConnectedCube(int _connectedCubeId, int _faceId){
    cout << "ConnectedCube(int _connectedCubeId, int _faceId)" <<endl;
    connectedCubeId = _connectedCubeId;
    faceId = _faceId;
}


void ConnectedCube::update() {
    // reduce lifetime of status
    if(statusLifetime == 0) {
        status = INACTIVE;
    } else if (statusLifetime > 0) {
        statusLifetime--;
    }
}

bool ConnectedCube::isLinkedToCube() {
    return (cubeId != -1);
}

void ConnectedCube::setStatus(int _status) {
    status = _status;
    
    cout << "status" << _status << endl;
    
    switch (status) {
        case DRAGGED:
            statusLifetime = -1;
            break;
        case TOUCHED:
        case DRAG_END:
            statusLifetime = 25;
            break;
        default:
            break;
    }
}

string ConnectedCube::getStatus() {
    switch (status) {
        case INACTIVE:
            return "";
        case DRAGGED:
            return "dragged";
        case TOUCHED:
            return "touched";
        case DRAG_END:
            return "drag end";
        default:
            return "unknown";
    }
}
