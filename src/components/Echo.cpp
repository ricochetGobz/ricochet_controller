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
    
    size = 5.0;
    expandForce = 99.0;
}
// UPDATE --------------------------------------------------------------
void Echo::update(){
    
    //// UPDATE EXPAND ////
    expandForce = (SIZE_MAX - size) * VEL;
    size += expandForce;
}

// DRAW --------------------------------------------------------------
void Echo::draw(ofRectangle _renderZone){
    
    //// UPDATE POS IN TERMS OF RENDERZONE ////
    float _contourFinderSize = _renderZone.width * CONTOUR_FINDER_RATIO;
    // pos = screenPos + ((screenSize * cubePos) / kinectSize) + (contourFinderSize / 2);
    // equ = _x + ((_w * pos.x) / _kinectWidth) +((_ w * CONTOUR_FINDER_RATIO) / 2);
    center.set(_renderZone.x + ((_renderZone.width * pos.x) / 640) + (_contourFinderSize / 2),
               _renderZone.y + ((_renderZone.height * pos.y) / 480) + (_contourFinderSize / 2));

    ofNoFill();
    ofSetColor(255.0, 255.0, 255.0);
    ofDrawCircle(center.x, center.y, (_renderZone.width / 640) * size);
}

// SOULD REMOVED --------------------------------------------------------------
bool Echo::souldRemove() {
    return (expandForce < FORCE_MIN);
}

// CHECK CUBE COLLISION --------------------------------------------------------------
bool Echo::checkCubeCollision(ofPoint _cubePos){
    return (size >= ofDist( pos.x, pos.y, _cubePos.x, _cubePos.y));
}