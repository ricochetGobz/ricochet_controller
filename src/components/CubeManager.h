//
//  CubeManager.h
//  ricochet_controller
//
//  Created by Boulay Jérémie on 06/05/2016.
//
//

#ifndef CubeManager_h
#define CubeManager_h

#include "Cube.h"
#include "EchoContainer.h"
#include "ServerController.h"



class CubeManager {

public:
    //// VARIABLES ////
    vector<Cube> cubes;

    //// METHODES ////
    CubeManager(ServerController _server);
    void update(vector<ofRectangle> _detectedCubes);
    void draw(ofRectangle _renderZone);

    void checkDetectedCube(ofRectangle _cubeDetected);
    
    void onClick(int _x, int _y);

private:
    //// VARIABLES ////
    int idIncremented;
    ServerController server;
    
    vector<EchoContainer> echoContainers;

    //// METHODES ////
    void createEchoContainer(Cube _cube);
};

#endif /* CubeManager_h */
