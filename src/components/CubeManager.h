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
#include "ConnectedCube.h"
#include "ofxOpenCv.h"
#include "EchoContainer.h"
#include "ServerController.h"

#define INACTIVE 0
#define DRAGGED 1
#define TOUCHED 2


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
    void mouseReleased(int _x, int _y);
    // - cube events
    void cubeConnected(int _connectedCubeId, int _faceId);
    void cubeDisconnected(int _connectedCubeId);
    void cubeTouched(int _connectedCubeId, int _connectedSoundId);
    void cubeDragged(int _connectedCubeId);
    void cubeDragEnd(int _connectedCubeId, int _connectedSoundId);
    void cubeFaceChanged(int _connectedCubeId, int _faceId);

    // test
    bool chronoIsActivate();

    // getters
    int getNbrCubesFound();
    Cube getDetectedCube(int _idCube);
    ConnectedCube getConnectedCube(int _idConnectedCube); // TODO doesn't work
    stringstream getConnectedCubesStatus();

    void removeAllConnectedCubes();

private:
    //// VARIABLES ////
    int idIncremented;

    ServerController* serv_;
    function_type sendPlayCube_;

    vector<Cube> detectedCubes; // TODO remplace vector to map
    map<int, ConnectedCube> connectedCubes;

    map<int, int> connectedCubesDragged;
    queue<int> lastConnectedCubesDragged;

    vector<EchoContainer> echoContainers;

    Cube * cubeChrono;

    //// METHODES ////
    void createEchoContainer(Cube* _cube);
    void updateDetectedCube(ofRectangle _cubeDetected);
    void playCube(EchoContainer* _echoContainer, Cube* _cube);
};

#endif /* CubeManager_h */
