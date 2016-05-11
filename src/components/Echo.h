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
#define CONTACT_RATIO 2
#define FORCE_MIN 0.5
#define VEL 0.05

class Echo{

public :
    //// VARIABLES ////
    //// METHODES ////
    Echo(ofRectangle _cubeZone);
    void expand();
    void draw();
    bool checkCubeCollision(ofPoint _cubePos);
    bool souldRemove();

private :
    //// VARIABLES ////
    float size = 5.0;
    float expandForce = 99.0;

    ofPoint center;
    float sizeMax;

    //// METHODES ////
};

#endif /* Echo_h */
