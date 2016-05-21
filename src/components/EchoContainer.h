//
//  EchoContainer.hpp
//  ricochet_proto_echo
//
//  Created by Boulay Jérémie on 09/02/2016.
//
//

#ifndef EchoContainer_h
#define EchoContainer_h

#include "Cube.h"
#include "Echo.h"

class EchoContainer {

public:
    //// VARIABLES ////
    vector<Echo> echoes;

    //// METHODES ////
    EchoContainer();
    void update();
    void draw(ofRectangle _renderZone);
    
    bool checkEchoesCollision(int _cubeId, ofPoint _cubePos);
    void createEcho(int _cubeId, ofPoint _cubePos);

private:
    //// VARIABLES ////
    vector<int> cubesTouched;
    
    //// METHODES ////
    bool isAlreadyTouched(int _cubeId);
};

#endif /* EchoContainer_hpp */
