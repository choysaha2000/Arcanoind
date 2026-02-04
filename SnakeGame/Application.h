#include <SFML/Graphics.hpp>
#include "Game.h"


class Application
{
public:

	// singleton realization
	static Application& instance()
	{
		static Application instance;
		return instance;
	}

	// delete default copy constructor 
	Application(const Application&) = delete;
	Application& operator = (const Application) = delete;
	

	void Run();

	// get private 

	sf::RenderWindow& GetWindow() { return window; }

	Game& GetGame() { return game; }

private:
	// private construct
	Application();

	// private deconcturctor
	~Application();

private:
	Game game;

	sf::RenderWindow window;
};

