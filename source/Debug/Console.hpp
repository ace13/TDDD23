#pragma once

#include "../IState.hpp"
#include <memory>

namespace sf { class Font; }

class Console : public IState
{
public:
    Console();
    ~Console();

    void load();
    void unload();

    bool event(const sf::Event& ev);
    void update(float dt);
    inline void draw(sf::RenderTarget& target) { }
    void drawUi(sf::RenderTarget& target);

private:
    std::shared_ptr<sf::Font> mFont;
};
