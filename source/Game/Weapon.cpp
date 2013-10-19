#include "Weapon.hpp"
#include "Ship.hpp"
#include "World.hpp"
#include <Box2D/Box2D.h>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>

using namespace Game;

Weapon::Weapon(Ship& own, float ang, float force) :
    mOwner(own), mFireAng(ang), mFireForce(force), mBody(nullptr), mLiveTime(0)
{

}

Weapon::~Weapon()
{
}


sf::Vector2f Weapon::getPosition() const
{
    if (mBody)
    {
        auto& pos = mBody->GetPosition();
        return sf::Vector2f(pos.x, pos.y);
    }

    return sf::Vector2f();
}

float Weapon::getAngle() const
{
    return 0;
}


void Weapon::addedToWorld(Game::World& world)
{
    auto& b2d = *world.getBox2D();

    {
        auto pos = mOwner.getPosition();

        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position = b2Vec2(pos.x, pos.y);
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
        shape.m_radius = 1;

        b2FixtureDef def;
        def.density = 1.f;
        def.isSensor = false;
        def.friction = 0.1f;
        def.restitution = 0.5f;
        def.shape = &shape;

        def.filter.groupIndex = mOwner.mGroup;

        auto fix = body.CreateFixture(&def);
    }

    sf::Vector2f force = sf::Vector2f(cos(mFireAng), sin(mFireAng)) * mFireForce * 500.f;

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
    sf::Vector2f delta = ((getPosition() - pos) / dist)  * strength * 150.f;

    mBody->ApplyForceToCenter(b2Vec2(-delta.x, -delta.y), true);
}

void Weapon::collide(Entity& other)
{
    auto& weaps = getWorld().getWeapons();

    //auto it = std::find(weaps.begin(), weaps.end(), *this);
    //weaps.erase(it);
}

void Weapon::update(float dt)
{
    mLiveTime += dt;
}

void Weapon::draw(sf::RenderTarget& target)
{
    sf::CircleShape bullet(1, 6);

    bullet.setPosition(getPosition());
    bullet.setOrigin(1, 1);
    
    target.draw(bullet);
}
