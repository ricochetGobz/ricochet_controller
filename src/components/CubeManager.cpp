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

bool shouldRemoveConnectedCube(ConnectedCube &e) {
    return true;
}

void CubeManager::updateDetectedCube(ofRectangle _cubeDetected) {
    // Test if cube already exist
    for(vector<Cube>::iterator it = detectedCubes.begin(); it != detectedCubes.end(); ++it){
        // If cube is already into the area
        if(ofDist(_cubeDetected.x, _cubeDetected.y, (*it).pos.x, (*it).pos.y) < 10) {
            (*it).increaseLifeCicle();
            return;
        }
    }

    //// NO CUBE DETECTED, CREATE A NEW CUBE ////
    idIncremented++;

    // If cube is positionned on chrono, prepare timer
    if(ofDist(_cubeDetected.x, _cubeDetected.y, 330, 80) < 15) {
        if(!cubeChrono) cubeChrono = new Cube(_cubeDetected.position, idIncremented);
        cubeChrono->increaseLifeCicle();
        return;
    }

    // Else, create a new generic cube
    detectedCubes.push_back(*new Cube(_cubeDetected.position, idIncremented));
    // TEMPS
    detectedCubes[detectedCubes.size() - 1].setFace((idIncremented % 5) + 1);
}

// UPDATE ------------------------------------------------------------------------------------------------
void CubeManager::update(ofxCvContourFinder &_contourFinder, int _cubeDilationTolerance, int _cubeSizeTolerance, int _cubeSizeCaptured) {
    
    //// CUBES DETECTION UPDATE /////
    // If no cubes moved
    if(connectedCubesDragged.size() == 0) {
        
        vector<ofRectangle> _detectedShapes;
        // for each forms found
        for(int i = 0; i < _contourFinder.nBlobs; i++) {
            ofRectangle r = _contourFinder.blobs.at(i).boundingRect;
            
            // if is aproximatively a square && if is approximatively at the good size.
            if((abs(r.width - r.height) < _cubeDilationTolerance) && (abs((r.width - _cubeSizeCaptured)) < _cubeSizeTolerance)) {
                _detectedShapes.push_back(r);
            }
        }
        
        //// CUBES CHRONO UPDATE /////
        if(cubeChrono) {
            cubeChrono->update();
            if(cubeChrono->isDead()) cubeChrono = NULL;
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
    }
    
    //// CUBES FOUND UPDATE ////
    for(vector<Cube>::iterator it = detectedCubes.begin(); it != detectedCubes.end(); ++it){
        if(connectedCubesDragged.size() == 0) {
            (*it).update();
        } else if(!(*it).isDetected()) {
            (*it).update();
        }
        
        // Check if all detected cubes is binded at hard cube
        if ((*it).isSeachingCubeMode()) {
            
            // (*it).connectedCubeId = lastConnectedCubesDragged.pop();
            
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
    

    //// CUBES CONNECTED UPDATE ////
    for(map<int, ConnectedCube>::iterator itConnectedCube = connectedCubes.begin(); itConnectedCube != connectedCubes.end(); itConnectedCube++) {
        // update to reduce status lifetime
        (&itConnectedCube->second)->update();
    }

    //// ECHOES UPDATE ////
    // check if echoContainers are alive
    ofRemove(echoContainers, shouldRemove);
    for (vector<EchoContainer>::iterator itWave = echoContainers.begin(); itWave != echoContainers.end(); ++itWave) {
        (*itWave).update();
        for(vector<Cube>::iterator itCube = detectedCubes.begin(); itCube != detectedCubes.end(); ++itCube){
            // TODO remplacer is detected par isLinkedToConnectedCube();
            if((*itCube).isDetected() && (*itWave).checkEchoesCollision((*itCube).cubeId, (*itCube).pos)) {
                playCube(&(*itWave), &(*itCube));
            }
        }
    }
}

// CHRONO IS ACTIVATE ----------------------------------------------------------
bool CubeManager::chronoIsActivate() {
    if(cubeChrono) {
        cout << "chronoIsActivate" << cubeChrono->isDetected() << cubeChrono->locked << endl;
        if(cubeChrono->isDetected() && !cubeChrono->locked) {
            cubeChrono->locked = true;
            return true;
        }
    }
    return false;
}

// DRAW --------------------------------------------------------------
void CubeManager::draw(ofRectangle _renderZone) {
    // cubes
    for(vector<Cube>::iterator it = detectedCubes.begin(); it != detectedCubes.end(); ++it){
        (*it).draw(_renderZone);
    }
    
    // chrono
    if(cubeChrono) cubeChrono->draw(_renderZone);

    // echoes
    for (vector<EchoContainer>::iterator it = echoContainers.begin(); it != echoContainers.end(); ++it) {
        (*it).draw(_renderZone);
    }
}

// ON CLICK -----------------------------------------------
void CubeManager::mouseReleased(int _x, int _y) {
    for(vector<Cube>::iterator it = detectedCubes.begin(); it != detectedCubes.end(); ++it) {
        if((*it).isDetected() && (*it).pointIsInsideDrawedShape(ofPoint(_x, _y))) {
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
        cout << "The cube " << _connectedCubeId << " is reconnected." << endl;
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

    // TODO test si connecedCubes[_connectedCubeID] existe.
    ConnectedCube *_cubeTouched = &connectedCubes[_connectedCubeId];

    _cubeTouched->setStatus(TOUCHED); // TEMPS

    // Si le cubeConnecté est bindé avec un cube détecté
    if(_cubeTouched->isLinkedToCube()) {

        // Récupérer le cube
        try {
            Cube _cube = getDetectedCube(_cubeTouched->cubeId);
            _cube.setFace(_connectedSoundId);

            // Lancer un écho
            createEchoContainer(&_cube);
        } catch(const string * e) {
            cout << e << " id : " << _cubeTouched->cubeId << endl;
        }
    } else {
        cout << "The cube " << _connectedCubeId << " is not detected on table." << endl;
    }
}

void CubeManager::cubeDragged(int _connectedCubeId) {
    // MESSAGE CUBE DRAGGED
    connectedCubes[_connectedCubeId].setStatus(DRAGGED);

    // CONTRUCTION
    // sauver le cube qui est draggué (plusieurs s'il le faut).
    connectedCubesDragged[_connectedCubeId] = _connectedCubeId;
    for(vector<Cube>::iterator it = detectedCubes.begin(); it != detectedCubes.end(); ++it){
        // If cube is linked to this connectedCube
        // TEST TEST
        if((*it).connectedCubeId == _connectedCubeId) {
            // TODO delete le pointer
            (*it).connectedCubeId = -1;
            return;
        }
    }
}

void CubeManager::cubeDragEnd(int _connectedCubeId, int _connectedSoundId) {
    connectedCubes[_connectedCubeId].setStatus(DRAG_END);

    // CONTRUCTION
    // supprimer le cube dragged du map.
    connectedCubesDragged.erase(_connectedCubeId);
    cout << connectedCubesDragged.size() << " connectedCube Dragged" << endl;
    // mettre le cube écouté dans une phase d'attende de positionnement.
    // lastConnectedCubesDragged.push(_connectedCubeId);
}

void CubeManager::cubeFaceChanged(int _connectedCubeId, int _connectedSoundId) {

  // SAME TO CUBE TOUCHED

//    try {
//        Cube _cube = getDetectedCube(_cubeTouched->cubeId);
//        _cube.setFace(_connectedSoundId);
//    } catch(const string * e) {
//        cout << e << " id : " << _cubeTouched->cubeId << endl;
//    }
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
    (serv_->*sendPlayCube_)(_cube->cubeId, _cube->faceId, _cube->pos);
    _cube->play();
}

// REMOVE ALL CONNECTED CUBES ------
void CubeManager::removeAllConnectedCubes() {

    // CONSTRUCTION
    connectedCubes.clear();
    // TODO désalouer la mémoire dans le map avant de clear.

    for(vector<Cube>::iterator it = detectedCubes.begin(); it != detectedCubes.end(); ++it) {
        (*it).connectedCubeId = -1;
    }
}

// GET NBR OF CUBES FOUND ------------------------------------------------
int CubeManager::getNbrCubesFound() {
    int cubeFound = 0;
    for(vector<Cube>::iterator it = detectedCubes.begin(); it != detectedCubes.end(); ++it) {
        if((*it).isDetected()){
            cubeFound++;
        }
    }
    return cubeFound;
}

// GET DETECTED CUBE ------------------------------------------------------
Cube CubeManager::getDetectedCube(int _idCube) {
    for(vector<Cube>::iterator it = detectedCubes.begin(); it != detectedCubes.end(); ++it){
        // If cube is already into the area
        if((*it).cubeId == _idCube) return (*it);
    }

    throw "CubeManager.getDetectedCube() ERROR : cubeDetected was not found";
}

// GET CONNECTED CUBES STATUS ------------------------------------------
stringstream CubeManager::getConnectedCubesStatus() {
    stringstream reportStream;
    reportStream << "Cubes Connected: " << endl;

    for (std::map<int, ConnectedCube>::iterator it=connectedCubes.begin(); it!=connectedCubes.end(); ++it) {
        reportStream << "Cube " << it->first << " => " << it->second.getStatus() << endl;
    }

    return reportStream;
}
