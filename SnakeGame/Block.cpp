#include "Block.h"


Block::Block()
{
    shape.setSize({ SETTINGS.BLOCK_WIDTH, SETTINGS.BLOCK_HEIGHT });
    shape.setOrigin(SETTINGS.BLOCK_WIDTH / 2.f, SETTINGS.BLOCK_HEIGHT / 2.f);
    isDestroyed = false;
}

void Block::Init(Game& game)
{
  
    isDestroyed = false;
}

void Block::Spawn(sf::Vector2f position, sf::Color color)
{
    shape.setPosition(position);
    shape.setFillColor(color);
    isDestroyed = false;
}

void Block::Update(float deltaTime, Game& game, sf::RenderWindow& window)
{
  
}

void Block::Draw(sf::RenderWindow& window)
{
    if (!isDestroyed) {
        window.draw(shape);
    }
}

void Block::Hit()
{
    isDestroyed = true;
}

sf::FloatRect Block::GetBounds() const
{
    return shape.getGlobalBounds();
} 