#include "Planet.hpp"
#include "World.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Shader.hpp>
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

    mWellShader = std::make_shared<sf::Shader>();
    mWellShader->loadFromFile("well.frag", sf::Shader::Fragment);
}

Planet::Planet(const Planet& other)
{
    mDirty = other.mDirty;
    mRadius = other.mRadius;
    mPosition = other.mPosition;
    mBody = other.mBody;

    mWellShader = other.mWellShader;
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

Planet::~Planet()
{
}

void Planet::addedToWorld(World& world)
{
    auto& b2d = *world.getBox2D();

    {
        b2BodyDef def;
        def.type = b2_staticBody;
        def.position = b2Vec2(mPosition.x / 5.f, mPosition.y / 5.f);
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
        shape.m_radius = mRadius / 5.f;

        b2FixtureDef def;
        def.density = 1.f;
        def.isSensor = false;
        def.friction = 1.f;
        def.restitution = 0;
        def.shape = &shape;
        def.userData = this;

        auto fix = body.CreateFixture(&def);
    }
}

void Planet::setPosition(const sf::Vector2f& pos)
{
    mPosition = pos;
    if (mBody)
        mBody->SetTransform(b2Vec2(pos.x / 5.f, pos.y / 5.f), 0);
}

float Planet::getPercentage() const
{
    return 1;
}

void Planet::draw(sf::RenderTarget& target)
{
    ///\TODO Better graphics :D
    
    sf::CircleShape shape(mRadius - 1.f, 36);
    shape.setPosition(mPosition);
    shape.setFillColor(sf::Color(0,75,0,255));
    shape.setOutlineColor(sf::Color::Green);
    shape.setOutlineThickness(1.f);
    shape.setOrigin(mRadius-1.f, mRadius-1.f);

    target.draw(shape);
}

void Planet::drawWell(sf::RenderTarget& target)
{
    sf::CircleShape shape(mRadius*4 * getPercentage(), 16);
    shape.setPosition(mPosition);
    shape.setOrigin(mRadius*4 * getPercentage(), mRadius*4 * getPercentage());
    
    if (sf::Shader::isAvailable())
    {
        shape.setFillColor(sf::Color::Transparent);

        auto& tone = *mWellShader;
        auto& view = target.getView();
        auto pos = (sf::Vector2f)target.mapCoordsToPixel(mPosition);

        tone.setParameter("sizeDiff", target.getSize().y / view.getSize().y);
        tone.setParameter("center", pos.x, target.getSize().y - pos.y, mRadius * getPercentage(), mRadius * 4 * getPercentage());
        tone.setParameter("color", sf::Color(0,64,255,255));

        target.draw(shape, &tone);
    }
    else
    {
        shape.setFillColor(sf::Color(0,0,255,50));

        target.draw(shape);
    }
}