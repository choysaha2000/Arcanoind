#include "SolidBlock.h"


SolidBrick::SolidBrick()
{
	health = 3;
}

void SolidBrick::Spawn(sf::Vector2f position, sf::Color color)
{
	Block::Spawn(position, sf::Color::Red);
	health = 3;
}

void SolidBrick::Hit()
{
	health--;

	if (health == 2)
	{
		shape.setFillColor(sf::Color(255, 165, 0));
	}

	else if (health == 1) {
		shape.setFillColor(sf::Color::Yellow); 
	}
	else if (health <= 0) {
		isDestroyed = true; 
	}
}