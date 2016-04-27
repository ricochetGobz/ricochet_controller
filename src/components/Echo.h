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

class Echo{
public :
    // Constructor
    Echo(ofPoint _pos);
    
    // Methods
    void expand();
    void draw();
    bool checkCubeCollision(ofPoint _cubePos);
    bool souldRemoved();
    
private :
    // Vars
    float size = 5;
    ofPoint pos;
    ofColor color;
    
    float sizeMax = 265;
    float expandForce = 99;
    float vel = 0.05;
};

#endif /* Echo_h */
