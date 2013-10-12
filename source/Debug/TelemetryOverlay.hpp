#pragma once

#include "../IState.hpp"
#include <memory>

namespace sf { class Font; }

class TelemetryOverlay : public IState
{
public:
    TelemetryOverlay();
    ~TelemetryOverlay();

    bool load();
    void unload();
    std::string getLoadState() const { return ""; }

    inline bool event(const sf::Event& ev) { return true; }
    void update(float dt);
    inline void draw(sf::RenderTarget& target) { }
    void drawUi(sf::RenderTarget& target);

private:
    std::shared_ptr<sf::Font> mFont;
};
