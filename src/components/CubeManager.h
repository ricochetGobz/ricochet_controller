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
    typedef void (ServerController::*function_type)(int, int, ofPoint);

    //// METHODES ////
    CubeManager(ServerController* _serv, function_type _sendPlayCube);
    
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
    
    ServerController* serv_;
    function_type sendPlayCube_;
    
    vector<Cube> cubes;
    vector<EchoContainer> echoContainers;

    //// METHODES ////
    void createEchoContainer(Cube* _cube);
    void playCube(EchoContainer* _echoContainer, Cube* _cube);
};

#endif /* CubeManager_h */
