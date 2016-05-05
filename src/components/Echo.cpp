//
//  Echo.cpp
//  ricochet_proto_echo
//
//  Created by Arthur Robert on 10/12/15.
//
//

#include "Echo.h"

// CONSTRUCTOR --------------------------------------------------------------
Echo::Echo(ofPoint _pos){
    pos = _pos;
}

// DRAW --------------------------------------------------------------
void Echo::draw(){
    ofNoFill();
    ofSetColor(255.0, 255.0, 255.0);
    ofDrawCircle(pos.x, pos.y, size);
}

// EXPAND (update) --------------------------------------------------------------
void Echo::expand(){
    expandForce = (SIZE_MAX - size) * VEL;
    size += expandForce;
}

// SOULD REMOVED --------------------------------------------------------------
bool Echo::souldRemove() {
    return (expandForce < FORCE_MIN);
}

// CHECK CUBE COLLISION --------------------------------------------------------------
bool Echo::checkCubeCollision(ofPoint _cubePos){
    return (size >= ofDist( pos.x, pos.y, _cubePos.x, _cubePos.y));
}
