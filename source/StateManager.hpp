#pragma once

namespace sf {
	class Event;
	class RenderTarget;
}

class Application;

class StateManager
{
public:
	StateManager(Application&);
	~StateManager();

	void setState();

	bool event(const sf::Event& ev);
	void update(float dt);
	void draw(sf::RenderTarget& target);
	void drawUi(sf::RenderTarget& target);

private:
	Application& mApp;
};
