#pragma once

#include <Box2D/Box2D.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

class DebugDraw : public b2Draw
{
public:
    DebugDraw(sf::RenderTarget &window);
    virtual ~DebugDraw();

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    void DrawTransform(const b2Transform& xf);

private:
    void DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2, float thick, const sf::Color& c);
    sf::Color B2SFColor(const b2Color &color, int alpha);

    inline float dotProduct(const sf::Vector2f& p1, const sf::Vector2f& p2) { return (p1.x*p2.x)+(p1.y*p2.y); }
    inline float getDistance(const sf::Vector2f& p1, const sf::Vector2f& p2) { return getLength(p2-p1); }
    inline float getLength(const sf::Vector2f& p) { return sqrt(dotProduct(p, p)); }
    inline sf::Vector2f getUnit(const sf::Vector2f& p) { return p / getLength(p); }


    sf::RenderTarget *window;
    sf::Font mFont;
};
