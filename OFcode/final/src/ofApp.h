#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxGui.h"

#define MODEL_RESOLUTION_X 350
#define MODEL_RESOLUTION_Y 600
#define VIEW_WIDTH 640
#define VIEW_HEIGHT 480

//--------------------------
// comment/uncommment option

#define SERIAL
//--------------------------

class ofApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
    void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
	
    //-------
    // Kinect
    //-------
	ofxKinect kinect;
	ofxCvGrayscaleImage grayImage;
	ofxCvGrayscaleImage grayThreshNear;
	ofxCvGrayscaleImage grayThreshFar;
	ofxCvContourFinder contourFinder;
    ofParameter<int> nearThreshold;
    ofParameter<int> farThreshold;
	int angle;
    int blobMinArea = 500;
    
    //----
    // map
    //----
    std::vector<glm::vec2> srcPoints;
    std::vector<glm::vec2> dstPoints;
    tuple<int, int> getGrids(double cord_x, double cord_y);
    vector<tuple<int, int>> gridsList;
    cv::Mat homographyMat;
    ofFbo positionView;
    ofFbo kinectView;
    ofFbo modelView;
    bool homographyReady;
    int activePoint;
    int grid_x = 0;
    int grid_y = 0;
    int gridWidth = 7;
    int gridHeight = 12;
    
    //-------
    // Serial
    //-------
#ifdef SERIAL
    ofSerial    serial;
    unsigned char index;
#endif
    
    //--------------------------
    // simualtion mapping canvas
    //--------------------------
    void drawPositions(vector<tuple<int, int>> gridsList, int r, int g, int b);
    
    //----
    // GUI
    //----
    ofParameter<bool> adjustMapping;
    ofxPanel guiPanel;
};
