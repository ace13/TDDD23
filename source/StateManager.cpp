#include "StateManager.hpp"
#include "Application.hpp"
#include "IState.hpp"

#include "Debug/TelemetryOverlay.hpp"
#include <SFML/Window/Event.hpp>

StateManager::StateManager(Application& app): 
    mApp(app), mShowDebug(false), mStateDirty(false)
{
#ifndef NDEBUG
    mShowDebug = true;
#endif

    pushState(new TelemetryOverlay());
    mDrawQueue.pop_back(); /* Don't want it in the draw queue,
                            * it's going to be manually taken care of */
}

StateManager::~StateManager()
{

}

void StateManager::pushState(IState* state)
{
    auto it = mStates.end();
    if ((it = std::find(mStates.begin(), mStates.end(), state)) != mStates.end())
        return;

    state->load();
    mStates.push_back(state);
    mDrawQueue.push_back(state);
    state->mStateMan = this;
}

void StateManager::popState()
{
    IState* state = mDrawQueue.back();
    mDrawQueue.pop_back();

    state->setDestroyed();
    mStateDirty = true;
}

bool StateManager::doEvent(const sf::Event& ev)
{
    for (auto it = mDrawQueue.begin(); it != mDrawQueue.end(); ++it)
    {
        bool s = (*it)->event(ev);
        if (s)
            return true;
    }

    if (ev.type == sf::Event::KeyReleased && ev.key.code == sf::Keyboard::F3)
        mShowDebug = !mShowDebug;

    return false;
}

void StateManager::doUpdate(float dt)
{
    if (mStateDirty)
    {
        mStateDirty = false;
        for (auto it = mStates.begin(); it != mStates.end();)
        {
            if ((*it)->isDestroyed())
            {
                for (auto it2 = mDrawQueue.begin(); it2 != mDrawQueue.end(); ++it2)
                    if (*it2 == *it)
                    {
                        mDrawQueue.erase(it2);
                        break;
                    }

                (*it)->unload();
                delete *it;
                it = mStates.erase(it);
            }
            else
                ++it;
        }
    }

    for (auto it = mDrawQueue.begin(); it != mDrawQueue.end(); ++it)
        (*it)->update(dt);

    mStates.front()->update(dt); // Update the Telemetry Overlay
}

void StateManager::doDraw(sf::RenderTarget& target)
{
    for (auto it = mDrawQueue.begin(); it != mDrawQueue.end(); ++it)
        (*it)->draw(target);
}

void StateManager::doDrawUi(sf::RenderTarget& target)
{
    for (auto it = mDrawQueue.begin(); it != mDrawQueue.end(); ++it)
        (*it)->drawUi(target);

    if (mShowDebug)
        mStates.front()->drawUi(target); // Draw the Telemetry Overlay
}
