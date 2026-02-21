#pragma once
#include "GameObject.h"
#include "Constants.h"


class Block : public GameObject
{
public:
    Block();


    void Init(Game& game) override;
    void Update(float deltaTime, Game& game, sf::RenderWindow& window) override;
    void Draw(sf::RenderWindow& window) override;

    // переопределение
    virtual void Spawn(sf::Vector2f position, sf::Color color);
    
    // попадание мяча
    virtual void Hit();
    
    sf::FloatRect GetBounds() const;
    bool IsDestroyed() const { return isDestroyed; }
    void Destroy() { isDestroyed = true; }

    // модификатор дает доступ к наследникам
protected:
    sf::RectangleShape shape;
    bool isDestroyed = false; 
};



