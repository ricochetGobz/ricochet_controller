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

//--------------------------------------------------------------
bool shouldRemove(Echo &e){
    return e.souldRemoved();
}

//--------------------------------------------------------------
void EchoContainer::update() {
    ofRemove(echoes, shouldRemove);
    
    for(vector<Echo>::iterator it = echoes.begin(); it != echoes.end(); ++it){
        (*it).expand();
    }
}

//--------------------------------------------------------------
void EchoContainer::draw() {
    for(vector<Echo>::iterator it = echoes.begin(); it != echoes.end(); ++it){
        (*it).draw();
    }
}

//--------------------------------------------------------------
void EchoContainer::createEcho(Cube _cube) {
    // sauve le cube emmeteur comme cube touché
    cubesTouched.push_back(_cube.cubeId);
    _cube.play();
    
    echoes.push_back(*new Echo(_cube.pos));
}

//--------------------------------------------------------------
void EchoContainer::checkEchoCollision(Cube _cube) {
    
    if(isAlreadyTouched(_cube.cubeId)) return;
    
    for(vector<Echo>::iterator it = echoes.begin(); it != echoes.end(); ++it) {

        // Si l'éco
        // peut activer d'autres cubes encore
        if((*it).checkCubeCollision(_cube.pos)) {
            
            //echoTab.erase(echoTab.begin() + i);
            
            // Créer un écho avec la liste des cubes que cet écho à déjà touché
            createEcho(_cube);
            return;
        }
    }
}

//--------------------------------------------------------------
bool EchoContainer::isAlreadyTouched(int _cubeId) {
    // Si le cube à déjà été touché
    if ( std::find(cubesTouched.begin(), cubesTouched.end(), _cubeId) != cubesTouched.end() ) {
        return true;
    }
    return false;
}