#include "World.hpp"
#include "Planet.hpp"
#include "Ship.hpp"
#include "Weapon.hpp"

#include <Box2D/Box2D.h>
#include <random>

#ifdef DEBUG
#include "../Debug/DebugDraw.hpp"
#endif

using namespace Game;

World::World() : mBox2DWorld(nullptr)
#ifdef DEBUG
    , mDebugDraw(nullptr), mDebugTarget(new sf::RenderTexture())
#endif
{
    for (int i = 0; i < 4; ++i)
        mWalls[i] = nullptr;

#ifdef DEBUG
    mDebugTarget->create(512, 512);
    mDebugTarget->setSmooth(true);
#endif
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
                s.addGravity(p.getPosition(), 1 - (distance / (p.getRadius() * 4 * p.getPercentage())));
        }
    }

    for (auto it = mWeapons.begin(); it != mWeapons.end();)
    {
        auto& w = *it;

        if (w.isDestroyed())
        {
            mBox2DWorld->DestroyBody(w.mBody);
            it = mWeapons.erase(it);
        }
        else
        {
            w.update(dt);

            FOR_EACH (auto& p, mPlanets)
            {
                float distance = sqrt(dist(w.getPosition(), p.getPosition()));
                if (distance < (p.getRadius() * 4))
                    w.addGravity(p.getPosition(), 1 - (distance / (p.getRadius() * 4 * p.getPercentage())));
            }

            ++it;
        }
    }
}

void World::updateWalls()
{
    if (mWalls[0] != nullptr)
        for (int i = 0; i < 4; ++i)
        {
            mBox2DWorld->DestroyBody(mWalls[i]);
        }

    auto setWall = [&](int id, const sf::Vector2f& pos, const sf::Vector2f& size) {
        {
            b2BodyDef def;
            def.type = b2_staticBody;
            def.position.Set((pos.x + size.x / 2) / 5.f, (pos.y + size.y / 2) / 5.f);
            def.angle = 0;
            def.linearVelocity = b2Vec2(0, 0);
            def.angularVelocity = 0;
            def.linearDamping = 0;
            def.angularDamping = 0;
            def.allowSleep = true;
            def.awake = true;
            def.fixedRotation = true;
            def.bullet = false;
            def.active = true;
            def.gravityScale = 0;

            mWalls[id] = mBox2DWorld->CreateBody(&def);
        }

        auto& body = *mWalls[id];
    
        {
            b2PolygonShape shape;
            shape.SetAsBox((size.x / 2) / 5.f, (size.y / 2) / 5.f);

            b2FixtureDef def;
            def.density = 1.f;
            def.isSensor = false;
            def.friction = 0.f;
            def.restitution = 1.f;
            def.shape = &shape;

            auto fix = body.CreateFixture(&def);
         }
    };

    setWall(0, sf::Vector2f(0, -1) - mSize/2.f, sf::Vector2f(mSize.x, 1));
    setWall(1, sf::Vector2f(mSize.x, 0) - mSize/2.f, sf::Vector2f(1, mSize.y));
    setWall(2, sf::Vector2f(0, mSize.y) - mSize/2.f, sf::Vector2f(mSize.x, 1));
    setWall(3, sf::Vector2f(-1, 0) - mSize/2.f, sf::Vector2f(1, mSize.y));
}

///\FIXME Don't draw everything always.
void World::draw(sf::RenderTarget& target)
{
#ifdef DEBUG
    auto view = target.getView();
    view.setSize(view.getSize().y, view.getSize().y);

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

    mCameraRect.width = target.getView().getSize().x;
    mCameraRect.height = target.getView().getSize().y;
    mCameraRect.left = target.getView().getCenter().x - mCameraRect.width  / 2.f;
    mCameraRect.top  = target.getView().getCenter().y - mCameraRect.height / 2.f;

    FOR_EACH (auto& p, mPlanets)
    {
        p.drawWell(target);
    }

    FOR_EACH (auto& p, mPlanets)
    {
        p.draw(target);
    }

    FOR_EACH (auto& s, mShips)
    {
        s.draw(target);
    }

    FOR_EACH (auto& w, mWeapons)
    {
        w.draw(target);
    }
}

