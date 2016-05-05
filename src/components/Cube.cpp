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
    cubeId = _id;
    server = _server;
    moveTo(_pos);

    color.set( ofRandom(255), ofRandom(255), ofRandom(255));
}

// DRAW --------------------------------------------------------------
void Cube::draw(){
    ofNoFill();
    ofSetColor(color);
    ofDrawRectangle(posToDraw, SIZE, SIZE);
    if(contactZoneShowed){
        ofDrawCircle(pos.x, pos.y, CONTACT_AREA);
    }
}

// LOAD SOUND ------------------------------------------------------------------
void Cube::loadSound(string soundPath){
    cubeSound.load(soundPath);
}

// MOVE TO ---------------------------------------------------------------------
void Cube::moveTo(ofPoint _pos){
    pos.set(_pos);
    posToDraw.x = pos.x - SIZE / 2;
    posToDraw.y = pos.y - SIZE / 2;
}

// PLAY ------------------------------------------------------------------------
void Cube::play(){
    cubeSound.play();
    server.sendPlayCube(cubeId, -1, posToDraw.x, posToDraw.y);
}

// POINT IS INSIDE -------------------------------------------------------------
bool Cube::pointIsInside(ofPoint pointPos){
    float _dist = ofDist(pos.x, pos.y, pointPos.x, pointPos.y);
    return (_dist < (SIZE / 2));
}
