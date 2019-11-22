#include "ofApp.h"

void ofApp::setup()
{
    ofBackground(0);

    renderFbo.allocate(PROJECTOR_RESOLUTION_X, PROJECTOR_RESOLUTION_Y);
    warpedImg.allocate(MODEL_RESOLUTION_X, MODEL_RESOLUTION_Y, OF_IMAGE_COLOR);

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
    projectWarped.set("Project Warped", true);

    guiPanel.setup("Homography", "settings.json");
    guiPanel.add(adjustMapping);
    guiPanel.add(projectWarped);
}

void ofApp::update()
{
    if (adjustMapping)
    {
        // Copy points from glm to cv format.
        std::vector<cv::Point2f> cvSrcPoints;
        std::vector<cv::Point2f> cvDstPoints;
        for (int i = 0; i < srcPoints.size(); i++)
        {
            // Scale points to projector dimensions.
            cvSrcPoints.push_back(cv::Point2f(srcPoints[i].x * PROJECTOR_RESOLUTION_X, srcPoints[i].y * PROJECTOR_RESOLUTION_Y));
            cvDstPoints.push_back(cv::Point2f(dstPoints[i].x * PROJECTOR_RESOLUTION_X, dstPoints[i].y * PROJECTOR_RESOLUTION_Y));
        }

        // Generate a homography from the two sets of points.
        homographyMat = cv::findHomography(cv::Mat(cvSrcPoints), cv::Mat(cvDstPoints));
        homographyReady = true;
    }

    glm::vec2 gravity = glm::vec2(0, 9.8f);
    renderFbo.begin();
    {
        ofClear(255, 255);

        for (int i = 0; i < balls.size(); i++)
        {
            balls[i].update(gravity);
            balls[i].draw();
        }
    }
    renderFbo.end();

    if (homographyReady)
    {
        // Read the FBO to pixels.
        renderFbo.readToPixels(renderPixels);

        // Warp the pixels into a new image.
        warpedImg.setFromPixels(renderPixels);
        ofxCv::warpPerspective(renderPixels, warpedImg, homographyMat, CV_INTER_LINEAR);
        warpedImg.update();
    }
}

void ofApp::draw()
{
    ofSetColor(255);

    // Draw unwarped image on the left.
    renderFbo.draw(0, 0, 640, 360);

    if (homographyReady)
    {
        // Draw warped image on the right.
        warpedImg.draw(640, 0, 640, 360);
    }

    if (adjustMapping)
    {
        // Draw mapping points.
        for (int i = 0; i < srcPoints.size(); i++)
        {
            ofSetColor(0, 0, 255);
            glm::vec2 srcPt = glm::vec2(ofMap(srcPoints[i].x, 0, 1, 0, 640), ofMap(srcPoints[i].y, 0, 1, 0, 360));
            ofDrawCircle(srcPt, 10);

            ofSetColor(255, 0, 0);
            glm::vec2 dstPt = glm::vec2(ofMap(dstPoints[i].x, 0, 1, 640, 1280), ofMap(dstPoints[i].y, 0, 1, 0, 360));
            ofDrawCircle(dstPt, 10);

            ofSetColor(255, 0, 255);
            ofDrawLine(srcPt, dstPt);
        }
    }

    guiPanel.draw();
}

void ofApp::drawProjector(ofEventArgs& args)
{
    ofBackground(0);
    ofSetColor(255);

    if (homographyReady && projectWarped)
    {
        warpedImg.draw(0, 0);
    }
    else
    {
        renderFbo.draw(0, 0);
    }

    if (adjustMapping)
    {
        // Draw mapping dst points.
        for (int i = 0; i < dstPoints.size(); i++)
        {
            ofSetColor(255, 0, 0);
            glm::vec2 dstPt = glm::vec2(dstPoints[i].x * PROJECTOR_RESOLUTION_X, dstPoints[i].y * PROJECTOR_RESOLUTION_Y);
            ofDrawCircle(dstPt, 20);
        }
    }
}

void ofApp::addBall(int x, int y)
{
    // Add a new ezBall.
    balls.push_back(ezBall());
    // Setup the last added ezBall.
    balls.back().setup(x, y);
}

void ofApp::keyPressed(int key)
{
    if (key == ' ')
    {
        balls.clear();
    }
}

void ofApp::mouseDragged(int x, int y, int button)
{
    if (adjustMapping)
    {
        if (activePoint > -1)
        {
            // Move the active Point under the mouse, but stick to edges.
            glm::vec2 normPt = glm::vec2(ofMap(x, 640, 1280, 0, 1, true), ofMap(y, 0, 360, 0, 1, true));
            dstPoints[activePoint] = normPt;
        }
    }
    else
    {
        // Only add a ball if we're dragging in the preview window.
        if (ofInRange(x, 0, 640) && ofInRange(y, 0, 360))
        {
            // Remap the ball to the FBO resolution.
            int ballX = ofMap(x, 0, 640, 0, renderFbo.getWidth());
            int ballY = ofMap(y, 0, 360, 0, renderFbo.getHeight());
            addBall(ballX, ballY);
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
            glm::vec2 dstPt = glm::vec2(ofMap(dstPoints[i].x, 0, 1, 640, 1280), ofMap(dstPoints[i].y, 0, 1, 0, 360));
            glm::vec2 mousePt = glm::vec2(x, y);
            if (glm::distance(dstPt, mousePt) < 20)
            {
                // Close enough, let's grab this one.
                activePoint = i;
                break;
            }
        }
    }
    else
    {
        mouseDragged(x, y, button);
    }
}

void ofApp::mouseReleased(int x, int y, int button)
{
    if (adjustMapping)
    {
        activePoint = -1;
    }
}
