#include "GameState.hpp"

GameState::GameState()
{

}
GameState::~GameState()
{

}

void GameState::load()
{
	mWorld.init();
}
void GameState::unload()
{

}

bool GameState::event(const sf::Event& ev)
{
	return false;
}
void GameState::update(float dt)
{
	mWorld.update(dt);
}
void GameState::draw(sf::RenderTarget& target)
{

}
void GameState::drawUi(sf::RenderTarget& target)
{

}