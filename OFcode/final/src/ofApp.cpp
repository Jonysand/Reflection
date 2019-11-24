#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //-------------
    // Kinect setup
    //-------------
    kinect.setRegistration(true);
    kinect.init();
    //	kinect.init(bool infrared=false, bool video=true, bool texture=true)
    kinect.open();
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    nearThreshold.set("Near Threshold", 250, 0, 255);
    farThreshold.set("Far Threshold", 230, 0, 255);
    angle = 0;
    kinect.setCameraTiltAngle(angle);
    
    
    //--------------
    // Mapping setup
    //--------------
    kinectView.allocate(kinect.width, kinect.height);
    modelView.allocate(MODEL_RESOLUTION_X, MODEL_RESOLUTION_Y);
    positionView.allocate(kinect.width, kinect.height);
    srcPoints.push_back(glm::vec2(0, 0));
    srcPoints.push_back(glm::vec2(1, 0));
    srcPoints.push_back(glm::vec2(0, 1));
    srcPoints.push_back(glm::vec2(1, 1));
    dstPoints.push_back(glm::vec2(0, 0));
    dstPoints.push_back(glm::vec2(1, 0));
    dstPoints.push_back(glm::vec2(0, 1));
    dstPoints.push_back(glm::vec2(1, 1));
    activePoint = -1;
    homographyReady = false;
    adjustMapping.set("Adjust Mapping", false);
    
    
    //-------------
    // Serial setup
    //-------------
#ifdef SERIAL
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    int baud = 9600;
    serial.setup("/dev/tty.usbmodem1432401", baud);
