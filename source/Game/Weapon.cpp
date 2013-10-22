#include "Weapon.hpp"
#include "Ship.hpp"
#include "World.hpp"
#include <Box2D/Box2D.h>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>

using namespace Game;

Weapon::Weapon(Ship* own, float ang, float force) :
    mOwner(own), mFireAng(ang), mFireForce(force), mBody(nullptr), mLiveTime(0), mDestroyed(false)
{
}

Weapon::Weapon(const Weapon& other)
{
    mOwner = other.mOwner;
    mBody = other.mBody;

    mFireAng = other.mFireAng;
    mFireForce = other.mFireForce;
    mLiveTime = other.mLiveTime;

    mDestroyed = other.mDestroyed;
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

Weapon::~Weapon()
{
}


sf::Vector2f Weapon::getPosition() const
{
    if (mBody)
    {
        auto& pos = mBody->GetPosition();
        return sf::Vector2f(pos.x * 5.f, pos.y * 5.f);
    }

    return sf::Vector2f();
}

float Weapon::getAngle() const
{
    return 0;
}


void Weapon::setOwner(Ship* owner)
{
    if (mBody)
    {
        mBody->SetUserData(this);
        auto fix = mBody->GetFixtureList();
        do
        {
            b2Filter old = fix->GetFilterData();
            old.groupIndex = owner->mGroup;
            fix->SetFilterData(old);
        } while (fix = fix->GetNext());
    }

    mOwner = owner;
}

void Weapon::addedToWorld(Game::World& world)
{
    auto& b2d = *world.getBox2D();

    {
        auto pos = mOwner->getPosition();

        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position = b2Vec2(pos.x / 5.f, pos.y / 5.f);
        def.angle = 0;
        def.linearVelocity = b2Vec2(0, 0);
        def.angularVelocity = 0;
        def.linearDamping = 0;
        def.angularDamping = 0;
        def.allowSleep = true;
        def.awake = true;
        def.fixedRotation = false;
        def.bullet = true;
        def.active = true;
        def.userData = this;
        def.gravityScale = 0;

        mBody = b2d.CreateBody(&def);
    }

    auto& body = *mBody;
    
    {
        b2CircleShape shape;
        shape.m_p.SetZero();
        shape.m_radius = 1 / 5.f;

        b2FixtureDef def;
        def.density = 1.f;
        def.isSensor = false;
        def.friction = 0.1f;
        def.restitution = 0.5f;
        def.shape = &shape;
        def.userData = this;

        def.filter.groupIndex = mOwner->mGroup;

        auto fix = body.CreateFixture(&def);
    }

    sf::Vector2f force = sf::Vector2f(cos(mFireAng), sin(mFireAng)) * mFireForce * 20.f;

    mBody->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
}

void Weapon::addGravity(const sf::Vector2f& pos, float strength)
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
    sf::Vector2f delta = ((getPosition() - pos) / dist)  * strength * 25.f;

    mBody->ApplyForceToCenter(b2Vec2(-delta.x, -delta.y), true);
}

void Weapon::collide(Entity& other)
{
    getWorld().addExplosion(getPosition(), 2048.f);

    mDestroyed = true;
}

void Weapon::update(float dt)
{
    mLiveTime += dt;

    if (mLiveTime > 30)
    {
        mDestroyed = true;
    }
}

void Weapon::draw(sf::RenderTarget& target)
{
    static const unsigned int ghosts = 5;

    auto velocity = sf::Vector2f();
    {
        auto b2vel = mBody->GetLinearVelocity();
        velocity.x = b2vel.x / 25;
        velocity.y = b2vel.y / 25;
    }

    sf::CircleShape bullet(1, 6);

    float alphagain = 255.f / ghosts;
    bullet.setFillColor(sf::Color(0,0,0,0));
    bullet.setPosition(getPosition());
    bullet.setOrigin(1, 1);
    
    bullet.move(velocity * -(float)(ghosts+1));

    for (unsigned int i = 0; i < ghosts; ++i)
    {
        bullet.move(velocity);
        bullet.setFillColor(sf::Color(255,255,255,alphagain * (i+1)));
        target.draw(bullet);
    }
}
