#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxGui.h"
#include <math.h>
//#include "ofxCv.h"

//#define PI 3.14159265
#define MODEL_RESOLUTION_X 350
#define MODEL_RESOLUTION_Y 600
#define VIEW_WIDTH 640
#define VIEW_HEIGHT 480
#define roomWidth 2311.4 // mm
#define roomHeight 3975.1 // mm
#define gridWidth 7
#define gridHeight 12

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
    ofxCvGrayscaleImage grayPreImage;
    ofxCvGrayscaleImage grayImageDif;
	ofxCvGrayscaleImage grayThreshNear;
	ofxCvGrayscaleImage grayThreshFar;
	ofxCvContourFinder contourFinder;
    ofParameter<int> nearThreshold;
    ofParameter<int> farThreshold;
	int angle;
    ofParameter<int> blobMinArea;
    int depthDifThreshold = 5;
    ofParameter<int> findingMethod;
    // 1: simple threshold
    // 2: differential threshold
    // 3: horizontal
    // 4: horizontal with BGS
    
    // for BGS
    ofxCvGrayscaleImage backgroundImg;
    ofParameter<bool> captureBackground;
    ofParameter<int> simpleThreshold;
    
    //----
    // map
    //----
    std::vector<glm::vec2> srcPoints;
    std::vector<glm::vec2> dstPoints;
    tuple<int, int> getGrids(double cord_x, double cord_y);
    vector<tuple<int, int>> gridsList;
    double mapped_x;
    double mapped_y;
    cv::Mat homographyMat;
    ofFbo positionView;
    ofFbo kinectView;
    ofFbo modelView;
    bool homographyReady;
    int activePoint;
    int grid_x = 0;
    int grid_y = 0;
    
    //------
    // Sound
    //------
    ofSoundPlayer  BookSound;
    ofSoundPlayer  PhoneSound;
    ofSoundPlayer  ForestSound;
    ofSoundPlayer  LampSound;
    const unsigned char nullData = 1;
    const unsigned char treeData = 32;
    const unsigned char paintData = 224;
    const Byte nullDataFromArd = 1;
    const Byte bookData = 64;
    const Byte phoneData = 128;
    bool phoneNotiPlayed = false;
    float phoneNotiStartTime = 0;
    const Byte lampData = 192;
    bool lampPlayed = false;
    
    
    //-------
    // Serial
    //-------
#ifdef SERIAL
    ofSerial    serial;
    int dataFromArd;
    bool isTree = false;
    bool treeDataSent = false;
    bool isPaint = false;
    bool paintDataSent = false;
    bool nullDataSent = false;
    bool isBook = false;
    bool isPhone = false;
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
