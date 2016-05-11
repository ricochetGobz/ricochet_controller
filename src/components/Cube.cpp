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
Cube::Cube(ofPoint _pos, int _id, ServerController _server){
    pos = _pos;
    lifeCicle = LIFECICLE_START;
    cubeId = _id;
    server = _server;

    color.set( ofRandom(255), ofRandom(255), ofRandom(255));
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
    if(isActive()) {
        ofSetColor(color);
        
        stringstream reportStream;
        reportStream << "_id: " << cubeId << endl
        << "id: " << linkedId << endl;
        ofDrawBitmapString(reportStream.str(), drawedShape.x + 2, drawedShape.y - 18);
        
    } else {
        ofSetColor(255, 255, 255, 50);
    }
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
    cubeSound.play();
    server.sendPlayCube(cubeId, -1, pos.x, pos.y);
}

// INCREASE LIFE CICLE ---------------------------------------------------------
// - IS DEAD ------------------------------------------------------------------
// - IS ACTIVE -----------------------------------------------------------------
void Cube::increaseLifeCicle() {
  if(lifeCicle < LIFECICLE_MAX) lifeCicle += 15;
}
void Cube::reduceLifeCicle() {
  lifeCicle -= 10;
}
bool Cube::isDead() {
  return (lifeCicle < 0);
}
bool Cube::isActive() {
  return (lifeCicle > LIFECICLE_ACTIVE);
}

// IS KNOW ---------------------------------------------------------
bool Cube::isKnow() {
    return (linkedId != -1);
}