void World::drawUi(sf::RenderTarget& target)
{
#ifdef DEBUG
    sf::RectangleShape debugSprite;
    sf::Vector2f targetSize(128, 128);
    auto size = target.getSize();

    debugSprite.setPosition(size.x - targetSize.x - 1, size.y - targetSize.y - 1);
    debugSprite.setSize(targetSize);
    debugSprite.setTexture(&mDebugTarget->getTexture());
    debugSprite.setOutlineColor(sf::Color::White);
    debugSprite.setOutlineThickness(1.f);

    target.draw(debugSprite);
#endif

    sf::FloatRect ship;

    sf::ConvexShape arrow(3);
    arrow.setPoint(0, sf::Vector2f( 0, -10));
    arrow.setPoint(1, sf::Vector2f( 5,  0));
    arrow.setPoint(2, sf::Vector2f(-5,  0));
    arrow.setOrigin(0, -5);

    arrow.setOutlineColor(sf::Color::White);
    arrow.setOutlineThickness(1.5f);
    arrow.setFillColor(sf::Color::Transparent);

    auto& cCent = target.getView().getCenter();
    sf::Vector2f wCent(mCameraRect.left + mCameraRect.width / 2, mCameraRect.top + mCameraRect.height / 2);
    arrow.setPosition(cCent);

    FOR_EACH(auto& s, mShips)
    {
        ship = sf::FloatRect(s.getPosition(), sf::Vector2f(10, 10));
        ship.left -= ship.width  / 2.f;
        ship.top  -= ship.height / 2.f;

        if (!mCameraRect.intersects(ship))
        {
            float dir = atan2(s.getPosition().y - wCent.y, s.getPosition().x - wCent.x);
            float distance = target.getSize().y / 2.f - 25;

            arrow.setRotation(dir * (180/M_PI) + 90);
            arrow.move(cos(dir) * distance, sin(dir) * distance);
            target.draw(arrow);
            arrow.move(cos(dir) * -distance, sin(dir) * -distance);
        }
    }
}

void World::addPlanet(const Planet& p)
{
    Game::Planet tmp = p;

    float radius = tmp.getRadius() * 2.f;

    std::random_device dev;
    std::uniform_real_distribution<float> distX(radius, mSize.x - radius);
    std::uniform_real_distribution<float> distY(radius, mSize.y - radius);

    sf::Vector2f pos;
    struct : public b2QueryCallback {
        bool occupied;
        bool ReportFixture(b2Fixture*) { occupied = true; return false; }
    } occupyCheck;

    unsigned int attempts = 0;

    do
    {
        occupyCheck.occupied = false;
        pos = sf::Vector2f(distX(dev) - mSize.x / 2.f, distY(dev) - mSize.y / 2.f);

        b2AABB aabb;
        aabb.lowerBound.Set(pos.x / 5.f-radius / 5.f, pos.y / 5.f-radius / 5.f);
        aabb.upperBound.Set(pos.x / 5.f+radius / 5.f, pos.y / 5.f+radius / 5.f);
        mBox2DWorld->QueryAABB(&occupyCheck, aabb);
    } while (occupyCheck.occupied && attempts++ < 100);

    if (occupyCheck.occupied)
        return;

    tmp.setPosition(pos);
    tmp.addToWorld(*this);

    mPlanets.push_back(tmp);
}

void World::addShip(const Ship& s)
{
    Game::Ship tmp = s;

    std::random_device dev;
    std::uniform_int_distribution<int> distP(0, mPlanets.size()-1);
    std::uniform_real_distribution<float> distR(0, 360);

    int planet = distP(dev);
    float angle = distR(dev) * (M_PI/180);

    Planet* targetPlanet = nullptr;
    unsigned int i = 0;
    FOR_EACH (auto& p, mPlanets)
        if (i++ == planet)
        {
            targetPlanet = &p;
            break;
        }

    auto planetPos = targetPlanet->getPosition();
    sf::Vector2f dir(cos(angle), sin(angle));

    tmp.setPosition(planetPos + dir * (targetPlanet->getRadius() + 5));
    tmp.setAngle(angle + (90 * (M_PI/180)));
    tmp.addToWorld(*this);

    mShips.push_back(tmp);
}

void World::addWeapon(const Weapon& w)
{
    Weapon tmp = w;

    tmp.addToWorld(*this);

    mWeapons.push_back(tmp);
}