#pragma once

#include "../IState.hpp"
#include "Player.hpp"
#include <memory>

namespace sf { class Font; }

class WinState : public IState
{
public:
    WinState(const Game::Player& winner);
    ~WinState();

    bool load();
    void unload();
    std::string getLoadState() const { return ""; }

    bool event(const sf::Event& ev);
    inline void update(float dt) { }
    inline void draw(sf::RenderTarget& target) { }
    void drawUi(sf::RenderTarget& target);

private:
    Game::Player mWinner;
    std::shared_ptr<sf::Font> mFont;
};
