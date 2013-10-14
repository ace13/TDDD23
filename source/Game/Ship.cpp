#include "Ship.hpp"
#include "World.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <Box2D/Box2D.h>

using namespace Game;

Ship::Ship() :
    mPlayer(nullptr), mBody(nullptr)
{
}

Ship::~Ship()
{
}

sf::Vector2f Ship::getPosition() const
{
    if (mBody)
    {
        auto pos = mBody->GetPosition();
        return sf::Vector2f(pos.x, pos.y);
    }
    return sf::Vector2f();
}

float Ship::getAngle() const
{
    if (mBody)
    {
        return mBody->GetAngle();
    }
    return 0.f;
}


void Ship::addedToWorld(World& world)
{
    auto& b2d = *world.getBox2D();

    {
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position.Set(400, 100);
        def.angle = 0;
        def.linearVelocity.SetZero();
        def.angularVelocity = 0;
        def.linearDamping = 0;
        def.angularDamping = 0;
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
        b2Vec2 points[] = {
            b2Vec2(0, -5),
            b2Vec2(5, 5),
            b2Vec2(-5, 5)
        };
        shape.Set(points, sizeof(points)/sizeof(b2Vec2));

        b2FixtureDef def;
        def.density = 1;
        def.isSensor = false;
        def.friction = 1;
        def.restitution = 0.01f;
        def.shape = &shape;

        auto fix = body.CreateFixture(&def);
    }
}

void Ship::addGravity(const sf::Vector2f& pos)
{
    auto dotProd = [](const sf::Vector2f& p) -> float {
        return (p.x*p.x) + (p.y*p.y);
    };
    auto calcDist = [dotProd](const sf::Vector2f& a, const sf::Vector2f& b) -> float {
        return sqrt(dotProd(b-a));
    };
    auto getUnit = [dotProd](const sf::Vector2f& vec) -> sf::Vector2f {
        return vec / sqrt(dotProd(vec));
    };

    float dist = calcDist(getPosition(), pos);
    sf::Vector2f delta = (getPosition() - pos) / dist;

    mBody->ApplyForceToCenter(b2Vec2(-delta.x, -delta.y), true);
}


void Ship::update(float dt)
{

}

void Ship::draw(sf::RenderTarget& target)
{
    if (!mBody)
        return;

    sf::ConvexShape shape(3);
    shape.setPoint(0, sf::Vector2f(0, -15));
    shape.setPoint(1, sf::Vector2f(10, 5));
    shape.setPoint(2, sf::Vector2f(-10, 5));

    float ang = mBody->GetAngle();
    auto pos = mBody->GetPosition();
    shape.setRotation(ang * (180/3.14159));
    shape.setOrigin(sf::Vector2f(0, -10));
    shape.setPosition(pos.x, pos.y);

    //printf("Position %fx%f...\n", pos.x, pos.y);

    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.f);

    target.draw(shape);
}
