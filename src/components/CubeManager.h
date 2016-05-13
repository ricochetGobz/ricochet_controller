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
#include "ofxOpenCv.h"
#include "EchoContainer.h"
#include "ServerController.h"



class CubeManager {

public:
    //// VARIABLES ////

    //// METHODES ////
    CubeManager(ServerController _server);
    void update(ofxCvContourFinder &_contourFinder,
                int _cubeDilationTolerance,
                int _cubeSizeTolerance,
                int _cubeSizeCaptured);
    
    void draw(ofRectangle _renderZone);
    
    // events
    void checkDetectedCube(ofRectangle _cubeDetected);
    void mouseReleased(int _x, int _y);

private:
    //// VARIABLES ////
    int idIncremented;
    ServerController server;
    
    vector<Cube> cubes;
    vector<EchoContainer> echoContainers;

    //// METHODES ////
    void createEchoContainer(Cube _cube);
};

#endif /* CubeManager_h */
