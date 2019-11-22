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
	
	void drawPointCloud();
	
    void keyPressed(int key);
	
    // Kinect
	ofxKinect kinect;
	ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayPreImage;
    ofxCvGrayscaleImage grayImageDif;
	ofxCvContourFinder contourFinder;
	int angle;
    int blobMinArea = 500;
    int depthDifThreshold = 5;
    
    // simualtion mapping canvas
    ofFbo canvasFbo;
    tuple<int, int> getGrids(int cord_x, int cord_y);
    tuple<int, int> grids;
    tuple<int, int> gridsPre;
    void drawPositions(tuple<int, int> gridCord, int r, int g, int b);
    int roomWidth = 91;
    int roomHeight = 156.5;
    int gridWidth = 7;
    int gridHeight = 12;
    int modelWidth = 280;
    int modelHeight = 480;
};
