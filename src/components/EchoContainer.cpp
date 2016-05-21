//
//  EchoContainer.cpp
//  ricochet_proto_echo
//
//  Created by Boulay Jérémie on 09/02/2016.
//
//

#include "EchoContainer.h"
#include "Cube.h"


EchoContainer::EchoContainer(){}


// UPDATE --------------------------------------------------------------
// - SHOULD REMOVE --------------------------------------------------------------
bool shouldRemove(Echo &e){
    return e.souldRemove();
}
void EchoContainer::update() {
    ofRemove(echoes, shouldRemove);

    for(vector<Echo>::iterator it = echoes.begin(); it != echoes.end(); ++it){
        (*it).update();
    }
}

// DRAW --------------------------------------------------------------
void EchoContainer::draw(ofRectangle _renderZone) {
    for(vector<Echo>::iterator it = echoes.begin(); it != echoes.end(); ++it){
        (*it).draw(_renderZone);
    }
}

// CREATE ECHO --------------------------------------------------------------
void EchoContainer::createEcho(int _cubeId, ofPoint _cubePos) {
    // Save the native cube like touched
    cubesTouched.push_back(_cubeId);
    echoes.push_back(*new Echo(_cubePos));
}

// CHECK ECHO COLLISION --------------------------------------------------------------
bool EchoContainer::checkEchoesCollision(int _cubeId, ofPoint _cubePos) {
    
    if(isAlreadyTouched(_cubeId)) return false;

    for(vector<Echo>::iterator it = echoes.begin(); it != echoes.end(); ++it) {
        // If echoes can activate another cubes
        if((*it).checkCubeCollision(_cubePos)) {
            return true;
        }
    }
    return false;
}

// IS ALREADY TOUCHED --------------------------------------------------------------
bool EchoContainer::isAlreadyTouched(int _cubeId) {
    // If the cube has already been hit
    return (std::find(cubesTouched.begin(), cubesTouched.end(), _cubeId) != cubesTouched.end());
}
