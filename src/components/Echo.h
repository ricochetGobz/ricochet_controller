//
//  Echo.h
//  ricochet_proto_echo
//
//  Created by Arthur Robert on 10/12/15.
//
//

#ifndef Echo_h
#define Echo_h

#include "ofMain.h"

//// CONST ////
#define SIZE_MAX 265
#define FORCE_MIN 0.9
#define VEL 0.05

class Echo{

public :
    //// VARIABLES ////
    //// METHODES ////
    Echo(ofPoint _pos);
    void expand();
    void draw();
    bool checkCubeCollision(ofPoint _cubePos);
    bool souldRemove();

private :
    //// VARIABLES ////
    float size = 5.0;
    float expandForce = 99.0;

    ofPoint pos;

    //// METHODES ////
};

#endif /* Echo_h */
