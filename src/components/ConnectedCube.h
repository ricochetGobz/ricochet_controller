//
//  ConnectedCube.h
//  ricochet_controller
//
//  Created by Boulay Jérémie on 28/05/2016.
//
//

#ifndef ConnectedCube_h
#define ConnectedCube_h

#include "ofMain.h"

#define INACTIVE 0
#define DRAGGED 1
#define TOUCHED 2
#define DRAG_END 3


class ConnectedCube {
public:
    //// VARIABLES ////
    int connectedCubeId;
    int cubeId = -1;

    int status = INACTIVE;

    //// METHODES ////
    ConnectedCube(int _connectedCubeId, int _faceId);
    ConnectedCube();
    
    void update();

    bool isLinkedToCube();

    void setStatus(int _status);
    string getStatus();

private:
    //// VARIABLES ////
    // id of sound
    int faceId;
    int statusLifetime = -1;
};


#endif /* ConnectedCube_h */
