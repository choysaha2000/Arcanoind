#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <vector>
#include "GameObject.h"

class Game;

class Paddle : public GameObject
{
public:
    Paddle();

   
    void Init(Game& game) override;
    void Update(float deltaTime, Game& game, sf::RenderWindow& window) override;
    void Draw(sf::RenderWindow& window) override;

    sf::FloatRect GetBounds() const;
    sf::Vector2f GetPosition() const;

private:
    sf::RectangleShape shape;
    float speed;
};


class Ball: public GameObject
{
public:
    Ball();
    void Init(Game& game) override;
    void Update(float deltaTime, Game& game, sf::RenderWindow& window) override;
    void Draw(sf::RenderWindow& window) override;


    void BounceX();
    void BounceY();
    sf::Vector2f GetVelocity() const;

    sf::FloatRect GetBounds() const;
    sf::Vector2f GetPosition() const;

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float speed;
};