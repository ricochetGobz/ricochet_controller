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

class KinectController {

public:
    //// VARIABLES ////
    
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
    
    //// METHODES ////
    // Basics
    void init();
    void update(bool _bBlur,
                int _nearThreshold,
                int _farThreshold,
                int _threshold,
                int _minArea,
                int _maxArea);
    
    // draw methods
    void drawRender(ofRectangle _renderZone);
    void drawControlPanel(ofRectangle _renderZone);
    void drawPointCloud();

    // kinect methods
    bool kinectIsConnected();
    void openKinect();

private:
    //// VARIABLES ////

    // KINECT / VIDEO
    ofxKinect kinect;

    // POINT CLOUD
    bool bDrawPointCloud;
    ofEasyCam easyCam;


    //// METHODES ////
    void drawContourFinder(ofRectangle _renderZone);
};

#endif /* KinectController_h */
