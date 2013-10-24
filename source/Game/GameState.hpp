#pragma once

#include "../IState.hpp"
#include "World.hpp"

#include <memory>

namespace Game { class Player; }
namespace sf { class Font; }

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
    Game::Ship* mCurrentShip;

    unsigned int mCurrentPlayer;
    std::vector<Game::Player*> mPlayers;

    std::shared_ptr<sf::Font> mFont;

    std::string mLoadState;
    Game::World mWorld;
};
