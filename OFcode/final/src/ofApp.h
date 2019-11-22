#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

class ofApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
    void keyPressed(int key);
	
    //-------
    // Kinect
    //-------
	ofxKinect kinect;
	ofxCvGrayscaleImage grayImage;
	ofxCvGrayscaleImage grayThreshNear;
	ofxCvGrayscaleImage grayThreshFar;
	ofxCvContourFinder contourFinder;
	int nearThreshold;
    int farThreshold;
	int angle;
    int blobMinArea = 500;
    
    //----
    // map
    //----
    std::vector<glm::vec2> srcPoints;
    std::vector<glm::vec2> dstPoints;
    cv::Mat homographyMat;
    
    
    //--------------------------
    // simualtion mapping canvas
    //--------------------------
    ofFbo canvasFbo;
    tuple<int, int> getGrids(int cord_x, int cord_y);
    void drawPositions(tuple<int, int> gridCord, int r, int g, int b);
    int roomWidth = 91;
    int roomHeight = 156.5;
    int gridWidth = 7;
    int gridHeight = 12;
    int modelWidth = 280;
    int modelHeight = 480;
};
