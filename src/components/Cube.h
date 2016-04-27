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

class Cube{
    public:
        // Constructor
        Cube(ofPoint _pos, int _id);

        // VARS
        int cubeId;
        bool contactZoneShowed = false;
        ofPoint pos;

        // METHODS
        void draw();
        void moveTo(ofPoint _pos);
        void play();
        void loadSound(string soundPath);
        bool pointIsInside(ofPoint _pointPos);

    private:
        // VARS
        int faceId;
        int size = 70;
        float contactArea = 245;

        ofColor color;
        ofSoundPlayer cubeSound;
    
        vector<ofColor> colors;

        // METHODS
};

#endif /* Cube_h */
