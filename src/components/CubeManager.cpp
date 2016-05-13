//
//  CubeManager.cpp
//  ricochet_controller
//
//  Created by Boulay Jérémie on 06/05/2016.
//
//

#include "CubeManager.h"
#include "ofxOpenCv.h"


CubeManager::CubeManager(ServerController _server){
    idIncremented = 0;
    server = server;
}

// - SHOULD REMOVE (echo container) --------------------------------------------------------------
// - SHOULDREMOVE CUBE (for ofRemove) --------------------------------------------------------------
// - CHECK DETECTED CUBE --------------------------------------------------------------
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
        // If cube is already into the area
        if(ofDist(_cubeDetected.x, _cubeDetected.y, (*it).pos.x, (*it).pos.y) < 10) {
            (*it).increaseLifeCicle();
            return;
        }
    }
    
    //// NO CUBE IN THIS PLACE ////
    cubes.push_back(*new Cube(ofPoint(_cubeDetected.x, _cubeDetected.y), idIncremented, server));
    idIncremented++;
}

void CubeManager::update(ofxCvContourFinder &_contourFinder, int _cubeDilationTolerance, int _cubeSizeTolerance, int _cubeSizeCaptured) {
    
    //// CUBES FOUNDS UPDATE /////
    vector<ofRectangle> detectedCubes;
    // for each forms found
    for(int i = 0; i < _contourFinder.nBlobs; i++) {
        ofRectangle r = _contourFinder.blobs.at(i).boundingRect;
        
        // if is aproximatively a square && if is approximatively at the good size.
        if((abs(r.width - r.height) < _cubeDilationTolerance) && (abs((r.width - _cubeSizeCaptured)) < _cubeSizeTolerance)) {
         detectedCubes.push_back(r);
        }
    }

    //// Check if a new cube position is detected ////
    for(vector<ofRectangle>::iterator it = detectedCubes.begin(); it != detectedCubes.end(); ++it){
        checkDetectedCube((*it));
    }

    //// CUBES FOUNDED UPDATE ////
    for(vector<Cube>::iterator it = cubes.begin(); it != cubes.end(); ++it){
        
        (*it).update();
        
        // Check if all detected cubes is binded at hard cube
        if((*it).isActive() && !(*it).isKnow()){
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
    
    //// ECHOES UPDATE ////
    // check if echoContainers are alive
    ofRemove(echoContainers, shouldRemove);
    for (vector<EchoContainer>::iterator itWave = echoContainers.begin(); itWave != echoContainers.end(); ++itWave) {
        (*itWave).update();
        for(vector<Cube>::iterator itCube = cubes.begin(); itCube != cubes.end(); ++itCube){
            if((*itCube).isActive()) (*itWave).checkEchoCollision((*itCube));
        }
    }
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
void CubeManager::mouseReleased(int _x, int _y) {
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
