//
//  KinectController.cpp
//  V 0.1.2
//  ricochet_test_kinect
//
//  Created by Boulay Jérémie on 15/04/2016.
//
//

#include "KinectController.h"

void KinectController::init() {
    // enable depth->video image calibration
    kinect.setRegistration(true);
    
    kinect.init();
    kinect.open();
    kinect.enableDepthNearValueWhite(true);
    
    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
    
    colorImg.allocate(kinect.width, kinect.height);
    depthImg.allocate(kinect.width, kinect.height);
    reworkImg.allocate(kinect.width, kinect.height);
    thresholdImg.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    
    ///// GUI INIT ////
    gui.setup("Ricochet - DEBUG", 0, 0, ofGetWidth(), ofGetHeight());
    //--------- PANEL 1 : DEBUG
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    gui.addDrawableRect("Kinect Video", &colorImg, OC_WIDTH, OC_HEIGHT);
    gui.addDrawableRect("Kinect Depth", &depthImg, OC_WIDTH, OC_HEIGHT);
    gui.setWhichColumn(1);
    gui.addDrawableRect("OpenCV Threshold", &thresholdImg, OC_WIDTH, OC_HEIGHT);
    gui.addDrawableRect("OpenCV Render", &reworkImg, OC_WIDTH*2, OC_HEIGHT*2);
    gui.setWhichColumn(2);
    // Stats
    gui.addChartPlotter(appFrameRate, 30, 80);
    stats.setName("Stats");
    stats.add( nBlobs.set("Blobs founds", 0) );
    stats.add( nCubes.set("Cubes founds", 0) );
    gui.addVariableLister(stats);
    
    //Threshold controls
    thresholdControls.setName("OpenCV threshold");
    thresholdControls.add(nearThreshold.set("nearThreshold", 165.0, 1.0, 255.0));
    thresholdControls.add(farThreshold.set("farThreshold", 158.0, 1.0, 255.0));
    gui.addGroup(thresholdControls);
    // Rework controls
    reworkControls.setName("OpenCV rework");
    reworkControls.add(bBlur.set("Blur", false));
    reworkControls.add(threshold.set("threshold", 0.0, 0.0, 255.0));
    gui.addGroup(reworkControls);
    // Render controls
    renderControls.setName("OpenCV render");
    renderControls.add(minArea.set("minArea", 370.0, 1.0, 3000.0));
    renderControls.add(maxArea.set("maxArea", 1040.0, 1.0, (OC_WIDTH*OC_HEIGHT)));
    gui.addGroup(renderControls);
}

void KinectController::update() {
    ///// GUI UPDATE /////
    gui.update();
    appFrameRate = ofGetFrameRate();
    nBlobs = contourFinder.nBlobs;

    ///// KINECT UPDATE /////
    kinect.update();
    
    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {
        
        // load grayscale depth image from the kinect source
        colorImg.setFromPixels(kinect.getPixels(), kinect.width, kinect.height);
        depthImg.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        
        // THRESHOLD
        // we do two thresholds - one for the far plane and one for the near plane
        // we then do a cvAnd to get the pixels which are a union of the two thresholds
        grayThreshNear = depthImg;
        grayThreshFar = depthImg;
        grayThreshNear.threshold(gui.getValueI("OpenCV_threshold:nearThreshold"), true);
        grayThreshFar.threshold(gui.getValueI("OpenCV_threshold:farThreshold"));
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), thresholdImg.getCvImage(), NULL);
        // update the cv images
        thresholdImg.flagImageChanged();
        
        // REWORK
        reworkImg = thresholdImg;
        if(bBlur) reworkImg.blurHeavily();
        if(threshold > 0) reworkImg.threshold(gui.getValueI("OpenCV_rework:threshold"));
        
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        contourFinder.findContours(reworkImg,
                                   gui.getValueI("OpenCV_render:minArea"),
                                   gui.getValueI("OpenCV_render:maxArea"),
                                   20, false);
    }

}

void KinectController::draw(int mode) {
    
    if(mode == NORMAL_MODE) {
        kinect.draw(2, 2, ofGetWidth() - 4, ofGetHeight() - 4);
        drawContourFinder(2, 2, ofGetWidth() - 4, ofGetHeight() - 4);
        
        // TODO show cubes
        
    } else if (mode == CALIBRATION_MODE){
        gui.draw();
        drawContourFinder(227, 246, OC_WIDTH*2, OC_HEIGHT*2);
    } else if (mode == CLOUD_MODE) {
        easyCam.begin();
        drawPointCloud();
        easyCam.end();
    }
}


void KinectController::drawPointCloud() {
    int w = 640;
    int h = 480;
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
}

//--------------------------------------------------------------
void KinectController::drawContourFinder(float x, float y, float w, float h) {
    ofNoFill();
    ofDrawRectangle(x, y, w, h);
    // we could draw the whole contour finder
    contourFinder.draw(x, y, w, h);
    
    // or, instead we can draw each blob individually from the blobs vector,
    // this is how to get access to them:
    //    for(int i = 0; i < contourFinder.nBlobs; i++) {
    //        ofRectangle r = contourFinder.blobs.at(i).boundingRect;
    //        r.x = x + (w*r.x)/320;
    //        r.y = y + (h*r.y)/240;
    //        r.width =  (w*r.width)/320;
    //        r.height = (h*r.height)/240;
    //
    //        ofSetColor(255);
    //        ofDrawRectangle(r);
    //
    //        // TODO Analyse blob
    //    }
}

bool KinectController::kinectIsConnected(){
    return kinect.isConnected();
}

void KinectController::open() {
    kinect.open();
}
