#pragma once

#include <SFML/Graphics/Color.hpp>
#include <string>
#include <list>

namespace Game
{

class Ship;

class Player
{
public:
    Player(const std::string& name);
    ~Player();

    void synchWithFile();

    std::string getName() const;
    sf::Color getColor() const;

private:
    std::list<Player*> mKills;
    std::list<Player*> mAssists;
    std::list<Ship*> mShips;

    std::string mName;
    sf::Color mColor;
    float mScore;
};

}
