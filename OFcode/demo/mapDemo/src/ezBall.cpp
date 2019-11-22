#include "ezBall.h"

#include "ofApp.h"

void ezBall::setup(int x, int y)
{
    pos = glm::vec2(x, y);
    mass = ofRandom(10, 30);
    color = ofColor(ofRandom(127, 255), ofRandom(127, 255), ofRandom(127, 255));
}

void ezBall::update(glm::vec2 force)
{
    // Add force.
    acc += force / mass;

    if (glm::length(vel) > 0)
    {
        // Add friction.
        glm::vec2 friction = glm::normalize(vel * -1) * 0.01f;
        acc += friction;
    }

    // Apply acceleration, then reset it!
    vel += acc;
    acc = glm::vec2(0.0f);

    // Move object.
    pos += vel;

    // Bounce off walls, taking radius into consideration.
    if (pos.x - mass < 0 || pos.x + mass > PROJECTOR_RESOLUTION_X)
    {
        pos.x = ofClamp(pos.x, mass, PROJECTOR_RESOLUTION_X - mass);
        vel.x *= -1;
    }
    if (pos.y - mass < 0 || pos.y + mass > PROJECTOR_RESOLUTION_Y)
    {
        pos.y = ofClamp(pos.y, mass, PROJECTOR_RESOLUTION_Y - mass);
        vel.y *= -1;
    }
}

void ezBall::draw()
{
    ofSetColor(color);
    ofDrawCircle(pos, mass);
}
