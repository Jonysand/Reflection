#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	// enable depth->video image calibration
	kinect.setRegistration(true);
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
//	kinect.init(false, false); // disable video image (faster fps)
	kinect.open();
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}
	grayImage.allocate(kinect.width, kinect.height);
	grayPreImage.allocate(kinect.width, kinect.height);
	grayImageDif.allocate(kinect.width, kinect.height);
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
    kinect.update();
    grayPreImage.setFromPixels(kinect.getDepthPixels());
    
    
    // simulation
    ofFboSettings settings;
    settings.width = modelWidth;
    settings.height = modelHeight;
    canvasFbo.allocate(settings);
    gridsPre = make_tuple(5,10);
    
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update() {
	
	ofBackground(100, 100, 100);
	
	kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels());
        cvAbsDiff(grayImage.getCvImage(), grayPreImage.getCvImage(), grayImageDif.getCvImage());
        
        
		grayImageDif.flagImageChanged();
//        grayImageDif.threshold(depthDifThreshold);
        ofPixels & pixDif = grayImageDif.getPixels();
        ofPixels & pixImg = grayImageDif.getPixels();
        int numPixels = pixDif.size();
        for(int i = 0; i < numPixels; i++) {
            if(pixDif[i] > depthDifThreshold) {
                pixDif[i] = 255;
            } else {
                pixDif[i] = 0;
            }
        }
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayImageDif, blobMinArea, (kinect.width*kinect.height)/2, 20, false);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	ofSetColor(255, 255, 255);
	
    // draw from the live kinect
    kinect.drawDepth(420, 10, 400, 300);
//    kinect.draw(420, 10, 400, 300);
    
//    grayImage.draw(10, 10, 400, 300);
    grayImageDif.draw(10, 10, 400, 300);
//    contourFinder.draw(10, 10, 400, 300);
//    contourFinder.draw();
    
    // draw silumation
    canvasFbo.begin();
    ofBackground(0);
    for(int i=0;i<contourFinder.nBlobs;i++){
        auto center =  contourFinder.blobs[0].centroid;
        int cord_x = center.x;
        int cord_y = kinect.getDistanceAt(center.x, center.y);
        grids = getGrids(cord_x, cord_y);
        if( (pow((get<0>(grids)-get<0>(gridsPre)),2)+pow((get<1>(grids)-get<1>(gridsPre)),2))<8
           ){
            gridsPre = grids;
        }
    }
    drawPositions(gridsPre, 255, 255,255);
    canvasFbo.end();
    canvasFbo.draw(10, 320);
	
	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
        
    if(kinect.hasAccelControl()) {
        reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
        << ofToString(kinect.getMksAccel().y, 2) << " / "
        << ofToString(kinect.getMksAccel().z, 2) << endl;
    } else {
        reportStream << "Note: this is a newer Xbox Kinect or Kinect For Windows device," << endl
		<< "motor / led / accel controls are not currently supported" << endl << endl;
    }
    
	reportStream << "set near threshold " << depthDifThreshold << " (press: + -)"<< "num blobs found " << contourFinder.nBlobs
	<< ", fps: " << ofGetFrameRate() << endl
	<< "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;

    if(kinect.hasCamTiltControl()) {
    	reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
        << "press 1-5 & 0 to change the led mode" << endl;
    }
    
//	ofDrawBitmapString(reportStream.str(), 20, 652);
    
    grayPreImage.setFromPixels(grayImage.getPixels());
}

//--------------------------------------------------------------
void ofApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
	switch (key) {
        case '+':
        case '=':
            depthDifThreshold ++;
            if (depthDifThreshold > 255) depthDifThreshold = 255;
            break;
        case '-':
            depthDifThreshold --;
            if (depthDifThreshold < 0) depthDifThreshold = 0;
            break;
            
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case '1':
			kinect.setLed(ofxKinect::LED_GREEN);
			break;
			
		case '2':
			kinect.setLed(ofxKinect::LED_YELLOW);
			break;
			
		case '3':
			kinect.setLed(ofxKinect::LED_RED);
			break;
			
		case '4':
			kinect.setLed(ofxKinect::LED_BLINK_GREEN);
			break;
			
		case '5':
			kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
			break;
			
		case '0':
			kinect.setLed(ofxKinect::LED_OFF);
			break;
			
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
	}
}



//---------------
// Test functions
//---------------

tuple<int, int> ofApp::getGrids(int cord_x, int cord_y){
    int gridX;
    int gridY;
    
    gridX = cord_x*gridWidth/kinect.width;
    gridY = cord_y*gridHeight/3975.1;

    return make_tuple(gridX, gridY);
}


void ofApp::drawPositions(tuple<int, int> gridCord, int r, int g, int b){
    ofSetColor(r, g, b);
    ofDrawRectangle(get<0>(gridCord)*modelWidth/gridWidth, get<1>(gridCord)*modelHeight/gridHeight, modelWidth/gridWidth, modelHeight/gridHeight);
}
