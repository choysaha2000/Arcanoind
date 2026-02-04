#pragma once
#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h" 
#include "UI.h"
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream> 
#include "Math.h"

const std::string SAVE_FILE = "records.txt";

enum class GameSettingBits
{
    EasyMode = 1 << 0,
    MiddleMode = 1 << 1,
    HardMode = 1 << 2,
    VeryHardMode = 1 << 3,
    HardcoreMode = 1 << 4,
};

enum class GameState
{
    None = 0,
    Menu,
    Playing,
    GameOver,
    LeaderBoard,
    PauseMenu,
    Options,
    Diffcult,
    Cin,
};

class Game
{
public:

    Game();
    ~Game();

 
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;


    void Update(float deltaTime, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);

  
    void PushGameState(GameState state);
    void PopGameState();
    void SwitchGameState(GameState newState);
    GameState GetCurrentGameState() const;
    void Restart(); 

    //SET AND GET FOR APP
    bool IsInputActive() const { return isInputActive; }
    std::string& GetTempPlayerName() { return tempPlayerName; }
    UI& GetUI() { return ui; }

    
    void SetGameSettings();


private:
    // IN LOGIC
    void SwitchGameStateInternal(GameState oldState, GameState newState);
  
    void LoadRecords();
    void SaveRecords();

private:
  
    Paddle paddle;
    Ball ball;


    UI ui;
    math Math; 

    sf::RectangleShape background;
    std::vector<GameState> gameStateStack;

    uint32_t gameMode = 0;

    bool isGameFinished = false;
    float timeSinceGameFinish = 0.f;


    int scoreEatenApples = 0;
    int playerRecord = 0;

    sf::Music playinStateMusic;
    sf::SoundBuffer Sound1;
    sf::Sound HitSound;

    sf::SoundBuffer Sound2; 
    sf::Sound LoseSound;

    bool isMusicOn = true;
    bool isSoundOn = true;
    bool isGamePause = false;
    bool isInputActive = false;

    bool isEasy = false;
    bool isMiddle = false;
    bool isHard = false;
    bool isVeryHard = false;
    bool isHardcore = false;

    std::string tempPlayerName = "";

// FRIENDS
    friend class Application;
    friend class UI;
};
