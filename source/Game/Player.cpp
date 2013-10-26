#include "Player.hpp"

#include <algorithm>
#include <fstream>

using namespace Game;

Player::Player(const std::string& name, const sf::Color& color) :
    mTotalKills(0), mTotalDeaths(0), mTotalAssists(0), mDirty(false), mName(name), mColor(color)
{
}
Player::~Player()
{

}

void Player::addKill(Player* p)
{
    mTotalKills++;
    mCurrentKills.push_back(p);

    mDirty = true;
}
void Player::addAssists(Player* p)
{
    mTotalAssists++;
    mCurrentAssists.push_back(p);

    mDirty = true;
}
void Player::lostShip(Ship* s)
{
    mTotalDeaths++;
    auto it = std::find(mCurrentShips.begin(), mCurrentShips.end(), s);
    if (it != mCurrentShips.end())
        mCurrentShips.erase(it);

    mDirty = true;
}
void Player::addShip(Ship* s)
{
    auto it = std::find(mCurrentShips.begin(), mCurrentShips.end(), s);
    if (it == mCurrentShips.end())
        mCurrentShips.push_back(s);
}

void Player::finishTurn()
{
    if (mCurrentShips.size() > 1)
    {
        Ship* tmp = mCurrentShips.front();
        mCurrentShips.pop_front();
        mCurrentShips.push_back(tmp);
    }
}

void Player::synchWithFile()
{
    std::fstream fs;
    fs.open("players.data", std::ios::in | std::ios::out | std::ios::binary);

    if (!fs.is_open())
        return;
    
    fs.seekg(0, std::ios_base::end);
    int len = fs.tellg();
    fs.seekg(0);

    if (len <= 0)
    {
        fs << 1 << '\n'; // Player count
        fs << mName << '\n' << mColor.r << '\n' << mColor.g << '\n' << mColor.b << '\n' << mTotalKills << '\n' << mTotalDeaths << '\n' << mTotalAssists << '\n';

        mDirty = false;
    }
    else
    {
        int pCount;
        fs >> pCount;

        bool found = false;
        for (int i = 0; i < pCount; ++i)
        {
            std::string name;
            sf::Color color;
            int kills, deaths, assists;

            size_t start = fs.tellg();

            fs >> name >> color.r >> color.g >> color.b >> kills >> deaths >> assists;

            if (name == mName)
            {
                found = true;

                if (mDirty)
                {
                    fs.seekp(start);

                    fs << mName << '\n' << mColor.r << '\n' << mColor.g << '\n' << mColor.b << '\n' << mTotalKills << '\n' << mTotalDeaths << '\n' << mTotalAssists << '\n';

                    mDirty = false;
                }
                else
                {
                    mColor = color;
                    mTotalKills = kills;
                    mTotalDeaths = deaths;
                    mTotalAssists = assists;
                }

                break;
            }
        }

        if (!found)
        {
            fs.seekg(0);

            int curCount;

            fs >> curCount;

            fs.seekp(0);

            fs << curCount+1 << '\n';

            fs.seekp(0, std::ios_base::end);

            fs << mName << '\n' << mColor.r << '\n' << mColor.g << '\n' << mColor.b << '\n' << mTotalKills << '\n' << mTotalDeaths << '\n' << mTotalAssists << '\n';

            mDirty = false;
        }
    }
}