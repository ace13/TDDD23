#pragma once

#include "../IState.hpp"
#include <memory>
#include <list>

namespace sf { class Font; }

class Console : public IState
{
public:
    Console();
    ~Console();

    bool load();
    void unload();
    std::string getLoadState() const;

    bool event(const sf::Event& ev);
    void update(float dt);
    inline void draw(sf::RenderTarget& target) { }
    void drawUi(sf::RenderTarget& target);

private:
    std::shared_ptr<sf::Font> mFont;
    std::list<std::string> mOutput;

    std::list<std::string> mOldLines;
    std::string mCurrentLine;
};
