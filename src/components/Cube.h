//
//  Cube.h
//  ricochet_proto_echo
//
//  Created by Arthur Robert on 10/12/15.
//
//

#ifndef Cube_h
#define Cube_h

#include "ofMain.h"

//// CONST ////
// drawerConvertor
#define CUBE_RATIO 0.1
#define CONTOUR_FINDER_RATIO 0.03
#define CONTACT_RATIO 2
// lifetime
#define LIFETIME_INCREMENT 10
#define LIFETIME_START 50
#define LIFETIME_DETECTED 100
#define LIFETIME_SEACHING_CONNECTED_CUBE 110
#define LIFETIME_MAX 1000

class Cube{
public:
    //// VARIABLES ////
    int cubeId;
    int connectedCubeId = -1;
    int faceId = -1;
    bool contactZoneShowed = false;
    bool locked = false;
    ofPoint pos;
    ofRectangle drawedShape;

    //// METHODES ////
    Cube(ofPoint _pos, int _id);

    void draw(ofRectangle _renderZone);
    void update();
    void play();
    void loadSound(string soundPath);
    bool pointIsInsideDrawedShape(ofPoint _pointPos);
    //lifecicle
    void reduceLifeCicle();
    void increaseLifeCicle();
    void kill();
    // link with hard cube
    bool isDead();
    bool isDetected();
    bool isSeachingCubeMode();
    bool isLinkedToConnectedCube();

    void setFace(int _faceId);

private:
    //// VARIABLES ////

    ofColor color;
    ofSoundPlayer cubeSound;

    // lifetime
    int lifetime = 50;
};

#endif /* Cube_h */
