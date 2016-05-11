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


    //// GUI INIT ////
    gui.setup("Ricochet - DEBUG", 0, 0, ofGetWidth(), ofGetHeight());
    // --------- PANEL 1 : DEBUG
    gui.setWhichPanel(0);
    // ----- Column 1
    gui.setWhichColumn(0);
    gui.addDrawableRect("Kinect Video", &colorImg, OC_WIDTH, OC_HEIGHT);
    //gui.addDrawableRect("Kinect Depth", &depthImg, OC_WIDTH, OC_HEIGHT);
    // !!TEMP!! //
    gui.addDrawableRect("Kinect Depth", &tempVidPlayer, OC_WIDTH, OC_HEIGHT);
    // !!TEMP!! //
    // ----- Column 2
    gui.setWhichColumn(1);
    gui.addDrawableRect("OpenCV Threshold", &thresholdImg, OC_WIDTH, OC_HEIGHT);
    gui.addDrawableRect("OpenCV Render", &reworkImg, OC_WIDTH*2, OC_HEIGHT*2);
    // ----- Column 3
    gui.setWhichColumn(2);
    // Stats
    gui.addChartPlotter(appFrameRate, 30, 80);
    stats.setName("Stats");
    stats.add( nBlobs.set("Blobs founds", 0) );
    stats.add( nCubes.set("Cubes founds", 0) );
    gui.addVariableLister(stats);
    //Threshold controls
    thresholdControls.setName("OpenCV threshold");
    thresholdControls.add(nearThreshold.set("nearThreshold", 255.0, 1.0, 255.0)); // (sauv : 165.0)
    thresholdControls.add(farThreshold.set("farThreshold", 146.0, 1.0, 255.0)); // (sauv : 158.0)
    gui.addGroup(thresholdControls);
    // Rework controls
    reworkControls.setName("OpenCV rework");
    reworkControls.add(bBlur.set("Blur", false));
    reworkControls.add(threshold.set("threshold", 0.0, 0.0, 255.0));
    gui.addGroup(reworkControls);
    // Render controls
    renderControls.setName("OpenCV render");
    renderControls.add(minArea.set("minArea", 231.0, 1.0, 3000.0)); // ( sauv : 300.0)
    renderControls.add(maxArea.set("maxArea", 1040.0, 1.0, (OC_WIDTH*OC_HEIGHT)));
    gui.addGroup(renderControls);
    cubeDetectionControls.setName("Cube detection");
    cubeDetectionControls.add(size.set("size", 25, 10, 60));
    cubeDetectionControls.add(dilationTolerance.set("dilationTolerance", 5, 0, 15));
    cubeDetectionControls.add(sizeTolerance.set("sizeTolerance", 7, 0, 30));
    gui.addGroup(cubeDetectionControls);
}

// UPDATE --------------------------------------------------------------
void KinectController::update() {
    //// GUI UPDATE /////
    gui.update();
    appFrameRate = ofGetFrameRate();
    nBlobs = contourFinder.nBlobs;

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
        grayThreshNear.threshold(gui.getValueI("OpenCV_threshold:nearThreshold"), true);
        grayThreshFar.threshold(gui.getValueI("OpenCV_threshold:farThreshold"));
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), thresholdImg.getCvImage(), NULL);
        // update the cv images
        thresholdImg.flagImageChanged();

        // REWORK RENDER
        reworkImg = thresholdImg;
        if(bBlur) reworkImg.blurHeavily();
        if(threshold > 0) reworkImg.threshold(gui.getValueI("OpenCV_rework:threshold"));

        // CONTOURS FINDER
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to true so we will get interior contours as well....
        contourFinder.findContours(reworkImg,
                                   gui.getValueI("OpenCV_render:minArea"),
                                   gui.getValueI("OpenCV_render:maxArea"),
                                   20, false);
    }

    //// CUBES FOUNDS UPDATE /////
    vector<ofRectangle> _detectedCubes;
    // for each forms found
    for(int i = 0; i < contourFinder.nBlobs; i++) {
        ofRectangle r = contourFinder.blobs.at(i).boundingRect;
        
        // if is aproximatively a square && if is approximatively at the good size.
        if((abs(r.width - r.height) < gui.getValueI("Cube_detection:dilationTolerance"))
           && (abs((r.width - gui.getValueI("Cube_detection:size"))) < gui.getValueI("Cube_detection:sizeTolerance"))) _detectedCubes.push_back(r);
    }

    detectedCubes = _detectedCubes;
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
    gui.draw();
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
