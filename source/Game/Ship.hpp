#pragma once

class b2Body;

namespace sf { class RenderTarget; }

namespace Game
{

class Player;

class Ship
{
public:
    Ship();
    ~Ship();

    void draw(sf::RenderTarget& target);

private:
    Player* mPlayer;
    b2Body* mBody;
};

}
