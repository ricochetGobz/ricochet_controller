//
//  EchoContainer.cpp
//  ricochet_proto_echo
//
//  Created by Boulay Jérémie on 09/02/2016.
//
//

#include "EchoContainer.h"
#include "Cube.h"


EchoContainer::EchoContainer(Cube _cube){
    createEcho(_cube);
}


// UPDATE --------------------------------------------------------------
// - SHOULD REMOVE --------------------------------------------------------------
bool shouldRemove(Echo &e){
    return e.souldRemove();
}
void EchoContainer::update() {
    ofRemove(echoes, shouldRemove);

    for(vector<Echo>::iterator it = echoes.begin(); it != echoes.end(); ++it){
        (*it).expand();
    }
}

// DRAW --------------------------------------------------------------
void EchoContainer::draw(ofRectangle _renderZone) {
    for(vector<Echo>::iterator it = echoes.begin(); it != echoes.end(); ++it){
        (*it).draw(_renderZone);
    }
}

// CREATE ECHO --------------------------------------------------------------
void EchoContainer::createEcho(Cube _cube) {
    // Save the native cube like touched
    cubesTouched.push_back(_cube.cubeId);
    echoes.push_back(*new Echo(_cube.pos));

    _cube.play();
}

// CHECK ECHO COLLISION --------------------------------------------------------------
void EchoContainer::checkEchoCollision(Cube _cube) {
    
    if(isAlreadyTouched(_cube.cubeId)) return;

    for(vector<Echo>::iterator it = echoes.begin(); it != echoes.end(); ++it) {
        // If echoes can activate another cubes
        if((*it).checkCubeCollision(_cube.pos)) {
            createEcho(_cube);
            return;
        }
    }
}

// IS ALREADY TOUCHED --------------------------------------------------------------
bool EchoContainer::isAlreadyTouched(int _cubeId) {
    // If the cube has already been hit
    return (std::find(cubesTouched.begin(), cubesTouched.end(), _cubeId) != cubesTouched.end());
}
