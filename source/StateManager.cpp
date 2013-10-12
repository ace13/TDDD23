#include "StateManager.hpp"
#include "Application.hpp"
#include "IState.hpp"
#include "Config.hpp"

#include "Debug/TelemetryOverlay.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>

StateManager::StateManager(Application& app): 
    mApp(app), mShowDebug(false), mStateDirty(false)
{
#ifndef NDEBUG
    mShowDebug = true;
#endif

    pushState(new TelemetryOverlay());
    mDrawQueue.back()->load();
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
    FOR_EACH (IState* state, mDrawQueue)
    {
        bool s = state->event(ev);
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
            auto state = *it;

            if (state->isDestroyed())
            {
                FOR_EACH (IState* state2, mDrawQueue)
                    if (state == state2)
                    {
                        mDrawQueue.remove(state2);
                        break;
                    }

                state->unloadStage();
                delete state;

                it = mStates.erase(it);
            }
            else
                ++it;
        }
    }

    FOR_EACH (IState* state, mDrawQueue)
    {
        if (!state->hasLoaded())
            state->loadStage();
        else
            state->update(dt);
    }

    mStates.front()->update(dt); // Update the Telemetry Overlay
}

void StateManager::doDraw(sf::RenderTarget& target)
{
    FOR_EACH (IState* state, mDrawQueue)
        if (state->hasLoaded())
            state->draw(target);
}

void StateManager::doDrawUi(sf::RenderTarget& target)
{
    FOR_EACH (IState* state, mDrawQueue)
    {
        if (state->hasLoaded())
            state->drawUi(target);
        else
        {
            target.clear(sf::Color::White);
            ///\TODO Draw loading screen
        }
    }

    if (mShowDebug)
        mStates.front()->drawUi(target); // Draw the Telemetry Overlay
}
