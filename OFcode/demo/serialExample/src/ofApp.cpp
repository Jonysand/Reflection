#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	
	ofBackground(255);	
	ofSetLogLevel(OF_LOG_VERBOSE);
    data.allocate(7);
	
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();

	int baud = 9600;

	serial.setup("/dev/cu.usbmodem1432401", baud);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    if(x>350 || x<0 || y>600 || y<0){
        return;
    }
    tuple<int, int> grids = getGrids(x, y);
    if(get<0>(grids) != cord_x || get<1>(grids) != cord_y){
        cord_x = get<0>(grids);
        cord_y = get<1>(grids);
        if(cord_x%2==0){
            index = 12*cord_x+cord_y;
        }else if(cord_x%2==1){
            index = 12*(cord_x+1)-1-cord_y;
        }
        serial.writeByte(index);
    }
}

tuple<int, int> ofApp::getGrids(int cord_x, int cord_y){
    int gridX;
    int gridY;
    
    gridX = cord_x*gridWidth/350;
    gridY = cord_y*gridHeight/600;

    return make_tuple(gridX, gridY);
}
