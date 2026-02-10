#include "Player.h"
#include <cmath>


// set paddle
Paddle::Paddle()
    : speed(PADDLE_SPEED)
{
    shape.setSize({ PADDLE_WIDTH, PADDLE_HEIGHT });
    shape.setFillColor(sf::Color::Blue);
    shape.setOrigin(PADDLE_WIDTH / 2.f, PADDLE_HEIGHT / 2.f);
}

void Paddle::Init(Game& game)
{
    shape.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - PADDLE_Y_OFFSET);
}


void Paddle::Update(float deltaTime, Game& game, sf::RenderWindow& window)
{
    sf::Vector2f pos = shape.getPosition();
    bool keyInput = false;


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        pos.x -= speed * deltaTime;
        keyInput = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        pos.x += speed * deltaTime;
        keyInput = true;
    }

    if (!keyInput)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        
        if (mousePos.x >= 0 && mousePos.x <= SCREEN_WIDTH &&
            mousePos.y >= 0 && mousePos.y <= SCREEN_HEIGHT)
        {
            pos.x = static_cast<float>(mousePos.x);
        }
    }


    if (pos.x - PADDLE_WIDTH / 2.f < 0.f) pos.x = PADDLE_WIDTH / 2.f;
    if (pos.x + PADDLE_WIDTH / 2.f > SCREEN_WIDTH) pos.x = SCREEN_WIDTH - PADDLE_WIDTH / 2.f;

    shape.setPosition(pos);
}

void Paddle::Draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect Paddle::GetBounds() const { return shape.getGlobalBounds(); }
sf::Vector2f Paddle::GetPosition() const { return shape.getPosition(); }


// ball
Ball::Ball()
    : speed(BALL_SPEED)
{
    shape.setRadius(BALL_RADIUS);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(BALL_RADIUS, BALL_RADIUS);
}


void Ball::Init(Game& game)
{
    shape.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
    
    velocity = { speed * 0.707f, speed * 0.707f };
}


void Ball::Update(float deltaTime, Game& game, sf::RenderWindow& window)
{
    shape.move(velocity * deltaTime);
}

void Ball::Draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

void Ball::BounceX() { velocity.x = -velocity.x; }
void Ball::BounceY() { velocity.y = -velocity.y; }

sf::FloatRect Ball::GetBounds() const { return shape.getGlobalBounds(); }
sf::Vector2f Ball::GetPosition() const { return shape.getPosition(); }
sf::Vector2f Ball::GetVelocity() const { return velocity; }