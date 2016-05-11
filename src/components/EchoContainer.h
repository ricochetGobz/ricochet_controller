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
    EchoContainer(Cube _cube);
    void checkEchoCollision(Cube _cube);
    void update();
    void draw(ofRectangle _renderZone);

private:
    //// VARIABLES ////
    vector<int> cubesTouched;
    
    //// METHODES ////
    void createEcho(Cube _cube);
    bool isAlreadyTouched(int _cubeId);
};

#endif /* EchoContainer_hpp */
