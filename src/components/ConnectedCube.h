//
//  ConnectedCube.h
//  ricochet_controller
//
//  Created by Boulay Jérémie on 28/05/2016.
//
//

#ifndef ConnectedCube_h
#define ConnectedCube_h

#define INACTIVE 0
#define DRAGGED 1
#define TOUCHED 2


class ConnectedCube {
public:
    //// VARIABLES ////
    int connectedCubeId;
    int cubeId = -1;
    
    int status = INACTIVE;
    
    //// METHODES ////
    ConnectedCube(int _connectedCubeId, int _faceId);
    ConnectedCube();

    
    bool isLinkedToCube();
    
private:
    //// VARIABLES ////
    
    // id of sound
    int faceId;
};


#endif /* ConnectedCube_h */
