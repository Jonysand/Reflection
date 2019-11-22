#include "ofMain.h"
#include "ofApp.h"

int main()
{
    ofGLFWWindowSettings settings;

    settings.setSize(1280, 720);
    settings.setPosition(ofVec2f(100, 100));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

    settings.setSize(PROJECTOR_RESOLUTION_X, PROJECTOR_RESOLUTION_Y);
    settings.setPosition(ofVec2f(ofGetScreenWidth(), 0));
    settings.resizable = false;
    settings.decorated = false;
    settings.shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> secondWindow = ofCreateWindow(settings);
    secondWindow->setVerticalSync(false);

    shared_ptr<ofApp> mainApp(new ofApp);
    ofAddListener(secondWindow->events().draw, mainApp.get(), &ofApp::drawProjector);

    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
