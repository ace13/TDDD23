#include "World.hpp"
#include "Planet.hpp"
#include "Ship.hpp"

#include <Box2D/Box2D.h>
#include <random>

#ifdef DEBUG
#include "../Debug/DebugDraw.hpp"
#endif

using namespace Game;

World::World() : mBox2DWorld(nullptr)
#ifdef DEBUG
    , mDebugDraw(nullptr), mDebugTarget(nullptr)
#endif
{
}

World::~World()
{
    mPlanets.clear();
    mShips.clear();

    if (mBox2DWorld)
        delete mBox2DWorld;
#ifdef DEBUG
    if (mDebugDraw)
        delete mDebugDraw;
    if (mDebugTarget)
        delete mDebugTarget;
#endif
}

void World::init()
{
    if (mBox2DWorld)
        return;

    mBox2DWorld = new b2World(b2Vec2(0,0));
}

void World::update(float dt)
{
    if (!mBox2DWorld)
        return;

    static int velocitySteps = 10;
    static int positionSteps = 5;

    mBox2DWorld->Step(dt, velocitySteps, positionSteps); ///\TODO Maybe check if this needs to be more or less static anywho

    FOR_EACH (auto& p, mPlanets)
    {
        if (p.getDirty())
        {
            p.resetDirty();

            ///\TODO Calculate if planet needs to be replaced.
        }
    }

    auto dist = [](const sf::Vector2f& a, const sf::Vector2f& b) -> float {
        return ((b.x-a.x)*(b.x-a.x))+((b.y-a.y)*(b.y-a.y));
    };

    FOR_EACH (auto& s, mShips)
    {
        FOR_EACH (auto& p, mPlanets)
        {
            float distance = sqrt(dist(s.getPosition(), p.getPosition()));
            if (distance < (p.getRadius() * 4))
                s.addGravity(p.getPosition(), 1-(distance / (p.getRadius() * 4)));
        }
    }
}

///\FIXME Don't draw everything always.
void World::draw(sf::RenderTarget& target)
{
#ifdef DEBUG
    auto& view = target.getView();
    if (!mDebugTarget)
    {
        float aspect = view.getSize().y / view.getSize().x;

        mDebugTarget = new sf::RenderTexture();
        mDebugTarget->create(512 * aspect, 512);
        
    }
    mDebugTarget->setSmooth(true);

    if (!mDebugDraw)
    {
        uint32 flags = b2Draw::e_shapeBit; //(b2Draw::e_aabbBit | b2Draw::e_centerOfMassBit | b2Draw::e_jointBit | b2Draw::e_pairBit | b2Draw::e_shapeBit);

        mDebugDraw = new DebugDraw(*mDebugTarget);
        mDebugDraw->SetFlags(flags);
        mBox2DWorld->SetDebugDraw(mDebugDraw);
    }

    mDebugTarget->setView(view);
    mDebugTarget->clear();
    sf::RectangleShape world(mSize);
    world.setFillColor(sf::Color::Transparent);
    world.setOutlineColor(sf::Color::Red);
    world.setOutlineThickness(5.f);
    world.setOrigin(mSize/2.f);
    mDebugTarget->draw(world);
    mBox2DWorld->DrawDebugData();
    mDebugTarget->display();
#endif
//#else
    FOR_EACH (auto& p, mPlanets)
    {
        p.draw(target);
    }

    FOR_EACH(auto& s, mShips)
    {
        s.draw(target);
    }
//#endif
}

void World::drawUi(sf::RenderTarget& target)
{
#ifdef DEBUG
    sf::RectangleShape debugSprite;
    sf::Vector2f realSize = (sf::Vector2f)mDebugTarget->getSize();
    float aspect = realSize.y / realSize.x;
    sf::Vector2f targetSize(128 * aspect, 128);
    auto size = target.getSize();

    debugSprite.setPosition(size.x - 128 * aspect - 1, size.y - 129);
    debugSprite.setSize(targetSize);
    debugSprite.setTexture(&mDebugTarget->getTexture());
    debugSprite.setOutlineColor(sf::Color::White);
    debugSprite.setOutlineThickness(1.f);

    target.draw(debugSprite);
#endif
}

void World::addPlanet(const Planet& p)
{
    Game::Planet tmp = p;

    float radius = tmp.getRadius() * 1.5;

    std::random_device dev;
    std::uniform_real_distribution<float> distX(radius, mSize.x - radius);
    std::uniform_real_distribution<float> distY(radius, mSize.y - radius);

    tmp.setPosition(sf::Vector2f(distX(dev) - mSize.x / 2.f, distY(dev) - mSize.y / 2.f));
    tmp.addedToWorld(*this);

    mPlanets.push_back(tmp);
}

void World::addShip(const Ship& s)
{
    Game::Ship tmp = s;

    std::random_device dev;
    std::uniform_int_distribution<int> distP(0, mPlanets.size()-1);
    std::uniform_real_distribution<float> distR(0, 360);

    int planet = distP(dev);
    float angle = distR(dev) * (3.14159/180);

    Planet* targetPlanet = nullptr;
    unsigned int i = 0;
    FOR_EACH(auto& p, mPlanets)
        if (i++ == planet)
        {
            targetPlanet = &p;
            break;
        }

    auto planetPos = targetPlanet->getPosition();
    sf::Vector2f dir(cos(angle), sin(angle));

    tmp.setPosition(planetPos + dir * (targetPlanet->getRadius() + 5));
    tmp.setAngle(angle + (90 * (3.14159/180)));
    tmp.addedToWorld(*this);

    mShips.push_back(tmp);
}