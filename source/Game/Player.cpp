#include "Player.hpp"

#include <fstream>

using namespace Game;

Player::Player(const std::string& name, const sf::Color& color) :
    mTotalKills(0), mTotalDeaths(0), mTotalAssists(0), mName(name), mColor(color)
{
}
Player::~Player()
{
}

void Player::synchWithFile()
{
    std::fstream fs;
    fs.open("players.data");

    
}