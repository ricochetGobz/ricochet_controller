//
//  Cube.cpp
//  ricochet_proto_echo
//
//  Created by Arthur Robert on 10/12/15.
//
//

#include "Cube.h"
#include "echo.h"

//--------------------------------------------------------------
Cube::Cube(ofPoint _pos, int _id, ServerController _server){
    color.set( ofRandom(255), ofRandom(255), ofRandom(255));
    pos = _pos;
    posMid.x = pos.x - size / 2;
    posMid.y = pos.y - size / 2;
    cubeId = _id;
    
    server = _server;
}

//--------------------------------------------------------------
void Cube::draw(){
    ofNoFill();
    ofSetColor(color);
    posMid.x = pos.x - size / 2;
    posMid.y = pos.y - size / 2;
    ofDrawRectangle(posMid,size,size);
    if(contactZoneShowed){
        ofDrawCircle(pos.x, pos.y, contactArea);
    }
}

//--------------------------------------------------------------
void Cube::loadSound(string soundPath){
        cubeSound.load(soundPath);
}

//--------------------------------------------------------------
void Cube::moveTo(ofPoint _pos){
    pos = ofPoint( _pos.x, _pos.y);
}

//--------------------------------------------------------------
void Cube::play(){
    cubeSound.play();
    // TODO envoyer une requête au serveur
    server.sendPlayCube(cubeId, -1, posMid.x, posMid.y);
}

//--------------------------------------------------------------
bool Cube::pointIsInside(ofPoint pointPos){
    
    float _dist = ofDist(pos.x, pos.y, pointPos.x, pointPos.y);
    if(_dist < size/2){
        return true;
    }
    return false;
}