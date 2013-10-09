#pragma once

#include "../IState.hpp"
#include "World.hpp"

class GameState : public IState
{
public:
	GameState();
	~GameState();

	void load();
    void unload();

    bool event(const sf::Event& ev);
    void update(float dt);
    void draw(sf::RenderTarget& target);
    void drawUi(sf::RenderTarget& target);

private:
	World mWorld;
};
