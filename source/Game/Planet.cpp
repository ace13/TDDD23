#include "Planet.hpp"
#include "World.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <random>

#include <Box2D/Box2D.h>

using namespace Game;

Planet::Planet() :
    mDirty(false), mRadius(0), mBody(nullptr)
{
    std::random_device dev;
    std::uniform_real_distribution<float> dist(50, 80);
    
    mRadius = dist(dev);
    mPosition = sf::Vector2f(dist(dev), dist(dev));
}

Planet::~Planet()
{
}

void Planet::addedToWorld(World& world)
{
    auto& b2d = *world.getBox2D();

    {
        b2BodyDef def;
        def.type = b2_staticBody;
        def.position = b2Vec2(mPosition.x, mPosition.y);
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
        def.userData = this;
        def.gravityScale = 0;

        mBody = b2d.CreateBody(&def);
    }

    auto& body = *mBody;
    
    {
        b2CircleShape shape;
        shape.m_p.SetZero();
        shape.m_radius = mRadius * 10;

        b2FixtureDef def;
        def.density = 1;
        def.isSensor = false;
        def.friction = 1;
        def.restitution = 0.25f;
        def.shape = &shape;

        auto fix = body.CreateFixture(&def);
    }
}

void Planet::setPosition(const sf::Vector2f& pos)
{
    mPosition = pos;
    if (mBody)
        mBody->SetTransform(b2Vec2(pos.x, pos.y), 0);
}

float Planet::getPercentage() const
{
    return 1;
}

void Planet::draw(sf::RenderTarget& target)
{
    if (!mBody)
        return;

    ///\TODO Better graphics :D
    sf::CircleShape shape(mRadius, 64);
//    shape.setFillColor(sf::Color::White);
    shape.setOutlineColor(sf::Color::Green);
    shape.setOutlineThickness(4.f);
    shape.setPosition(mPosition);

    shape.setOrigin(mRadius/2.f, mRadius/2.f);

    target.draw(shape);
}