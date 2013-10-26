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
    Player(const std::string& name, const sf::Color& color);
    ~Player();

    void synchWithFile();

    inline std::string getName() const { return mName; }
    inline sf::Color getColor() const { return mColor; }

    void addKill(Player* p);
    void addAssists(Player* p);
    void addShip(Ship* s);
    void lostShip(Ship* s);

    void finishTurn();

    inline std::list<Ship*> getShips() const { return mCurrentShips; }

    inline unsigned int getKills() const { return mCurrentKills.size(); }

private:
    std::list<Player*> mCurrentKills;
    std::list<Player*> mCurrentAssists;
    std::list<Ship*>   mCurrentShips;

    unsigned int mTotalKills;
    unsigned int mTotalAssists;
    unsigned int mTotalDeaths;

    bool        mDirty;
    std::string mName;
    sf::Color   mColor;
};

}
