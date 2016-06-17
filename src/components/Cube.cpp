//
//  Cube.cpp
//  ricochet_proto_echo
//
//  Created by Arthur Robert on 10/12/15.
//
//

#include "Cube.h"
#include "echo.h"

// CONSTRUCTOR --------------------------------------------------------------
Cube::Cube(ofPoint _pos, int _id){
    pos = _pos;
    cubeId = _id;
}

void Cube::update() {
    if(!isLinkedToConnectedCube()) reduceLifeCicle();
}

// DRAW --------------------------------------------------------------
// POINT IS INSIDE DRAWED SHAPE -------------------------------------------------------------
void Cube::draw(ofRectangle _renderZone){

    //// UPDATE POS IN TERMS OF RENDERZONE ////
    float _cubeSize = _renderZone.width * CUBE_RATIO;
    float _contourFinderSize = _renderZone.width * CONTOUR_FINDER_RATIO;
    // pos = screenPos + ((screenSize * cubePos) / kinectSize) - (cubeSize / 2) - (contourFinderSize / 2);
    // equ = _x + ((_w * pos.x) / _kinectWidth) - ((_w * SIZE_RATIO) / 2) - ((_w * CONTOUR_FINDER_RATIO) / 2);
    drawedShape.set(_renderZone.x + ((_renderZone.width * pos.x) / 640) - ((_cubeSize - _contourFinderSize) / 2),
                    _renderZone.y + ((_renderZone.height * pos.y) / 480) - ((_cubeSize - _contourFinderSize) / 2),
                    _cubeSize,
                    _cubeSize);


    //// DRAW CUBE ////
    ofNoFill();

    if(isLinkedToConnectedCube()) {
        ofSetColor(56, 247, 83, 255);
    } else if(isDetected()) {
        if(isSeachingCubeMode()) {
            ofSetColor(255, 251, 111, 255);
        } else {
            ofSetColor(255, 68, 41, 255);
        }
    } else {
        ofSetColor(255, 255, 255, 255);
    }

    stringstream reportStream;
    reportStream << "cubeId: " << cubeId << endl
    << "connectedCubeId: " << connectedCubeId << endl
    << "faceId: " << faceId << endl
    << "x: " << pos.x << ", y: " << pos.y << endl;
    ofDrawBitmapString(reportStream.str(), drawedShape.x + 2, drawedShape.y - 40);

    ofDrawRectangle(drawedShape);

//  if(contactZoneShowed){
//    ofDrawCircle(drawedShape.getCenter(), drawedShape.width * CONTACT_RATIO);
//  }
}
bool Cube::pointIsInsideDrawedShape(ofPoint _pointPos){
    return ( _pointPos.x > drawedShape.x
            && _pointPos.y > drawedShape.y
            && _pointPos.x < drawedShape.x + drawedShape.width
            && _pointPos.y < drawedShape.y + drawedShape.height );
}

// LOAD SOUND ------------------------------------------------------------------
void Cube::loadSound(string soundPath){
    cubeSound.load(soundPath);
}

// PLAY ------------------------------------------------------------------------
void Cube::play(){
    // TEST si le son est bien loaded
    cubeSound.play();
}

// KILL ------------------------------------------------------------------------
void Cube::kill(){
    lifetime = 0;
}

// INCREASE LIFE CICLE ---------------------------------------------------------
// RECUDE LIFE CICLE -----------------------------------------------------------
// - IS DEAD -------------------------------------------------------------------
// - IS DETECTED ---------------------------------------------------------------
// - IS LINKED TO CONNECTED CUBE -----------------------------------------------
void Cube::increaseLifeCicle() {
  if(lifetime < LIFETIME_MAX) lifetime += (LIFETIME_INCREMENT * 1.5);
}
void Cube::reduceLifeCicle() {
  lifetime -= LIFETIME_INCREMENT;
}
bool Cube::isDead() {
  return (lifetime < 0);
}
bool Cube::isDetected() {
  return (lifetime > LIFETIME_DETECTED);
}
bool Cube::isSeachingCubeMode() {
    return (lifetime > LIFETIME_DETECTED && lifetime < LIFETIME_SEACHING_CONNECTED_CUBE);
}
bool Cube::isLinkedToConnectedCube() {
    return (connectedCubeId != -1);
}

// SET FACE -----
void Cube::setFace(int _faceId) {
    faceId = _faceId;
    if(_faceId > 0 && _faceId <= 6) loadSound("./sounds/note_" + to_string(_faceId) +".wav");
}
