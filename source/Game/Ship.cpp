#include "Ship.hpp"
#include "World.hpp"
#include "Player.hpp"
#include "../Config.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <Box2D/Box2D.h>

using namespace Game;

Ship::Ship() :
    mPlayer(nullptr), mBody(nullptr), mAngle(0), mFlyTime(0), mAnimTime(0), mTurn(false)
{
}

Ship::Ship(const Ship& other)
{
    mPlayer = other.mPlayer;
    mBody = other.mBody;
    mGroup = other.mGroup;
    mAngle = other.mAngle;
    mPosition = other.mPosition;
    mFlyTime = other.mFlyTime;
    mAnimTime = other.mAnimTime;
    mTurn = other.mTurn;

    setWorld(other.getWorld());

    if (mBody)
    {
        mBody->SetUserData(this);
        auto fix = mBody->GetFixtureList();

        do
        {
            fix->SetUserData(this);
        } while (fix = fix->GetNext());
    }
}

Ship::~Ship()
{
}

sf::Vector2f Ship::getPosition() const
{
    if (mBody)
    {
        auto pos = mBody->GetPosition();
        return sf::Vector2f(pos.x * 5.f, pos.y * 5.f);
    }
    return mPosition;
}

float Ship::getAngle() const
{
    if (mBody)
    {
        return mBody->GetAngle();
    }
    return mAngle;
}

void Ship::setPlayer(Player* p)
{
    mPlayer = p;
}

void Ship::addedToWorld(World& world)
{
    auto& b2d = *world.getBox2D();

    {
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position.Set(mPosition.x / 5.f, mPosition.y / 5.f);
        def.angle = mAngle;
        def.linearVelocity.SetZero();
        def.angularVelocity = 0;
        def.linearDamping = 0;
        def.angularDamping = 1;
        def.allowSleep = true;
        def.awake = true;
        def.fixedRotation = false;
        def.bullet = false;
        def.active = true;
        def.userData = this;
        def.gravityScale = 0;

        mBody = b2d.CreateBody(&def);
    }

    auto& body = *mBody;
    
    {
        b2PolygonShape shape;
        {
            b2Vec2 points[] = {
                b2Vec2(0, -1),
                b2Vec2(1, 1),
                b2Vec2(0, 0.9)
            };
            shape.Set(points, sizeof(points)/sizeof(b2Vec2));
        }

        b2FixtureDef def;
        def.density = 1.f;
        def.isSensor = false;
        def.friction = 0.75f;
        def.restitution = 0.f;
        def.shape = &shape;
        def.userData = this;

        def.filter.groupIndex = -((int)world.getShips().size() + 1);
        mGroup = def.filter.groupIndex;

        auto fix = body.CreateFixture(&def);

        {
            b2Vec2 points[] = {
                b2Vec2(0, 0.9),
                b2Vec2(-1, 1),
                b2Vec2(0, -1)
            };
            shape.Set(points, sizeof(points)/sizeof(b2Vec2));
        }

        fix = body.CreateFixture(&def);
    }
}

void Ship::setPosition(const sf::Vector2f& pos)
{
    mPosition = pos;
    if (mBody)
        mBody->SetTransform(b2Vec2(pos.x / 5.f, pos.y / 5.f), getAngle());
}

void Ship::setAngle(float ang)
{
    mAngle = ang;
    if (mBody)
    {
        auto p = getPosition();
        mBody->SetTransform(b2Vec2(p.x / 5.f, p.y / 5.f), ang);
    }
}

void Ship::addGravity(const sf::Vector2f& pos, float strength)
{
    auto dotProd = [](const sf::Vector2f& p) -> float {
        return (p.x*p.x) + (p.y*p.y);
    };
    auto calcDist = [dotProd](const sf::Vector2f& a, const sf::Vector2f& b) -> float {
        return sqrt(dotProd(b-a));
    };
    auto getUnit = [dotProd](const sf::Vector2f& vec) -> sf::Vector2f {
        return vec / (float)sqrt(dotProd(vec));
    };

    float dist = calcDist(getPosition(), pos);
    sf::Vector2f delta = ((getPosition() - pos) / dist)  * strength * 100.f;
    b2Vec2 force(-delta.x, -delta.y);

    mBody->ApplyForceToCenter(force, false);
    if (mFlyTime > 1.f)
    {
        float ang = mBody->GetAngle();
        float target = atan2(force.y, force.x) - (M_PI/2);
        mBody->SetTransform(mBody->GetPosition(), target);
    }

    if (mFlyTime > 10.f)
    {
        b2Vec2 vel = mBody->GetLinearVelocity();
        vel *= -0.75f;
        mBody->ApplyForceToCenter(vel, true);
    }
}


void Ship::update(float dt)
{
    mAnimTime += dt;

    if (mBody->IsAwake())
    {
        mFlyTime += dt;
    }
    else if (mFlyTime > 0)
    {
        mFlyTime = 0;
    }
}

void Ship::draw(sf::RenderTarget& target)
{
    if (!mBody)
        return;

    sf::ConvexShape shape(3);
    shape.setPoint(0, sf::Vector2f(0, -6));
    shape.setPoint(1, sf::Vector2f(6, 6));
    shape.setPoint(2, sf::Vector2f(-6, 6));

    if (mPlayer)
        shape.setFillColor(mPlayer->getColor());

    float ang = mBody->GetAngle();
    auto pos = getPosition();
    shape.setRotation(ang * (180/M_PI));
    shape.setOrigin(sf::Vector2f(0, 0));
    shape.setPosition(pos.x, pos.y);

    if (mTurn)
    {
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1.f + sin(mAnimTime*2)/2.f);
    }

    target.draw(shape);
}
