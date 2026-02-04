#pragma once
#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// MAYBE THAT WOULD BE NEED IN FUTURE

struct Rectangle
{
	Position2D position;
	Vector2D size;
};

struct Circle
{
	Position2D position;
	float radius{};
};

struct math
{
	Rectangle snakeRect;
	Circle appleCircle;
};

// Math
bool DoShapesCollide(const Rectangle& rect1, const Rectangle& rect2);
bool DoShapesCollide(const Circle& circle1, const Circle& circle2);
bool DoShapesCollide(const Rectangle& rect, const Circle& circle);
Position2D GetRandomPositionInField();
void SetSpriteSize(sf::Sprite& sprite, float width, float height);
void SetSpriteRelativeOrigin(sf::Sprite& sprite, float x, float y);