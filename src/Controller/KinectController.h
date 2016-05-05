//
//  KinectController.h
//  ricochet_test_kinect
//
//  Created by Boulay Jérémie on 15/04/2016.
//
//

#ifndef KinectController_h
#define KinectController_h

#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxControlPanel.h"

//// CONST ////
// Mode
#define NORMAL_MODE 0
#define CALIBRATION_MODE 1
#define CLOUD_MODE 2
// Screen size
#define RES 1.33333
#define WIDTH 800
#define HEIGHT WIDTH/RES
#define OC_WIDTH 200
#define OC_HEIGHT OC_WIDTH/RES


class KinectController {

public:
    //// VARIABLES ////
    vector<ofRectangle> detectedCubes;
    
    //// METHODES ////
    // Basics
    void init();
    void update(int _mode);
    void draw();

    bool kinectIsConnected();
    void open();
    vector<ofRectangle> getDetectedCubes();

private:
    //// VARIABLES ////
    int mode;

    // GUI
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
    // rework controls
    ofParameterGroup reworkControls;
    ofParameter <bool> bBlur;
    ofParameter <int> threshold;
    // contour finder render controls
    ofParameterGroup renderControls;
    ofParameter <int> minArea;
    ofParameter <int> maxArea;
    // cube detection controls
    ofParameterGroup cubeDetectionControls;
    ofParameter <int> size;
    ofParameter <int> dilationTolerance;
    ofParameter <int> sizeTolerance;

    // KINECT / VIDEO
    ofxKinect kinect;
    // !!TEMP!! //
    ofVideoPlayer tempVidPlayer;
    // !!TEMP!! //

    // OPENCV
    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage depthImg; // grayscale depth image
    // Threshold render
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    ofxCvGrayscaleImage thresholdImg;
    // Reworking
    ofxCvGrayscaleImage reworkImg;
    // contour finder
    ofxCvContourFinder contourFinder;

    // POINT CLOUD
    bool bDrawPointCloud;
    // used for viewing the point cloud
    ofEasyCam easyCam;


    //// METHODES ////
    void drawPointCloud();
    void drawContourFinder(float x, float y, float w, float h);
};

#endif /* KinectController_h */
