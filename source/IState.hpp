#pragma once

#include "StateManager.hpp"

namespace sf 
{
    class Event;
    class RenderTarget;
}

class IState
{
public:
    IState() : mStateMan(nullptr), mDestroyed(false), mHasLoaded(false) { }
    virtual ~IState() { }
    virtual bool load() = 0;
    virtual void unload() = 0;
    virtual std::string getLoadState() const = 0;

    virtual bool event(const sf::Event& ev) = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderTarget& target) = 0;
    virtual void drawUi(sf::RenderTarget& target) = 0;

    inline bool isDestroyed() const { return mDestroyed; }
    inline bool hasLoaded() const { return mHasLoaded; }

protected:
    inline void setDestroyed() { mDestroyed = true; }
    inline StateManager& getManager() const { return *mStateMan; }
    inline Application& getApplication() const { return mStateMan->mApp; }

private:
    inline void loadStage() { bool l = load(); if (l) mHasLoaded = true; }
    inline void unloadStage() { mHasLoaded = false; unload(); }

    friend class StateManager;
    StateManager* mStateMan;
    bool mDestroyed;
    bool mHasLoaded;
};
