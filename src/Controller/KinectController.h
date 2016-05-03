//
//  KinectController.h
//  V 0.1.2
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

#define NORMAL_MODE 0
#define CALIBRATION_MODE 1
#define CLOUD_MODE 2

#define RES 1.33333
#define WIDTH 800
#define HEIGHT WIDTH/RES
#define OC_WIDTH 200
#define OC_HEIGHT OC_WIDTH/RES

class KinectController {
 
public:
    void init();
    void update();
    void draw(int mode);
    bool kinectIsConnected();
    void setDepthNearValue(bool white);
    void open();
    
private:
    // GUI
    ofxControlPanel gui;
    // Infos
    ofParameterGroup stats;
    ofParameter <float> appFrameRate;
    ofParameter <int> nBlobs;
    ofParameter <int> nCubes;
    // OpenCV controls
    ofParameterGroup resultControls;
    ofParameter <bool> bDepthNearValue;
    ofParameter <int> nearThreshold;
    ofParameter <int> farThreshold;
    ofParameter <int> minArea;
    ofParameter <int> maxArea;

    
    void drawPointCloud();
    ofxKinect kinect;
    
    ofxCvColorImage colorImg;
    
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    ofxCvContourFinder contourFinder;
    
    bool bDrawPointCloud;
    bool depthNearValue;
    
    // used for viewing the point cloud
    ofEasyCam easyCam;

};

#endif /* KinectController_h */
