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

	bool event(const sf::Event& ev);
	void update(float dt);
	void draw(sf::RenderTarget& target);
	void drawUi(sf::RenderTarget& target);

private:
	Application& mApp;

	bool mStateDirty;
	std::list<IState*> mStates;
	std::list<IState*> mDrawQueue;
};
