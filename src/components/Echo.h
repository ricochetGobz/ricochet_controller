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
// drawerConvertor
#define CONTOUR_FINDER_RATIO 0.04

#define FORCE_MIN 0.5
#define VEL 0.05
#define SIZE_MAX 128.0 // cubeSize / 2 - ((640 / 10) * 2);

class Echo{

public :
    //// VARIABLES ////
    //// METHODES ////
    Echo(ofPoint _pos);
    void update();
    void draw(ofRectangle _renderZone);
    
    bool checkCubeCollision(ofPoint _cubePos);
    bool souldRemove();

private :
    //// VARIABLES ////
    float size;
    float expandForce;

    ofPoint center;
    ofPoint pos;

    //// METHODES ////
};

#endif /* Echo_h */
