#include "GameState.hpp"
#include "Planet.hpp"
#include "Ship.hpp"
#include "../Application.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <unordered_map>
#include <functional>
#include <string>

GameState::GameState() : mMouseDrag(false), mLoadState("Creating the universe")
{

}
GameState::~GameState()
{

}

bool GameState::load()
{
    static std::unordered_map<std::string, std::function<void()> > loadStates;
    if (loadStates.empty())
    {
        loadStates["Creating the universe"] = [&]() {
            static int stage = 0;
            switch (stage++)
            {
            case 0:
                mWorld.init();
                mWorld.setSize(sf::Vector2f(1600, 1000));
                break;

            case 1:
                break;

            default:
                mLoadState = "Placing planets";
            }
        };
        loadStates["Placing planets"] = [&]() {
            static int totalPlanets = 0;
            
            if (totalPlanets++ > 9)
                mLoadState = "Adding retards";
            else
            {
                Game::Planet p;

                mWorld.addPlanet(p);
            }
        };
        loadStates["Adding retards"] = [&]() {
            static int totalRetards = 0;

            if (totalRetards++ > 0)
                mLoadState = "Giving them guns";
            else
            {
                Game::Ship s;

                mWorld.addShip(s);
            }
        };
        loadStates["Giving them guns"] = [&]() {
            static int scripts = 0;

            if (scripts++ > 0)
                mLoadState = "Finalizing";
            else
            {
                ///\TODO Load weapons from scripts.
            }
        };
        loadStates["Finalizing"] = [&]() { 
            static int finalizeWait = 0;

            if (finalizeWait++ > 0)
                mLoadState = "Reticulating splines";
        };
        loadStates["Reticulating splines"] = [&]() { 
            static int noWait = 0;

            if (noWait++ > 0)
                mLoadState = "Done";
        };
    }

    if (loadStates.count(mLoadState) == 0)
        return true;

    loadStates[mLoadState]();
    return false;
}
void GameState::unload()
{

}

bool GameState::event(const sf::Event& ev)
{
    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
    {
        mMouseDrag = true;
        mLastMouse = getApplication().getMouse();
    }
    else if (ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Left)
        mMouseDrag = false;

    return false;
}
void GameState::update(float dt)
{
    if (mMouseDrag)
    {
        sf::Vector2f curMouse = getApplication().getMouse();

        sf::Vector2f diff = mLastMouse - curMouse;
        curMouse += diff;
        
        getApplication().getGameView().move(diff);
        mLastMouse = curMouse;
    }

    mWorld.update(dt);
}
void GameState::draw(sf::RenderTarget& target)
{
    mWorld.draw(target);
}
void GameState::drawUi(sf::RenderTarget& target)
{

}