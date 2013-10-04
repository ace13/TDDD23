#pragma once

#include <SFML/System/Vector.hpp>

namespace sf { class RenderTarget; }

class Entity
{
public:
	sf::Vector2f getPosition() const;
	float getAngle() const;

	void update(float dt);
	void draw(sf::RenderTarget& target);
};
