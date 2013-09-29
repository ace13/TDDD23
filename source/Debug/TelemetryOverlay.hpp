#pragma once

#include "../IState.hpp"
#include <memory>

namespace sf { class Font; }

class TelemetryOverlay : public IState
{
public:
    TelemetryOverlay();
    ~TelemetryOverlay();

    void load();
    void unload();

    inline bool event(const sf::Event& ev) { }
    void update(float dt);
    inline void draw(sf::RenderTarget& target) { }
    void drawUi(sf::RenderTarget& target);

private:
    std::shared_ptr<sf::Font> mFont;
};
