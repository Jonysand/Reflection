#pragma once

#include "ofMain.h"

class ezBall
{
public:
    void setup(int x, int y);

    void update(glm::vec2 force);
    void draw();

private:
    glm::vec2 pos;
    glm::vec2 vel;
    glm::vec2 acc;

    float mass;

    ofColor color;
};
