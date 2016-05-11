//
//  CubeManager.cpp
//  ricochet_controller
//
//  Created by Boulay Jérémie on 06/05/2016.
//
//

#include "CubeManager.h"


CubeManager::CubeManager(ServerController _server){
    server = server;
}


// UPDATE --------------------------------------------------------------
// - SHOULD REMOVE --------------------------------------------------------------
// - SHOULDREMOVE (for ofRemove) --------------------------------------------------------------
bool shouldRemove(EchoContainer &c) {
    if(c.echoes.size() == 0) {
        return true;
    }
    return false;
}
bool shouldRemoveCube(Cube &e){
    return e.isDead();
}
void CubeManager::checkDetectedCube(ofRectangle _cubeDetected) {
    for(vector<Cube>::iterator it = cubes.begin(); it != cubes.end(); ++it){
        if(ofDist(_cubeDetected.x, _cubeDetected.y, (*it).pos.x, (*it).pos.y) < 10) {
            // UN CUBE EST DEJA A CETTE PLACE
            (*it).increaseLifeCicle();
            return;
        }
    }
    
    //// NO CUBE IN THIS PLACE ////
    cubes.push_back(*new Cube(ofPoint(_cubeDetected.x, _cubeDetected.y), idIncremented, server));
    idIncremented++;
}
void CubeManager::update(vector<ofRectangle> _detectedCubes) {
    
    //// ECHOES UPDATE ////
    // check if echoContainers are alive
    ofRemove(echoContainers, shouldRemove);
    for (vector<EchoContainer>::iterator itWave = echoContainers.begin(); itWave != echoContainers.end(); ++itWave) {
        (*itWave).update();
        for(vector<Cube>::iterator itCube = cubes.begin(); itCube != cubes.end(); ++itCube){
            if((*itCube).isActive()) (*itWave).checkEchoCollision((*itCube));
        }
    }


    //// Check if a new cube position is detected ////
    for(vector<ofRectangle>::iterator it = _detectedCubes.begin(); it != _detectedCubes.end(); ++it){
        checkDetectedCube((*it));
    }

    for(vector<Cube>::iterator it = cubes.begin(); it != cubes.end(); ++it){
        
        (*it).reduceLifeCicle();
        
        if((*it).isActive()){
            // TODO faire correspondre les cubes avec les id connues.
            // REGARDER LE TABLEAU DES DERNIERS CUBES DRAGGEES
            // SI IL Y EN A UN
            // CUBE = CUBEID
            // CUBE LOCKED JUSQU'A CE QU'ON LE DRAG
            // SINON
            // CUBE INCONNE
        }
    }
    
    ofRemove(cubes, shouldRemoveCube);
}

// DRAW --------------------------------------------------------------
void CubeManager::draw(ofRectangle _renderZone) {
    for(vector<Cube>::iterator it = cubes.begin(); it != cubes.end(); ++it){
        (*it).draw(_renderZone);
    }
    
    // echoes
    for (vector<EchoContainer>::iterator it = echoContainers.begin(); it != echoContainers.end(); ++it) {
        (*it).draw(_renderZone);
    }
}

// ON CLICK -----------------------------------------------
void CubeManager::onClick(int _x, int _y) {
    for(vector<Cube>::iterator it = cubes.begin(); it != cubes.end(); ++it) {
        if((*it).isActive() && (*it).pointIsInsideDrawedShape(ofPoint(_x, _y))) {
            createEchoContainer((*it));
        }
    }
}

// CREATE ECHO CONTAINER -----------------------------------------------
void CubeManager::createEchoContainer(Cube _cube){
    EchoContainer newEchoContainer = *new EchoContainer(_cube);
    echoContainers.push_back(newEchoContainer);
}
