#include "GameState.hpp"
#include "Planet.hpp"

#include <unordered_map>
#include <functional>
#include <string>

GameState::GameState()
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
        loadStates[""] = [&]() {
            mWorld.init();
            mLoadState = "Settling planets";
        };
        loadStates["Settling planets"] = [&]() {
            static int totalPlanets = 0;
            
            if (totalPlanets++ > 100)
                mLoadState = "Adding retards";
            else
            {
                Game::Planet p;

                mWorld.addPlanet(p);
            }
        };
        loadStates["Adding retards"] = [&]() {
            static int totalRetards = 0;

            if (totalRetards++ > 100)
                mLoadState = "Finalizing";
        };
        loadStates["Finalizing"] = [&]() { 
            static int finalizeWait = 0;

            if (finalizeWait++ > 100)
                mLoadState = "Doing absolutely nothing";
        };
        loadStates["Doing absolutely nothing"] = [&]() { 
            static int noWait = 0;

            if (noWait++ > 100)
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
    return false;
}
void GameState::update(float dt)
{
    mWorld.update(dt);
}
void GameState::draw(sf::RenderTarget& target)
{

}
void GameState::drawUi(sf::RenderTarget& target)
{

}