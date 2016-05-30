//
//  CubeManager.cpp
//  ricochet_controller
//
//  Created by Boulay Jérémie on 06/05/2016.
//
//

#include "CubeManager.h"
#include "ofxOpenCv.h"


CubeManager::CubeManager(ServerController* _serv, function_type _sendPlayCube):serv_(_serv),sendPlayCube_(_sendPlayCube){
    idIncremented = 0;
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
    cubes.push_back(*new Cube(_cubeDetected.position, idIncremented));
    cubes[cubes.size()-1].loadSound("./sounds/note_" + std::to_string(((cubes.size()-1)%6)+1) +".wav");
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

    /* Check if a new cube position is detected :
     *
     * We have a list of detected cube and we want to check if cube is already exist on
     * this place.
     *
     */
    for(vector<ofRectangle>::iterator it = detectedCubes.begin(); it != detectedCubes.end(); ++it){
        checkDetectedCube((*it));
    }

    //// CUBES FOUND UPDATE ////
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
            if((*itCube).isActive() && (*itWave).checkEchoesCollision((*itCube).cubeId, (*itCube).pos)) {
                playCube(&(*itWave), &(*itCube));
            }
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
            createEchoContainer(&(*it));
        }
    }
}

// CUBE CONNECTED --------------------------------------------------
// CUBE DISCONNECTED -----------------------------------------------
// CUBE TOUCHED ----------------------------------------------------
// CCUBE DRAGGED ---------------------------------------------------
// CUBE DRAG END ---------------------------------------------------
void CubeManager::cubeConnected(int cubeId) {
    
}
void CubeManager::cubeDisconnected(int cubeId) {
    
}
void CubeManager::cubeTouched(int cubeId) {
    
}
void CubeManager::cubeDragged(int cubeId) {
    // TODO
    // Supprimer la position du cube connecté.
    // mettre le cube écouté dans une phase d'attende de positionnement.
}
void CubeManager::cubeDragEnd(int cubeId) {
    // TODO
    // ????
}

// CREATE ECHO CONTAINER -----------------------------------------------
void CubeManager::createEchoContainer(Cube* _cube){
    EchoContainer newEchoContainer = *new EchoContainer();
    playCube(&newEchoContainer, _cube);
    echoContainers.push_back(newEchoContainer);
}
// PLAY CUBE ----
void CubeManager::playCube(EchoContainer* _echoContainer, Cube* _cube) {
    _echoContainer->createEcho(_cube->cubeId, _cube->pos);
    (serv_->*sendPlayCube_)(_cube->cubeId, -1, _cube->pos);
    _cube->play();
}


int CubeManager::getNbrCubesFound() {
    return cubes.size();
}
