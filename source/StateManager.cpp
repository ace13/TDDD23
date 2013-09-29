#include "StateManager.hpp"
#include "Application.hpp"
#include "IState.hpp"

#ifndef NDEBUG
#include <SFML/Graphics/Text.hpp>

namespace
{
    sf::Text Debug;
    sf::Font DebugFont;
    bool DebugLoaded = false;
}
#endif


StateManager::StateManager(Application& app): mApp(app)
{
#ifndef NDEBUG
    if (!DebugLoaded)
    {
        DebugFont.loadFromFile("Dosis-Book.ttf");
        Debug.setFont(DebugFont);
        Debug.setCharacterSize(12);
        DebugLoaded = true;
    }
#endif

}

StateManager::~StateManager()
{

}

void StateManager::pushState(IState* state)
{
    auto it = mStates.end();
    if ((it = std::find(mStates.begin(), mStates.end(), state)) == mStates.end())
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
    for (auto it = mDrawQueue.begin(); it != mDrawQueue.end(); ++it)
    {
        bool s = (*it)->event(ev);
        if (s)
            return true;
    }

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

                delete *it;
                it = mStates.erase(it);
            }
            else
                ++it;
        }
    }

    for (auto it = mDrawQueue.begin(); it != mDrawQueue.end(); ++it)
        (*it)->update(dt);
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

#ifndef NDEBUG
    char tmp[256];

    auto& ref = mApp.mTelem;
    sprintf(tmp, "FPS (1,5,10):\n%f\n%f\n%f\nUPS: %d", ref.getFPS(), ref.getFPS(5), ref.getFPS(10), ref.getUPS());

    Debug.setString(tmp);
    target.draw(Debug);
#endif
}
