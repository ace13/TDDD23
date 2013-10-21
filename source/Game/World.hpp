#pragma once

#include "Entity.hpp"
#include "../Config.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <list>

namespace sf { class RenderTexture; }

class b2ContactListener;
class b2Draw;
class b2Body;
class b2World;

namespace Game
{

class Planet;
class Ship;
class Weapon;

class World
{
public:
    World();
    ~World();

    void init();
    void update(float dt);
    void updateWalls();

    void addPlanet(Planet& planet);
    void addShip(Ship& ship);
    void addWeapon(Weapon& weap);

    void addExplosion(const sf::Vector2f& pos, float radius, bool damageTerrain = true);

    void draw(sf::RenderTarget& target);
    void drawUi(sf::RenderTarget& target);

    inline sf::Vector2f getSize() const { return mSize; }
    inline void setSize(sf::Vector2f size) { mSize = size; updateWalls(); }

    inline b2World* getBox2D() { return mBox2DWorld; }

    inline std::list<Weapon>& getWeapons() { return mWeapons; }
    inline std::list<Planet>& getPlanets() { return mPlanets; }
    inline std::list<Ship>& getShips() { return mShips; }

private:
    World(const World&);
    World& operator=(const World& other);

    sf::Vector2f mSize;

    std::list<Weapon> mWeapons;
    std::list<Planet> mPlanets;
    std::list<Ship> mShips;
    sf::FloatRect mCameraRect;

    b2Body* mWalls[4];
    b2World* mBox2DWorld;
    b2ContactListener* mListener;
#ifdef DEBUG
    b2Draw* mDebugDraw;
    sf::RenderTexture* mDebugTarget;
#endif
};

}
