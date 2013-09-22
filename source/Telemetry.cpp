#include "Telemetry.hpp"
#include <cassert>

Telemetry::Telemetry(): mMaxSamples(10), mSecond(0)
{
    mFrameTimes.resize(mMaxSamples, 0.f);
    mFrames.resize(mMaxSamples, 0);
}

Telemetry::~Telemetry()
{
}

void Telemetry::startFrame()
{
    mClock.restart();
    ++mCurrentFrames;
}

void Telemetry::endFrame()
{
    float time = mClock.getElapsedTime().asSeconds();
    mSecond += time;

    mFrameTimes.push_back(time);
    while (mFrameTimes.size() > mMaxSamples)
        mFrameTimes.pop_front();

    if (mSecond > 1.f)
    {
        mFrames.push_back(mCurrentFrames);
        mCurrentFrames = 0;
        mSecond -= 1.f;
    }
}

void Telemetry::setMaxSamples(int samples)
{
    assert(samples > 0);

    mMaxSamples = samples;
    mFrameTimes.resize(mMaxSamples, 0.f);
    mFrames.resize(mMaxSamples, 0);
}

template<typename T>
inline void getTime(std::deque<T> times, int samples, float& time)
{
    int startS = samples;
    auto it = times.rbegin();
    while (samples --> 0)
    {
        time += *it++;
    }
    time /= startS;
}

float Telemetry::getFPS(int samples)
{
    assert(samples > 0 && samples <= mMaxSamples);

    float time = 0;
    getTime(mFrames, samples, time);

    return time;
}

float Telemetry::getMS(int samples)
{
    assert(samples > 0 && samples <= mMaxSamples);

    float ms = 0;
    getTime(mFrameTimes, samples, ms);

    return ms*1000.f;
}
