#include "Block.h"


Block::Block()
{
    shape.setSize({ BLOCK_WIDTH, BLOCK_HEIGHT });
    shape.setOrigin(BLOCK_WIDTH / 2.f, BLOCK_HEIGHT / 2.f);
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

sf::FloatRect Block::GetBounds() const
{
    return shape.getGlobalBounds();
} 