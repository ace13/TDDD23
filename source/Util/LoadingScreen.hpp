#pragma once

#include "../IState.hpp"
#include <SFML/Graphics/Font.hpp>

class LoadingScreen : public IState
{
public:
    LoadingScreen();
    ~LoadingScreen();

    bool load();
    void unload();
    inline std::string getLoadState() const { return ""; }

    inline bool event(const sf::Event&) { return false; }
    void update(float dt);
    inline void draw(sf::RenderTarget&) { }
    void drawUi(sf::RenderTarget& target);

    void setLoadingText(const std::string& text);

private:
    float mTime;
    std::string mCurrentString;
    std::vector<std::string> mLastMessages;

    std::shared_ptr<sf::Font> mFont;
    std::shared_ptr<sf::Texture> mSpinner;
};
