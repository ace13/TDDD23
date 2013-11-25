#include "GameState.hpp"
#include "Planet.hpp"
#include "Player.hpp"
#include "Ship.hpp"
#include "Weapon.hpp"
#include "../Application.hpp"
#include "WinState.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

#include <unordered_map>
#include <functional>
#include <random>
#include <string>

GameState::GameState() : mMouseDrag(false), mMoving(false), mCurrentPlayer(0), mLoadState("Creating the universe")
{
    mFont = std::make_shared<sf::Font>();
    mFont->loadFromFile("Dosis-Book.ttf");
}
GameState::~GameState()
{
    FOR_EACH (auto& p, mPlayers)
        p->synchWithFile();
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
                mWorld.setSize(sf::Vector2f(2048, 2048));
                break;

            case 1:
                mWorld.updateWalls();
                break;

            case 2:
                mPlayers.push_back(new Game::Player("Player 1", sf::Color::Green));
                mPlayers.push_back(new Game::Player("Player 2", sf::Color::Red));

                FOR_EACH (auto& p, mPlayers)
                    p->synchWithFile();
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

            if (totalRetards++ > 3)
                mLoadState = "Giving them guns";
            else
            {
                Game::Ship s;
                
                if (totalRetards > 2)
                    s.setPlayer(mPlayers[0]);
                else
                    s.setPlayer(mPlayers[1]);

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
    {
        std::random_device dev;
        std::uniform_int_distribution<int> distP(0, mPlayers.size() - 1);
        mCurrentPlayer = distP(dev);

        sanitizeCamera();
        return true;
    }

    loadStates[mLoadState]();
    return false;
}
void GameState::unload()
{

}

bool GameState::event(const sf::Event& ev)
{
    if (!mMoving)
    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left)
    {
        mMouseDrag = true;
        mLastMouse = getApplication().getMouse();
    }
    else if (ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Left)
        mMouseDrag = false;
    else if (ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Right)
    {
        sf::Vector2f mPos = getApplication().getMouse();
        auto ships = mPlayers[mCurrentPlayer]->getShips();
        
        {
            auto& s = *ships.front();

            auto pos = s.getPosition();

            float force = std::min((float)sqrt(((pos.x - mPos.x)*(pos.x - mPos.x)) + ((pos.y - mPos.y)*(pos.y - mPos.y))), 100.f);

            Game::Weapon fired(&s, atan2(mPos.y - pos.y, mPos.x - pos.x), force);
            s.setTurn(false);

            mWorld.addWeapon(fired);
        }
    }
    else if (ev.type == sf::Event::MouseWheelMoved)
    {
        static float zoomFactor = 5.f;
        int delta = -ev.mouseWheel.delta;

        sf::Vector2f curMouse = getApplication().getMouse();
        sf::View& gameView = getApplication().getGameView();

        sf::Vector2f diff = gameView.getCenter() - curMouse;
        if (delta < 0)
            diff = -diff;

        ///\FIXME limit zoomFactor to the available zoom factor
        float worldDot = (mWorld.getSize().x);
        float cameraDot = (getApplication().getGameView().getSize().x);
        if (!((delta > 0 && cameraDot >= worldDot) || (delta < 0 && cameraDot <= 512)))
        {
            gameView.move(diff/zoomFactor);

            gameView.zoom(1 + delta / zoomFactor);

            sanitizeCamera();
        }
    }

    return false;
}
void GameState::update(float dt)
{
    auto list = mWorld.getAlive();

    if (list.empty())
    {
        if (mMoving)
        {
            mPlayers[mCurrentPlayer]->finishTurn();

            int lastPlayer = mCurrentPlayer;
            do
            {
                mCurrentPlayer = (mCurrentPlayer+1) % mPlayers.size();
            } while(mPlayers[mCurrentPlayer]->getShips().empty() && mCurrentPlayer != lastPlayer);

            unsigned int hasLiving = 0;

            FOR_EACH (auto p, mPlayers)
            {
                if (!p->getShips().empty())
                    hasLiving++;
            }
            
            if (hasLiving <= 1)
            {
                FOR_EACH(auto p, mPlayers)
                {
                    if (p->getShips().empty())
                        continue;

                    getApplication().getLogger().log("%s just won the game with %i kills!", Logger::Info, p->getName().c_str(), p->getKills());

                    getManager().popState();
                    getManager().pushState(new WinState(*p));
                    return;
                }
            }

            mCurrentShip = mPlayers[mCurrentPlayer]->getShips().front();
            mCurrentShip->setTurn();

            getApplication().getGameView().setCenter(mCurrentShip->getPosition());
        }

        mMoving = false;
        if (mMouseDrag)
        {
            sf::Vector2f curMouse = getApplication().getMouse();

            sf::Vector2f diff = mLastMouse - curMouse;
            curMouse += diff;
        
            getApplication().getGameView().move(diff);
            mLastMouse = curMouse;
        }
    }
    else
    {
        mMoving = true;
        sf::Vector2f min(mWorld.getSize() * 1.1f), max(mWorld.getSize() * -1.1f);

        sf::Vector2f center;
        FOR_EACH (auto e, list)
        {
            auto pos = e->getPosition();
            center += pos;
            float rad = e->getRadius() * 5.f;

            min.x = std::min(min.x, pos.x - rad);
            min.y = std::min(min.y, pos.y - rad);
            max.x = std::max(max.x, pos.x + rad);
            max.y = std::max(max.y, pos.y + rad);
        }

        center /= (float)list.size();
        
        sf::Vector2f target = max - min;
        sf::Vector2f current = getApplication().getGameView().getSize();

        if (current.x < current.y)
        {
            getApplication().getGameView().zoom((target.x / current.x));
            current = getApplication().getGameView().getSize();
            if (current.y < target.y)
                getApplication().getGameView().zoom(target.y / current.y);
        }
        else
        {
            getApplication().getGameView().zoom((target.y / current.y));
            current = getApplication().getGameView().getSize();
            if (current.x < target.x)
                getApplication().getGameView().zoom(target.x / current.x);
        }


        sf::Vector2f curPos = getApplication().getGameView().getCenter();
        getApplication().getGameView().move((center - curPos) / 4.f);
    }

    sanitizeCamera();

    mWorld.update(dt);
}
void GameState::draw(sf::RenderTarget& target)
{
    mWorld.draw(target);

    if (mWorld.getAlive().empty() && !mPlayers[mCurrentPlayer]->getShips().empty())
    {
        auto mPos = getApplication().getMouse();
        auto& s = *mPlayers[mCurrentPlayer]->getShips().front();

        auto pos = s.getPosition();

        float len = std::min((float)sqrt(((pos.x - mPos.x)*(pos.x - mPos.x)) + ((pos.y - mPos.y)*(pos.y - mPos.y))), 100.f);
        float ang = atan2(mPos.y - pos.y, mPos.x - pos.x);

        sf::ConvexShape shape;
        shape.setPointCount(7);

        shape.setPoint(0, sf::Vector2f(len, 0));
        shape.setPoint(1, sf::Vector2f(len - 10, 10));
        shape.setPoint(2, sf::Vector2f(len - 10, 2.5f));
        shape.setPoint(3, sf::Vector2f(10, 2.5f));
        shape.setPoint(4, sf::Vector2f(10, -2.5f));
        shape.setPoint(5, sf::Vector2f(len - 10, -2.5f));
        shape.setPoint(6, sf::Vector2f(len - 10, -10));

        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1.f);
        shape.setFillColor(sf::Color::Transparent);
        shape.setPosition(pos);
        shape.setRotation(ang * (180/M_PI));

        target.draw(shape);
    }
}
void GameState::drawUi(sf::RenderTarget& target)
{
    mWorld.drawUi(target);

    sf::Text text(mPlayers[mCurrentPlayer]->getName(), *mFont);
    auto size = text.getLocalBounds();

    text.setPosition(target.getSize().x / 2.f - size.width / 2.f, 0);

    target.draw(text);
}

void GameState::sanitizeCamera()
{
    sf::Vector2f worldSize = mWorld.getSize();
    sf::Vector2f viewSize = getApplication().getGameView().getSize();

    {
        sf::Vector2f halfWorld = worldSize / 2.f;
        sf::Vector2f halfView;// = viewSize / 2.f;
        sf::Vector2f curPos = getApplication().getGameView().getCenter();
        curPos.x = std::max(-halfWorld.x + halfView.x, std::min(curPos.x, halfWorld.x - halfView.x));
        curPos.y = std::max(-halfWorld.y + halfView.y, std::min(curPos.y, halfWorld.y - halfView.y));
        getApplication().getGameView().setCenter(curPos);
    }

    {
        static const float smallest = 512;
        float worldDot;
        float cameraDot;

        if (viewSize.x < viewSize.y)
        {
            worldDot = (worldSize.y);
            cameraDot = (viewSize.y);
        }
        else
        {
            worldDot = (worldSize.x);
            cameraDot = (viewSize.x);
        }

        if (cameraDot > worldDot)
            getApplication().getGameView().zoom(worldDot/cameraDot);
        else if (cameraDot < smallest)
            getApplication().getGameView().zoom(smallest/cameraDot);
    }
}