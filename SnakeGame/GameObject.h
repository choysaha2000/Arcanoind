#pragma once
#include <SFML/Graphics.hpp>

class Game; // Forward declaration

class GameObject
{
public:
    virtual ~GameObject() = default;
    virtual void Init(Game& game) = 0;
    virtual void Update(float deltaTime, Game& game, sf::RenderWindow& window) = 0;
    virtual void Draw(sf::RenderWindow& window) = 0;
};