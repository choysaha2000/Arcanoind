#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <vector>
#include <unordered_map>
#include <fstream>

class Game;

enum class MenuItem {
	Start,
	Leaderboard,
	Diffucult,
	Options,
	Exit,
	Count
};

enum class Options
{
	Sound,
	Music,
	Back,
	Count
};

enum class Diffucult
{
	Easy,
	Easy2,
	Middle,
	Hard,
	VeryHard,
	Back,
	Count
};

enum class Cin
{
	YES,
	NO,
	Count
};

class UI
{
public:
	UI() = default;
	~UI() = default;



	UI(const UI&) = delete;
	UI& operator = (const UI&) = delete;


	void InitUI(Game& game);


    void InitializeLeaderBoard();
    void UpdateLeaderboardGameOver(int playerScore, std::string name);
    std::string GetLeaderboardString(const std::unordered_map<std::string, int>& records);

    void UpdateLeaderBoardState(Game& game, float deltaTime);
    void DrawLeaderBoard(sf::RenderWindow& window);

    void DrawPlaying(Game& game, sf::RenderWindow& window);


    void StartMenuState();
    void UpdateMenuState(Game& game, float deltaTime);
    void DrawMenu(sf::RenderWindow& window);

 
    void OptionsState(Game& game); 
    void UpdateOptionsState(Game& game, float deltaTime);
    void DrawOptions(sf::RenderWindow& window);


    void StartDiffState(Game& game);
    void UpdateDiffState(Game& game, float deltaTime);
    void DrawDiff(sf::RenderWindow& window);


    void PauseState(Game& game);
    void PauseMenuUpdate(Game& game, float deltaTime);
    void DrawPause(sf::RenderWindow& window);


    void CinState(Game& game);
    void UpdateCinState(Game& game, float deltaTime);
    void DrawCin(Game& game, sf::RenderWindow& window);

    
    void GameOVERUI(Game& game); 

    void UpdatePlayingState(Game& game,float deltaTime, sf::RenderWindow& window);
    void UpdateGameOverState(Game& game, float deltaTime);
    void StartPlayinState(Game& game);
    void StartGameOverState(Game& game);


private:
 
    sf::Font font;
    sf::Text scoreText;
    sf::Text controlsHintText;
    sf::Text gameOverText;
    sf::Text gameOverScoreText;
    sf::Text restartControl;
    sf::Text BacktoMenuControl;
    sf::Text PlayerRecord;

 
    sf::Text menuText;
    sf::Text startText;
    sf::Text leaderboard;
    sf::Text exitText;


    sf::Text PauseMenuPlay;
    sf::Text PauseMenuMenu;
    sf::Text PauseMenuElement;


    sf::Text MainText;
    sf::Text BackToMenu;

    std::unordered_map<std::string, int> recordsTable;


    std::vector<std::pair<sf::Text, MenuItem>> menuItems;
    int menuIndex = 0;

    std::vector<std::pair<sf::Text, Options>> optionItems;
    int optionIndex = 0;
    sf::Text OptionsElem;

    std::vector<std::pair<sf::Text, Diffucult>> diffItems;
    int diffIndex = 0;
    int selectedDiffIndex = 2; 
    sf::Text diffElem;

 
    std::vector<std::pair<sf::Text, Cin>> cinItems;
    int cinIndex = 0;
    sf::Text cinElem;

  // UI NOW IS FRIENDLY TO GAME
    friend class Game;
};


