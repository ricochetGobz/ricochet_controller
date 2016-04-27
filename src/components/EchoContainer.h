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
        // Constructor
        EchoContainer(Cube _cube);

        // VARS
        vector<Echo> echoes;

        // METHODS
        void checkEchoCollision(Cube _cube);
        void update();
        void draw();

    private:
        // VARS
        vector<int> cubesTouched;
        /* temps */
        bool echoWithVel;

        // METHODS
        void createEcho(Cube _cube);
        bool isAlreadyTouched(int _cubeId);
};

#endif /* EchoContainer_hpp */
