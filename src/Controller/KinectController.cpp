//
//  KinectController.cpp
//  ricochet_test_kinect
//
//  Created by Boulay Jérémie on 15/04/2016.
//
//

#include "KinectController.h"

// INIT --------------------------------------------------------------
void KinectController::init() {

    //// KINECT INIT ////
    kinect.init();
    kinect.open();
    kinect.setRegistration(true);
    kinect.enableDepthNearValueWhite(true);

    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }

    //// DEBUG SCREENS INIT ////
    colorImg.allocate(kinect.width, kinect.height);
    depthImg.allocate(kinect.width, kinect.height);
    reworkImg.allocate(kinect.width, kinect.height);
    thresholdImg.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);

    // !!TEMP!! //
    tempVidPlayer.load("videoKinectDepth.mp4");
    tempVidPlayer.play();
    tempVidPlayer.setLoopState(OF_LOOP_NORMAL);
    // !!TEMP!! //

}

// UPDATE --------------------------------------------------------------
void KinectController::update(bool _bBlur,
                              int _nearThreshold,
                              int _farThreshold,
                              int _threshold,
                              int _minArea,
                              int _maxArea) {
    //// KINECT VIDEO UPDATE /////
    // !!TEMP!! //
    //kinect.update();
    tempVidPlayer.update();
    // !!TEMP!! //

    // there is a new frame and we are connected
    // !!TEMP!! //
    //if(kinect.isFrameNew()) {
    if(tempVidPlayer.isFrameNew()) {
    // !!TEMP!! //

        //// OPEN CV UPDATE /////

        // load grayscale depth image from the kinect source
        // !!TEMP!! //
        //colorImg.setFromPixels(kinect.getPixels(), kinect.width, kinect.height);
        //depthImg.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        colorImg.setFromPixels(tempVidPlayer.getPixels(), kinect.width, kinect.height);
        depthImg = colorImg;
        // !!TEMP!! //

        // THRESHOLD
        // we do two thresholds - one for the far plane and one for the near plane
        // we then do a cvAnd to get the pixels which are a union of the two thresholds
        grayThreshNear = depthImg;
        grayThreshFar = depthImg;
        grayThreshNear.threshold(_nearThreshold
                                 , true);
        grayThreshFar.threshold(_farThreshold);
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), thresholdImg.getCvImage(), NULL);
        // update the cv images
        thresholdImg.flagImageChanged();

        // REWORK RENDER
        reworkImg = thresholdImg;
        if(_bBlur) reworkImg.blurHeavily();
        if(_threshold > 0) reworkImg.threshold(_threshold);

        // CONTOURS FINDER
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        contourFinder.findContours(reworkImg,
                                   _minArea,
                                   _maxArea,
                                   10, false);
    }
}

// DRAW ------------------------------------------------------------------------------
// - DRAW RENDER ---------------------------------------------------------------------
// - DRAW CONTROL PANEL --------------------------------------------------------------
// - DRAW POINT CLOUD ----------------------------------------------------------------
// - DRAW CONTOUR FINDER--------------------------------------------------------------
void KinectController::drawRender(ofRectangle _renderZone) {
    kinect.draw(_renderZone);
    drawContourFinder(_renderZone);
}
void KinectController::drawControlPanel(ofRectangle _renderZone) {
    drawContourFinder(_renderZone);
}
void KinectController::drawPointCloud() {
    int w = 640;
    int h = 480;
    easyCam.begin();

    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_POINTS);
    int step = 2;
    for(int y = 0; y < h; y += step) {
        for(int x = 0; x < w; x += step) {
            if(kinect.getDistanceAt(x, y) > 0) {
                mesh.addColor(kinect.getColorAt(x,y));
                mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
            }
        }
    }
    glPointSize(3);
    ofPushMatrix();
    // the projected points are 'upside down' and 'backwards'
    ofScale(1, -1, -1);
    ofTranslate(0, 0, -1000); // center the points a bit
    glEnable(GL_DEPTH_TEST);
    mesh.drawVertices();
    glDisable(GL_DEPTH_TEST);
    ofPopMatrix();

    easyCam.end();
}
void KinectController::drawContourFinder(ofRectangle _renderZone) {
    ofNoFill();
    ofDrawRectangle(_renderZone);
    contourFinder.draw(_renderZone);
}

// KINECTISCONNECTED -----------------------------------------------------------
bool KinectController::kinectIsConnected() {
    return kinect.isConnected();
}

// OPEN (connect kinect) -------------------------------------------------------
void KinectController::openKinect() {
    kinect.open();
}
