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
// drawerConvertor
#define CUBE_RATIO 0.1
#define CONTOUR_FINDER_RATIO 0.04
#define CONTACT_RATIO 2
// lifetime
#define LIFETIME_INCREMENT 10
#define LIFETIME_START 50
#define LIFETIME_ACTIVE 200
#define LIFETIME_MAX 1000

class Cube{
public:
    //// VARIABLES ////
    int cubeId;
    int linkedId = -1;
    bool contactZoneShowed = false;
    ofPoint pos;
    ofRectangle drawedShape;
    
    //// METHODES ////
    Cube(ofPoint _pos, int _id, ServerController _server);

    void draw(ofRectangle _renderZone);
    void update();
    void play();
    void loadSound(string soundPath);
    bool pointIsInsideDrawedShape(ofPoint _pointPos);
    //lifecicle
    void reduceLifeCicle();
    void increaseLifeCicle();
    bool isDead();
    bool isActive();
    // link with hard cube
    bool isKnow();

private:
    //// VARIABLES ////
    
    // id of sound
    int faceId;
    
    ofColor color;
    ofSoundPlayer cubeSound;
    
    // server saved here
    ServerController server;
    
    // lifetime
    int lifetime = 50;
};

#endif /* Cube_h */
