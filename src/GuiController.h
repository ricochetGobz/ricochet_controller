//
//  GuiController.h
//  ricochet_controller
//
//  Created by Boulay Jérémie on 12/05/2016.
//
//

#ifndef GuiController_h
#define GuiController_h

#include "ofxOpenCv.h"
#include "ofxControlPanel.h"


class GuiController {

public:
    //// VARIABLES ////
    
    //// METHODES ////
    GuiController(ofxCvColorImage &_colorImg,
                  //ofxCvGrayscaleImage _depthImg,
                  // !!TEMP!! //
                  ofVideoPlayer &_tempVidPlayer,
                  // !!TEMP!! //
                  ofxCvGrayscaleImage &_thresholdImg,
                  ofxCvGrayscaleImage &_reworkImg);

    void update(int _nBlobs);
    void draw();

    // getters
    int getNearThreshold();
    int getFarThreshold();
    bool getBBlur();
    int getThreshold();
    int getMinArea();
    int getMaxArea();
    int getCubeSize();
    int getDilationTolerance();
    int getSizeTolerance();
    int getEchoVelocity();
    int getEchoSizeMax();
    int getCubeTimelapsActive();
    int getCubeTimelapsDesactive();

private:
    //// VARIABLES ////
    ofxControlPanel gui;
    // Infos
    ofParameterGroup stats;
    ofParameter <float> appFrameRate;
    ofParameter <int> nBlobs;
    ofParameter <int> nCubes;
    // threshold controls
    ofParameterGroup thresholdControls;
    ofParameter <int> nearThreshold;
    ofParameter <int> farThreshold;
    // contour finder render controls
    ofParameterGroup renderControls;
    ofParameter <bool> bBlur;
    ofParameter <int> threshold;
    ofParameter <int> minArea;
    ofParameter <int> maxArea;
    // cube detection controls
    ofParameterGroup cubeDetection;
    ofParameter <int> size;
    ofParameter <int> dilationTolerance;
    ofParameter <int> sizeTolerance;
    // cube timelaps
    ofParameter <int> timelapsActive;
    ofParameter <int> timelapsDesactive;
    // echo
    ofParameter <int> echoSizeMax;
    ofParameter <int> echoVelocity;

    
    //// METHODES ////

};


#endif /* GuiController_h */
