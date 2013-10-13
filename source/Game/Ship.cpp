#include "Ship.hpp"
#include "World.hpp"

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
        def.position.SetZero();
        def.angle = 0;
        def.linearVelocity.SetZero();
        def.angularVelocity = 0;
        def.linearDamping = 0;
        def.angularDamping = 0.01f;
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
            b2Vec2(0, 0)
        };
        shape.Set(points, sizeof(points)/sizeof(b2Vec2));

        b2FixtureDef def;
        def.density = 1;
        def.isSensor = false;
        def.friction = 1;
        def.restitution = 0.25f;
        def.shape = &shape;

        auto fix = body.CreateFixture(&def);
    }
}


void Ship::update(float dt)
{

}

void Ship::draw(sf::RenderTarget& target)
{

}
