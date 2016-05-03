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
    
    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
    
    colorImg.allocate(kinect.width, kinect.height);
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    
    ///// GUI INIT ////
    gui.setup("Ricochet - DEBUG", (ofGetWidth()/3.5)*2.5, 0, ofGetWidth()/3.5, ofGetHeight());
    //--------- PANEL 1 : DEBUG
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    // Stats
    gui.addChartPlotter(appFrameRate, 30, 80);
    stats.setName("Stats");
    stats.add( nBlobs.set("Blobs founds", 0) );
    stats.add( nCubes.set("Cubes founds", 0) );
    gui.addVariableLister(stats);
    // Resultcontrol
    resultControls.setName("OpenCV result");
    resultControls.add(bDepthNearValue.set("deathNearValue", false));
    resultControls.add(nearThreshold.set("nearThreshold", 152.0, 1.0, 255.0));
    resultControls.add(farThreshold.set("farThreshold", 145.0, 1.0, 255.0));
    resultControls.add(minArea.set("minArea", 800.0, 1.0, 3000.0));
    resultControls.add(maxArea.set("maxArea", 3000.0, 1.0, (OC_WIDTH*OC_HEIGHT)));
    gui.addGroup(resultControls);
}

void KinectController::update() {
    ///// GUI UPDATE /////
    gui.update();
    appFrameRate = ofGetFrameRate();

    ///// KINECT UPDATE /////
    kinect.update();
    
    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {
        
        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        
        // we do two thresholds - one for the far plane and one for the near plane
        // we then do a cvAnd to get the pixels which are a union of the two thresholds
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(gui.getValueI("OpenCV_result:nearThreshold"), true);
        grayThreshFar.threshold(gui.getValueI("OpenCV_result:farThreshold"));
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        
        // update the cv images
        grayImage.flagImageChanged();
        
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        contourFinder.findContours(grayImage,
                                   gui.getValueI("OpenCV_result:minArea"),
                                   gui.getValueI("OpenCV_result:maxArea"),
                                   gui.getValueI("OpenCV_result:nBlobMax"),
                                   false);
    }

}

void KinectController::draw(int mode) {
    
    if(mode == NORMAL_MODE) {
        
        kinect.draw(2, 2, ofGetWidth() - 4, ofGetHeight() - 4);
        
        // TODO draw contourFinder
        // TODO show cubes
        
    } else if (mode == CALIBRATION_MODE){
        // draw from the live kinect
        kinect.drawDepth(10, 10, 400, 300);
        kinect.draw(420, 10, 400, 300);
        
        grayImage.draw(10, 320, 400, 300);
        contourFinder.draw(10, 320, 400, 300);
        // TODO just draw the gui
        gui.draw();
        
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

bool KinectController::kinectIsConnected(){
    return kinect.isConnected();
}

void KinectController::setDepthNearValue(bool white){
    if(white == depthNearValue){
        white = depthNearValue;
        kinect.enableDepthNearValueWhite(white);
    }
}

void KinectController::open() {
    kinect.open();
}
