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

void CubeManager::updateDetectedCube(ofRectangle _cubeDetected) {
    for(vector<Cube>::iterator it = detectedCubes.begin(); it != detectedCubes.end(); ++it){
        // If cube is already into the area
        if(ofDist(_cubeDetected.x, _cubeDetected.y, (*it).pos.x, (*it).pos.y) < 10) {
            (*it).increaseLifeCicle();
            return;
        }
    }

    //// NO CUBE IN THIS PLACE ////
    detectedCubes.push_back(*new Cube(_cubeDetected.position, idIncremented));
    detectedCubes[detectedCubes.size()-1].loadSound("./sounds/note_" + std::to_string(((detectedCubes.size()-1)%6)+1) +".wav");
    idIncremented++;
}

void CubeManager::update(ofxCvContourFinder &_contourFinder, int _cubeDilationTolerance, int _cubeSizeTolerance, int _cubeSizeCaptured) {

    //// CUBES FOUNDS UPDATE /////
    vector<ofRectangle> _detectedShapes;
    // for each forms found
    for(int i = 0; i < _contourFinder.nBlobs; i++) {
        ofRectangle r = _contourFinder.blobs.at(i).boundingRect;

        // if is aproximatively a square && if is approximatively at the good size.
        if((abs(r.width - r.height) < _cubeDilationTolerance) && (abs((r.width - _cubeSizeCaptured)) < _cubeSizeTolerance)) {
         _detectedShapes.push_back(r);
        }
    }

    /* Check if a new cube position is detected :
     *
     * We have a list of detected cube and we want to check if cube is already exist on
     * this place.
     *
     */
    for(vector<ofRectangle>::iterator it = _detectedShapes.begin(); it != _detectedShapes.end(); ++it){
        updateDetectedCube((*it));
    }

    //// CUBES FOUND UPDATE ////
    for(vector<Cube>::iterator it = detectedCubes.begin(); it != detectedCubes.end(); ++it){
        (*it).update();

        // Check if all detected cubes is binded at hard cube
        if((*it).isActive() && !(*it).isLinkedToConnectedCube()){
            // TODO faire correspondre les cubes avec les id connues.
            // REGARDER LE TABLEAU DES DERNIERS CUBES DRAGGEES
            // SI IL Y EN A UN
            // CUBE = CUBEID
            // CUBE LOCKED JUSQU'A CE QU'ON LE DRAG
            // SINON
            // CUBE INCONNE
        }
    }

    ofRemove(detectedCubes, shouldRemoveCube);

    //// ECHOES UPDATE ////
    // check if echoContainers are alive
    ofRemove(echoContainers, shouldRemove);
    for (vector<EchoContainer>::iterator itWave = echoContainers.begin(); itWave != echoContainers.end(); ++itWave) {
        (*itWave).update();
        for(vector<Cube>::iterator itCube = detectedCubes.begin(); itCube != detectedCubes.end(); ++itCube){
            if((*itCube).isActive() && (*itWave).checkEchoesCollision((*itCube).cubeId, (*itCube).pos)) {
                playCube(&(*itWave), &(*itCube));
            }
        }
    }
}

// DRAW --------------------------------------------------------------
void CubeManager::draw(ofRectangle _renderZone) {
    for(vector<Cube>::iterator it = detectedCubes.begin(); it != detectedCubes.end(); ++it){
        (*it).draw(_renderZone);
    }

    // echoes
    for (vector<EchoContainer>::iterator it = echoContainers.begin(); it != echoContainers.end(); ++it) {
        (*it).draw(_renderZone);
    }
}

// ON CLICK -----------------------------------------------
void CubeManager::mouseReleased(int _x, int _y) {
    for(vector<Cube>::iterator it = detectedCubes.begin(); it != detectedCubes.end(); ++it) {
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
void CubeManager::cubeConnected(int _connectedCubeId, int _faceId) {
    map<int, ConnectedCube>::iterator it = connectedCubes.find(_connectedCubeId);

    // if cube with this id not found
    if (it == connectedCubes.end()) {
        connectedCubes[_connectedCubeId] = ConnectedCube(_connectedCubeId, _faceId);
    } else {
        cout << "The cube" << _connectedCubeId << "is reconnected." << endl;
    }
}

void CubeManager::cubeDisconnected(int _connectedCubeId) {
    map<int, ConnectedCube>::iterator it = connectedCubes.find(_connectedCubeId);

    // TODO déconnecter le detectedCube avec cette id

    if (it != connectedCubes.end()) {
        connectedCubes.erase(it);
    }
}

void CubeManager::cubeTouched(int _connectedCubeId, int _connectedSoundId) {
    cout << "cube touched" << endl;

    // TODO Parcourir les id pour dire quel cube a été touched.
    connectedCubes[_connectedCubeId].setStatus(TOUCHED); // TEMPS

    // Si le cubeConnecté est bindé avec un cube détecté
        // Lancer le touch
        // A la fin du touch supprimer le touched
}

void CubeManager::cubeDragged(int _connectedCubeId) {
    // TODO
    // Supprimer la position du cube connecté.
    // mettre le cube écouté dans une phase d'attende de positionnement.
    connectedCubes[_connectedCubeId].setStatus(DRAGGED);
}

void CubeManager::cubeDragEnd(int _connectedCubeId) {
    // TODO
    // ????
    connectedCubes[_connectedCubeId].setStatus(INACTIVE);
}

void CubeManager::cubeFaceChanged(int _connectedCubeId, int _faceId) {
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
    return detectedCubes.size();
}

stringstream CubeManager::getConnectedCubesStatus() {
    stringstream reportStream;
    reportStream << "Cubes Connected: " << endl;

    for (std::map<int, ConnectedCube>::iterator it=connectedCubes.begin(); it!=connectedCubes.end(); ++it) {
        reportStream << "Cube " << it->first << " => " << it->second.getStatus() << endl;
    }

    return reportStream;
}
