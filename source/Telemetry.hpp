#pragma once

#include <SFML/System/Clock.hpp>
#include <deque>

class Telemetry
{
public:
    Telemetry();
    ~Telemetry();

    void startFrame();
    void endFrame();

    void setMaxSamples(int s);

    float getFPS(int samples = 1);
    float getMS(int samples = 1);

private:
    sf::Clock mClock;
    int mMaxSamples;
    std::deque<float> mFrameTimes;
};
