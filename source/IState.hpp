#pragma once

namespace sf 
{
	class Event;
	class RenderTarget;
}

class StateManager;

class IState
{
public:
	IState() : mDestroyed(false) { }
	virtual ~IState() { }
	virtual void load() = 0;
	virtual void unload() = 0;

	virtual bool event(const sf::Event& ev) = 0;
	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderTarget& target) = 0;
	virtual void drawUi(sf::RenderTarget& target) = 0;

	inline bool isDestroyed() { return mDestroyed; }

protected:
	inline void setDestroyed() { mDestroyed = true; }

private:
	friend class StateManager;
	bool mDestroyed;
};