#endif
    
    //----------
    // GUI setup
    //----------
    guiPanel.setup("PCOMP final", "settings.json");
    guiPanel.add(adjustMapping);
    guiPanel.add(nearThreshold);
    guiPanel.add(farThreshold);
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update() {
    ofBackground(100, 100, 100);
    
    //---------------
    // Adjust Mapping
    //---------------
    if (adjustMapping)
    {
        // Copy points from glm to cv format.
        std::vector<cv::Point2f> cvSrcPoints;
        std::vector<cv::Point2f> cvDstPoints;
        for (int i = 0; i < srcPoints.size(); i++)
        {
            // Scale points to projector dimensions.
            cvSrcPoints.push_back(cv::Point2f(srcPoints[i].x * VIEW_WIDTH, srcPoints[i].y * VIEW_HEIGHT));
            cvDstPoints.push_back(cv::Point2f(dstPoints[i].x * MODEL_RESOLUTION_X, dstPoints[i].y * MODEL_RESOLUTION_Y));
        }
        // Generate a homography from the two sets of points.
        homographyMat = cv::findHomography(cv::Mat(cvSrcPoints), cv::Mat(cvDstPoints));
        homographyReady = true;
    }
    
    
    //-------
    // Kinect
    //-------
    kinect.update();
    // draw in kinectView
    if(kinect.isFrameNew()) {
        grayImage.setFromPixels(kinect.getDepthPixels());
        // we do two thresholds - one for the far plane and one for the near plane
        // we then do a cvAnd to get the pixels which are a union of the two thresholds
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(nearThreshold, true);
        grayThreshFar.threshold(farThreshold);
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        grayImage.flagImageChanged();
        contourFinder.findContours(grayImage, blobMinArea, (kinect.width*kinect.height)/2, 20, false);
        if(homographyReady){
            for(int i=0;i<contourFinder.nBlobs;i++){
                auto center =  contourFinder.blobs[i].centroid;
                std::vector<double> cvSrcMat;
                cvSrcMat.push_back(double(center.x));
                cvSrcMat.push_back(double(center.y));
                cvSrcMat.push_back(double(1));
                
                cv::Mat tempMat = homographyMat*cv::Mat(cvSrcMat).reshape(1,3);
                double mapped_x = tempMat.at<double>(0)/tempMat.at<double>(2);
                double mapped_y = tempMat.at<double>(1)/tempMat.at<double>(2);
                
                tuple<int, int> grids = getGrids(mapped_x, mapped_y);
                grid_x = get<0>(grids);
                grid_y = get<1>(grids);
#ifdef SERIAL
                if(grid_x%2==0){
                    index = 12*grid_x+grid_y;
                }else if(grid_x%2==1){
                    index = 12*(grid_x+1)-1-grid_y;
                }
                serial.writeByte(index);
#endif
                gridsList.push_back(grids);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    //----------------
    // draw when adjusting
    //----------------
    if(adjustMapping){
        kinectView.begin();
        kinect.drawDepth(0, 0);
        kinectView.end();
        positionView.begin();
        grayImage.draw(0, 0);
        positionView.end();
        modelView.begin();
        drawPositions(gridsList, 255, 255, 255);
        modelView.end();
        
        positionView.draw(10, 10, VIEW_WIDTH, VIEW_HEIGHT);
        kinectView.draw(10, 20+VIEW_HEIGHT, VIEW_WIDTH, VIEW_HEIGHT);
        if (homographyReady)
        {
            // Draw warped image on the right.
            modelView.draw(20+VIEW_WIDTH, 10, MODEL_RESOLUTION_X, MODEL_RESOLUTION_Y);
        }
        // draw instructions
        ofSetColor(255, 255, 255);
        stringstream reportStream;
        reportStream << "num blobs found " << contourFinder.nBlobs
        << ", fps: " << ofGetFrameRate() << endl
        << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;
        if(kinect.hasCamTiltControl()) {
            reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
            << "press 1-5 & 0 to change the led mode" << endl;
        }
        ofDrawBitmapString(reportStream.str(), 20+VIEW_WIDTH, 20+MODEL_RESOLUTION_Y);
        
        //--------------------
        // Draw mapping points
        //--------------------
        for (int i = 0; i < srcPoints.size(); i++)
        {
            ofSetColor(0, 0, 255);
            glm::vec2 srcPt = glm::vec2(ofMap(srcPoints[i].x, 0, 1, 0, VIEW_WIDTH)+10, ofMap(srcPoints[i].y, 0, 1, 0, VIEW_HEIGHT)+10);
            ofDrawCircle(srcPt, 10);

            ofSetColor(255, 0, 0);
            glm::vec2 dstPt = glm::vec2(ofMap(dstPoints[i].x, 0, 1, 0, MODEL_RESOLUTION_X)+20+VIEW_WIDTH, ofMap(dstPoints[i].y, 0, 1, 0, MODEL_RESOLUTION_Y)+10);
            ofDrawCircle(dstPt, 10);

            ofSetColor(255, 0, 255);
            ofDrawLine(srcPt, dstPt);
        }
        ofSetColor(255, 255, 255);
    }
    
    guiPanel.draw();
    gridsList.clear();
}

//--------------------------------------------------------------
void ofApp::exit() {
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();
}

//--------------------------------------------------------------
tuple<int, int> ofApp::getGrids(double cord_x, double cord_y){
    int gridX;
    int gridY;
    
    gridX = cord_x*gridWidth/MODEL_RESOLUTION_X;
    gridY = cord_y*gridHeight/MODEL_RESOLUTION_Y;
    
    return make_tuple(gridX, gridY);
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
    switch (key) {
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

void ofApp::mouseDragged(int x, int y, int button)
{
    if (adjustMapping)
    {
        if (activePoint > -1)
        {
            // Move the active Point under the mouse, but stick to edges.
            glm::vec2 normPt = glm::vec2(ofMap(x, 10, VIEW_WIDTH+10, 0, 1, true), ofMap(y, 10, VIEW_HEIGHT+10, 0, 1, true));
            srcPoints[activePoint] = normPt;
        }
    }
}

void ofApp::mousePressed(int x, int y, int button)
{
    if (adjustMapping)
    {
        // Try to snap to a dst point.
        for (int i = 0; i < dstPoints.size(); i++)
        {
            glm::vec2 srcPt = glm::vec2(ofMap(srcPoints[i].x, 0, 1, 0, VIEW_WIDTH)+10, ofMap(srcPoints[i].y, 0, 1, 0, VIEW_HEIGHT)+10);
            glm::vec2 mousePt = glm::vec2(x, y);
            if (glm::distance(srcPt, mousePt) < 20)
            {
                // Close enough, let's grab this one.
                activePoint = i;
                break;
            }
        }
    }
}

void ofApp::mouseReleased(int x, int y, int button)
{
    if (adjustMapping)
    {
        activePoint = -1;
    }
}

//---------------
// Test functions
//---------------
void ofApp::drawPositions(vector<tuple<int, int>> gridsList, int r, int g, int b){
    ofBackground(0);
    ofSetColor(r, g, b);
    for(int i=0; i<gridsList.size();i++){
        ofDrawRectangle(get<0>(gridsList[i])*MODEL_RESOLUTION_X/gridWidth, get<1>(gridsList[i])*MODEL_RESOLUTION_Y/gridHeight, MODEL_RESOLUTION_X/gridWidth, MODEL_RESOLUTION_Y/gridHeight);
    }
}
