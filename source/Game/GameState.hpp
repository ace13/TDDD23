#pragma once

#include "../IState.hpp"
#include "World.hpp"

class GameState : public IState
{
public:
    GameState();
    ~GameState();

    bool load();
    void unload();
    std::string getLoadState() const { return mLoadState; }

    bool event(const sf::Event& ev);
    void update(float dt);
    void draw(sf::RenderTarget& target);
    void drawUi(sf::RenderTarget& target);

private:
    void sanitizeCamera();

    bool mMoving;
    bool mMouseDrag;
    sf::Vector2f mLastMouse;

    std::string mLoadState;
    Game::World mWorld;
};
