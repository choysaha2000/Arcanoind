#include "Application.h"
#include <cstdlib>
#include "Constants.h"
#include "ctime"


Application::Application()
    : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SnakeGame")
{
    int seed = (int)time(nullptr);
    srand(seed);

}


Application::~Application()
{
 
}


void Application::Run()
{
	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();

	game.timeSinceGameFinish = 0.0f;

	// game loop

    while (window.isOpen())
    {
        float currentTime = gameClock.getElapsedTime().asSeconds();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::TextEntered)
            {
                if (game.GetCurrentGameState() == GameState::Cin && game.isInputActive)
                {

                    // cin ivent
                    if (event.text.unicode < 128)
                    {
                        char enteredChar = static_cast<char>(event.text.unicode);

                        if (enteredChar == 8)
                        {
                            if (!game.tempPlayerName.empty())
                                game.tempPlayerName.pop_back();
                        }

                        else if (enteredChar >= 32 && enteredChar <= 126)
                        {
                            if (game.tempPlayerName.size() < 10)
                                game.tempPlayerName += enteredChar;
                        }
                    }
                }
            }

            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
                break;
            }

        }
        game.Update(deltaTime, window);

        game.Draw(window);

        window.display();
    }
}