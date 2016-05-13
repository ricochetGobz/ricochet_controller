//
//  GuiController.cpp
//  ricochet_controller
//
//  Created by Boulay Jérémie on 12/05/2016.
//
//

#include "GuiController.h"
#include "ofxControlPanel.h"

// Screen size
#define RES 1.33333
#define WIDTH 800
#define HEIGHT WIDTH/RES
#define OC_WIDTH 200
#define OC_HEIGHT OC_WIDTH / RES


ofxCvGrayscaleImage depthImg; // grayscale depth image
// Threshold render
ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
ofxCvGrayscaleImage thresholdImg;
// Reworking
ofxCvGrayscaleImage reworkImg;
// contour finder
ofxCvContourFinder contourFinder;

// GUI CONTROLLER -----------------------------------------------------------
GuiController::GuiController(ofxCvColorImage &_colorImg,
                             //ofxCvGrayscaleImage _depthImg,
                             // !!TEMP!! //
                             ofVideoPlayer &_tempVidPlayer,
                             // !!TEMP!! //
                             ofxCvGrayscaleImage &_thresholdImg,
                             ofxCvGrayscaleImage &_reworkImg) {
    
    
    //// GUI INIT ////
    gui.setup("Ricochet - DEBUG", 0, 0, ofGetWidth(), ofGetHeight());
    // --------- PANEL 1 : DEBUG
    gui.setWhichPanel(0);
    // ----- Column 1
    gui.setWhichColumn(0);
    gui.addDrawableRect("Kinect Video", &_colorImg, OC_WIDTH, OC_HEIGHT);
    //gui.addDrawableRect("Kinect Depth", &_depthImg, OC_WIDTH, OC_HEIGHT);
    // !!TEMP!! //
    gui.addDrawableRect("Kinect Depth", &_tempVidPlayer, OC_WIDTH, OC_HEIGHT);
    // !!TEMP!! //
    // ----- Column 2
    gui.setWhichColumn(1);
    gui.addDrawableRect("OpenCV Threshold", &_thresholdImg, OC_WIDTH, OC_HEIGHT);
    gui.addDrawableRect("OpenCV Render", &_reworkImg, OC_WIDTH*2, OC_HEIGHT*2);
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
    // Render controls
    renderControls.setName("OpenCV render");
    renderControls.add(bBlur.set("Blur", false));
    renderControls.add(threshold.set("threshold", 0.0, 0.0, 255.0));
    renderControls.add(minArea.set("minArea", 231.0, 1.0, 3000.0)); // ( sauv : 300.0)
    renderControls.add(maxArea.set("maxArea", 1040.0, 1.0, (OC_WIDTH*OC_HEIGHT)));
    gui.addGroup(renderControls);
    cubeDetection.setName("Cube detection");
    cubeDetection.add(size.set("size", 25, 10, 60));
    cubeDetection.add(dilationTolerance.set("dilationTolerance", 5, 0, 15));
    cubeDetection.add(sizeTolerance.set("sizeTolerance", 7, 0, 30));
    cubeDetection.add(timelapsActive.set("timelapsActive", 7, 0, 30));
    cubeDetection.add(timelapsDesactive.set("timelapsDesactive", 7, 0, 30));
    cubeDetection.add(echoSizeMax.set("echoSizeMax", 7, 0, 30));
    cubeDetection.add(echoVelocity.set("echoVelocity", 7, 0, 30));
    gui.addGroup(cubeDetection);
}

// UPDATE -----------------------------------------------------------
void GuiController::update(int _nBlobs) {
    gui.update();
}

// DRAW -----------------------------------------------------------
void GuiController::draw() {
    gui.draw();
}

// GETTERS -----------------------------------------------------------
// - (for kinectCtrl)
int GuiController::getNearThreshold() {
  return gui.getValueI("OpenCV_threshold:nearThreshold");
}
int GuiController::getFarThreshold() {
  return gui.getValueI("OpenCV_threshold:farThreshold");
}
bool GuiController::getBBlur() {
    return bBlur;
}
int GuiController::getThreshold() {
  return gui.getValueI("OpenCV_render:threshold");
}
int GuiController::getMinArea() {
  return gui.getValueI("OpenCV_render:minArea");
}
int GuiController::getMaxArea() {
  return gui.getValueI("OpenCV_render:maxArea");
}
// - (for kinectCtrl)
int GuiController::getCubeSize() {
  return gui.getValueI("Cube_detection:size");
}
int GuiController::getDilationTolerance() {
  return gui.getValueI("Cube_detection:dilationTolerance");
}
int GuiController::getSizeTolerance() {
  return gui.getValueI("Cube_detection:sizeTolerance");
}
int GuiController::getEchoVelocity() {
  return -1;
}
int GuiController::getEchoSizeMax() {
  return -1;
}
int GuiController::getCubeTimelapsActive() {
  return -1;
}
int GuiController::getCubeTimelapsDesactive() {
  return -1;
}
