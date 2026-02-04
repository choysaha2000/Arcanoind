#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <vector>



class Paddle
{
public:
    Paddle();
    void Init();
    void Update(float deltaTime, const sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);
    sf::FloatRect GetBounds() const;
    sf::Vector2f GetPosition() const;

private:
    sf::RectangleShape shape;
    float speed;
};


class Ball
{
public:
    Ball();
    void Init();
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);

    // PHYSICS
    void BounceX();
    void BounceY();

    sf::FloatRect GetBounds() const;
    sf::Vector2f GetPosition() const;
    sf::Vector2f GetVelocity() const;

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float speed;
};