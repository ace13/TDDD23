#include "DebugDraw.hpp"

static const float RATIO = 1;

DebugDraw::DebugDraw(sf::RenderTarget &window)
{
    this->window = &window;

    mFont.loadFromFile("Dosis-Book.ttf");
}

DebugDraw::~DebugDraw()
{

}

sf::Color DebugDraw::B2SFColor(const b2Color &color, int alpha = 255)
{
    sf::Color result((sf::Uint8)(color.r*255), (sf::Uint8)(color.g*255), (sf::Uint8)(color.b*255), (sf::Uint8) alpha);
    return result;
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
    float lineProportion;
    lineProportion = 0.15; // 0.15 ~ 10 pixels
    b2Vec2 p1 = xf.p, p2;

    p2 = p1 + (lineProportion * xf.q.GetXAxis());
    DrawLine(sf::Vector2f(p1.x * RATIO, p1.y * RATIO), sf::Vector2f(p2.x * RATIO, p2.y * RATIO), 1, sf::Color::Red);
    p2 = p1 - (lineProportion * xf.q.GetYAxis());
    DrawLine(sf::Vector2f(p1.x * RATIO, p1.y * RATIO), sf::Vector2f(p2.x * RATIO, p2.y * RATIO), 1, sf::Color::Green);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    DrawLine(sf::Vector2f(p1.x, p1.y), sf::Vector2f(p2.x, p2.y), 1.f, B2SFColor(color));
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    sf::CircleShape circle(radius * RATIO);
    
    circle.setOrigin(radius * RATIO, radius * RATIO);
    circle.setOutlineColor(B2SFColor(color));
    circle.setOutlineThickness(1);
    circle.setFillColor(B2SFColor(color, 50));
    circle.setPosition(center.x * RATIO, center.y * RATIO);

    this->window->draw(circle);

    DrawLine(sf::Vector2f(center.x * RATIO, center.y * RATIO), sf::Vector2f(center.x * RATIO, center.y * RATIO) + sf::Vector2f(axis.x * radius * RATIO, axis.y * radius * RATIO), 1.f, B2SFColor(color));
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    sf::CircleShape circle(radius);

    circle.setOrigin(radius * RATIO, radius * RATIO);
    circle.setOutlineColor(B2SFColor(color));
    circle.setOutlineThickness(1);
    circle.setFillColor(sf::Color::Transparent);
    circle.setPosition(center.x * RATIO, center.y * RATIO);

    this->window->draw(circle);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::ConvexShape polygon(vertexCount);
    for (int32 i=0; i<vertexCount; i++)
    {
        b2Vec2 vertex = vertices[i];
        polygon.setPoint(i, sf::Vector2f(vertex.x*RATIO, vertex.y*RATIO));
    }
    polygon.setFillColor(B2SFColor(color,50));
    polygon.setOutlineColor(B2SFColor(color));
    polygon.setOutlineThickness(1.f);
    this->window->draw(polygon);

}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::ConvexShape polygon(vertexCount);
    for (int32 i=0; i<vertexCount; i++)
    {
        b2Vec2 vertex = vertices[i];
        polygon.setPoint(i, sf::Vector2f(vertex.x*RATIO, vertex.y*RATIO));
    }
    polygon.setFillColor(sf::Color::Transparent);
    polygon.setOutlineColor(B2SFColor(color));
    polygon.setOutlineThickness(1.f);
    this->window->draw(polygon);

}

void DebugDraw::DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2, float thick, const sf::Color& c)
{
    float distance = getDistance(p1, p2);
    sf::Vector2f dir = getUnit(p2 - p1);

    sf::RectangleShape line(sf::Vector2f(distance, thick));
    line.setOrigin(0, thick/2.f);
    line.setPosition(p1);
    line.setRotation(atan2(dir.y, dir.x * (180/3.14159f)));
    line.setFillColor(c);

    window->draw(line);
}