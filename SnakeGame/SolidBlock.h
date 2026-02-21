#pragma once
#include "Block.h"

class SolidBrick : public Block
{
public:
    SolidBrick();

    // переопределяем наследника 
    void Spawn(sf::Vector2f position, sf::Color color) override;
    void Hit() override;

private:
    int health; 
};