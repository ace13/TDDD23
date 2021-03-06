#include "StateManager.hpp"
#include "Application.hpp"
#include "IState.hpp"
#include "Config.hpp"

#include "Debug/TelemetryOverlay.hpp"
#include "Debug/Console.hpp"
#include "Util/LoadingScreen.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>

StateManager::StateManager(Application& app): 
    mApp(app), mShowDebug(false), mStateDirty(false), mLoad(new LoadingScreen()), mConsole(new Console())
{
    mLoad->mStateMan = this;
    mLoad->load();
    mConsole->mStateMan = this;
    mConsole->load();

    pushState(new TelemetryOverlay());
    mDrawQueue.back()->load();
    mDrawQueue.pop_back(); /* Don't want it in the draw queue,
                            * it's going to be manually taken care of */
}

StateManager::~StateManager()
{
    delete mLoad;
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
    if (ev.type == sf::Event::KeyReleased && ev.key.code == sf::Keyboard::F3)
        mShowDebug = !mShowDebug;

    if (mShowDebug && mConsole->event(ev))
        return true;

    FOR_EACH (IState* state, mDrawQueue)
    {
        bool s = state->event(ev);
        if (s)
            return true;

        if (state->isDestroyed())
            goto end;
    }

    end:

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

    mLoad->update(dt);

    FOR_EACH (IState* state, mDrawQueue)
    {
        if (state->hasLoaded())
        {
            mLoad->unload(); ///\TODO Cache this.
            state->update(dt);

            if (state->isDestroyed())
                goto end; ///\FIXME Stupid hack
        }
        else
        {
            state->loadStage();
            mLoad->setLoadingText(state->getLoadState());
        }
    }

    end:

    mStates.front()->update(dt); // Update the Telemetry Overlay
    if (mShowDebug)
        mConsole->update(dt);
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
            mLoad->drawUi(target);
        }
    }

    if (mShowDebug)
    {
        mConsole->drawUi(target);
        mStates.front()->drawUi(target); // Draw the Telemetry Overlay
    }
}
