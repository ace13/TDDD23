#include "Telemetry.hpp"
#include <cassert>

Telemetry::Telemetry(): mMaxSamples(10)
{
    mFrameTimes.resize(mMaxSamples, 0.f);
}

Telemetry::~Telemetry()
{
}

void Telemetry::startFrame()
{
    mClock.restart();
}

void Telemetry::endFrame()
{
    float time = mClock.getElapsedTime().asSeconds();

    mFrameTimes.push_back(time);
    while (mFrameTimes.size() > mMaxSamples)
        mFrameTimes.pop_front();
}

void Telemetry::setMaxSamples(int samples)
{
    assert(samples > 0);

    mMaxSamples = samples;
    mFrameTimes.resize(mMaxSamples, 0.f);
}

inline void getTime(std::deque<float> times, int samples, float& time)
{
    int startS = samples;
    auto it = times.rbegin();
    for (;samples --> 0;)
    {
        time += *it++;
    }
    time /= startS;
}

float Telemetry::getFPS(int samples)
{
    assert(samples > 0 && samples <= mMaxSamples);

    float time = 0;
    getTime(mFrameTimes, samples, time);

    return 1.f/time;
}

float Telemetry::getMS(int samples)
{
    assert(samples > 0 && samples <= mMaxSamples);

    float ms = 0;
    getTime(mFrameTimes, samples, ms);

    return ms*1000.f;
}