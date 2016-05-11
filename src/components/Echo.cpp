//
//  Echo.cpp
//  ricochet_proto_echo
//
//  Created by Arthur Robert on 10/12/15.
//
//

#include "Echo.h"

// CONSTRUCTOR --------------------------------------------------------------
Echo::Echo(ofRectangle _cubeZone){
    center = _cubeZone.getCenter();
    sizeMax = _cubeZone.width * CONTACT_RATIO;
}

// DRAW --------------------------------------------------------------
void Echo::draw(){
    ofNoFill();
    ofSetColor(255.0, 255.0, 255.0);
    ofDrawCircle(center, size);
}

// EXPAND (update) --------------------------------------------------------------
void Echo::expand(){
    expandForce = (sizeMax - size) * VEL;
    size += expandForce;
}

// SOULD REMOVED --------------------------------------------------------------
bool Echo::souldRemove() {
    return (expandForce < FORCE_MIN);
}

// CHECK CUBE COLLISION --------------------------------------------------------------
bool Echo::checkCubeCollision(ofPoint _cubePos){
    return (size >= ofDist(center.x, center.y, _cubePos.x, _cubePos.y));
}
