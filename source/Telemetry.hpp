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
    void startUpdate();
    void endUpdate();

    void setMaxSamples(unsigned int s);

    int getUPS() const;
    float getFPS(unsigned int samples = 1) const;
    float getMS(unsigned int samples = 1) const;

private:
    sf::Clock mClock;
    unsigned int mMaxSamples;
    unsigned int mCurrentFrames;
    unsigned int mCurrentUpdates;
    float mSecond;
    int mUpdates;
    std::deque<unsigned int> mFrames;
    std::deque<float> mFrameTimes;
};
