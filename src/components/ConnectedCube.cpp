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

bool ConnectedCube::isLinkedToCube() {
    return (cubeId != -1);
}

void ConnectedCube::setStatus(int _status) {
    status = _status;
}

string ConnectedCube::getStatus() {
    switch (status) {
        case INACTIVE:
            return "inactive";
        case DRAGGED:
            return "dragged";
        case TOUCHED:
            return "touched";
        default:
            return "unknown";
    }
}
