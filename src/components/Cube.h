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
#include "ServerController.h"

//// CONST ////
#define SIZE 70
#define CONTACT_AREA 245.0

class Cube{
public:
    //// VARIABLES ////
    int cubeId;
    bool contactZoneShowed = false;
    ofPoint pos;
    //// METHODES ////
    Cube(ofPoint _pos, int _id, ServerController _server);

    void draw();
    void moveTo(ofPoint _pos);
    void play();
    void loadSound(string soundPath);
    bool pointIsInside(ofPoint _pointPos);

private:
    //// VARIABLES ////
    // id of sound
    int faceId;
    // position
    ofPoint posToDraw;
    ofColor color;
    ofSoundPlayer cubeSound;
    // server saved here
    ServerController server;
};

#endif /* Cube_h */
