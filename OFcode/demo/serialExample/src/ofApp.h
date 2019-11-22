#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	
public:
    void setup();
    void update();
    void draw();

    void mouseMoved(int x, int y );

    ofBuffer    data;
    unsigned char index;
    int gridWidth = 7;
    int gridHeight = 12;
    int cord_x = 0;
    int cord_y = 0;
    tuple<int, int> getGrids(int cord_x, int cord_y);
    
    ofSerial	serial;
    
};

