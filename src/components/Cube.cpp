//
//  Cube.cpp
//  ricochet_proto_echo
//
//  Created by Arthur Robert on 10/12/15.
//
//

#include <stdio.h>
#include "Cube.h"
#include "echo.h"

//--------------------------------------------------------------
Cube::Cube(ofPoint _pos, int _id){
    color.set( ofRandom(255), ofRandom(255), ofRandom(255));
    pos = _pos;
    cubeId = _id;
    
    colors.push_back(*new ofColor(143, 202, 217));
    colors.push_back(*new ofColor(193, 243, 221));
    colors.push_back(*new ofColor(236, 210, 234));
    colors.push_back(*new ofColor(129, 233, 158));
    colors.push_back(*new ofColor(229, 131, 217));
    colors.push_back(*new ofColor(89, 166, 212));
}

//--------------------------------------------------------------
void Cube::draw(){
    ofFill();
    ofSetColor(colors[cubeId % 6]);
    ofPoint posMid;
    posMid.x = pos.x - size /2;
    posMid.y = pos.y - size/2;
    ofDrawRectangle(posMid,size,size);

    ofNoFill();
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
    cout << " Cube Moved" << endl;
}

//--------------------------------------------------------------
void Cube::play(){
    cubeSound.play();
}

//--------------------------------------------------------------
bool Cube::pointIsInside(ofPoint pointPos){
    
    float _dist = ofDist(pos.x, pos.y, pointPos.x, pointPos.y);
    if(_dist < size/2){
        return true;
    }
    return false;
}