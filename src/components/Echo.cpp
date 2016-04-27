//
//  Echo.cpp
//  ricochet_proto_echo
//
//  Created by Arthur Robert on 10/12/15.
//
//

#include "Echo.h"

//--------------------------------------------------------------
Echo::Echo(ofPoint _pos){
    color.set(255.0, 255.0, 255.0);
    pos = _pos;
}

//--------------------------------------------------------------
void Echo::draw(){
    ofNoFill();
    ofSetColor(color);
    ofDrawCircle(pos.x, pos.y, size);
}

//--------------------------------------------------------------
void Echo::expand(){
    
    expandForce = (sizeMax - size) * vel;
    size += expandForce;
}

bool Echo::souldRemoved() {
    
    return (expandForce < 0.9);
    //return (size > sizeMax);
}

//--------------------------------------------------------------
bool Echo::checkCubeCollision(ofPoint _cubePos){
    float _dist = ofDist( pos.x, pos.y, _cubePos.x, _cubePos.y);
    
    return (size >= _dist);
}