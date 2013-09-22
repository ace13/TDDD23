#pragma once

#include <SFML/System/Clock.hpp>
#include <deque>

/** Class for handling basic application telemetry
 *
 * \TODO Should handle more than simply FPS/Frame time,
 * maybe profiling data too?
 */
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
    int mCurrentFrames;
    float mSecond;
    std::deque<int> mFrames;
    std::deque<float> mFrameTimes;
};
