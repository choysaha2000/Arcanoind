#include "Block.h"

// –азмеры блока можно вынести в Constants.h, но пусть пока будут тут


Block::Block()
{
    shape.setSize({ BLOCK_WIDTH, BLOCK_HEIGHT });
    shape.setOrigin(BLOCK_WIDTH / 2.f, BLOCK_HEIGHT / 2.f);
    isDestroyed = false;
}

void Block::Init(Game& game)
{
    // ѕустой метод, так как мы используем Spawn дл€ конкретной позиции
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
    // Ѕлоки статичны, логики обновлени€ нет
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