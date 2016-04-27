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

#define NORMAL_MODE 0
#define CALIBRATION_MODE 1
#define CLOUD_MODE 2

class KinectController {
 
public:
    // Constructor
    KinectController();
    
    void init();
    void update();
    void draw(int mode);
    void onKeyPressed(int key);
    bool kinectIsConnected();
    

    
private:
    
    void drawPointCloud();
    ofxKinect kinect;
    
    ofxCvColorImage colorImg;
    
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    ofxCvContourFinder contourFinder;
    
    bool bThreshWithOpenCV;
    bool bDrawPointCloud;
    
    int nearThreshold;
    int farThreshold;
    
    int angle;
    
    // used for viewing the point cloud
    ofEasyCam easyCam;

};

#endif /* KinectController_h */
