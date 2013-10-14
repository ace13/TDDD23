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

//convert a Box2D (float 0.0f - 1.0f range) color to a SFML color (uint8 0 - 255 range)
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
/*
    //red (X axis)
    p2 = p1 + (lineProportion * xf.q.GetXAxis());
    sf::Shape redLine = sf::Shape::Line(p1.x*RATIO, p1.y*RATIO, p2.x*RATIO, p2.y*RATIO, 1, sf::Color::Red);

    //green (Y axis)
    p2 = p1 - (lineProportion * xf.q.GetYAxis());
    sf::Shape greenLine = sf::Shape::Line(p1.x*RATIO, p1.y*RATIO, p2.x*RATIO, p2.y*RATIO, 1, sf::Color::Green);

    this->window->Draw(redLine);
    this->window->Draw(greenLine);
*/
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    /*
    sf::Shape line = sf::Shape::Line(p1.x*RATIO, p1.y*RATIO, p2.x*RATIO, p2.y*RATIO, 1, this->B2SFColor(color));
    line.EnableFill(true);
    this->window->Draw(line);
    */
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    sf::CircleShape circle(radius);// = sf::Shape::Circle(center.x*RATIO, center.y*RATIO, radius*RATIO, this->B2SFColor(color, 50), 1.0f,this->B2SFColor(color));
    
    circle.setOrigin(radius/2, radius/2);
    circle.setOutlineColor(B2SFColor(color));
    circle.setOutlineThickness(1);
    circle.setFillColor(sf::Color::Transparent);
    circle.setPosition(center.x * RATIO, center.y * RATIO);

    this->window->draw(circle);

}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    sf::CircleShape circle(radius);// = sf::Shape::Circle(center.x*RATIO, center.y*RATIO, radius*RATIO, this->B2SFColor(color, 50), 1.0f,this->B2SFColor(color));

    circle.setOrigin(radius/2, radius/2);
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
    polygon.setFillColor(B2SFColor(color));
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