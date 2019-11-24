#pragma once

#include "ofMain.h"

#include "ofxCv.h"
#include "ofxGui.h"

#include "ezBall.h"

// This must match the display resolution of your projector
#define PROJECTOR_RESOLUTION_X 1920
#define PROJECTOR_RESOLUTION_Y 1080
#define MODEL_RESOLUTION_X 640
#define MODEL_RESOLUTION_Y 360

class ofApp : public ofBaseApp
{
public:
    void setup();
    
    void update();

    void draw();
    void drawProjector(ofEventArgs& args);

    void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    void addBall(int x, int y);

    std::vector<ezBall> balls;

    ofFbo renderFbo;
    ofFbo mappedFbo;
    ofPixels renderPixels;
    ofImage warpedImg;

    std::vector<glm::vec2> srcPoints;
    std::vector<glm::vec2> dstPoints;

    int activePoint;

    cv::Mat homographyMat;
    bool homographyReady;

    ofParameter<bool> adjustMapping;
    ofParameter<bool> projectWarped;

    ofxPanel guiPanel;
};
