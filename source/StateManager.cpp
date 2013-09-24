#include "StateManager.hpp"
#include "Application.hpp"

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

void StateManager::setState()
{

}

bool StateManager::event(const sf::Event& ev)
{

}

void StateManager::update(float dt)
{

}

void StateManager::draw(sf::RenderTarget& target)
{

}

void StateManager::drawUi(sf::RenderTarget& target)
{
#ifndef NDEBUG
	char tmp[256];

	auto& ref = mApp.mTelem;
	sprintf(tmp, "FPS (1,5,10):\n%f\n%f\n%f\nUPS: %d", ref.getFPS(), ref.getFPS(5), ref.getFPS(10), ref.getUPS());

	Debug.setString(tmp);
	target.draw(Debug);
#endif
}
