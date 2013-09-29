#pragma once

#include <list>

namespace sf {
    class Event;
    class RenderTarget;
}

class Application;
class IState;

class StateManager
{
public:
    StateManager(Application&);
    ~StateManager();

    void pushState(IState* s);
    void popState();

    bool doEvent(const sf::Event& ev);
    void doUpdate(float dt);
    void doDraw(sf::RenderTarget& target);
    void doDrawUi(sf::RenderTarget& target);

private:
    friend class IState;
    Application& mApp;

    bool mShowDebug;
    bool mStateDirty;
    std::list<IState*> mStates;
    std::list<IState*> mDrawQueue;
};
