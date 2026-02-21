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
#include <vector> 
#include "Math.h"
#include "Block.h"
#include <memory>
#include "SolidBlock.h"


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
    Win
};

class Game
{
public:
    Game();
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    // main cicle
    void Update(float deltaTime, sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);
    void Restart();
    void SetGameSettings();
    void InitLevel();

    // state machines
    void PushGameState(GameState state);
    void PopGameState();
    void SwitchGameState(GameState newState);
    GameState GetCurrentGameState() const;


   //main gets and sets
    Paddle& GetPaddle() { return paddle; }
    Ball& GetBall() { return ball; }
    UI& GetUI() { return ui; }

    void InitPaddle() { paddle.Init(*this); }
    void InitBall() { ball.Init(*this); }

    //main gets and sets
    bool GetIsInputActive() const { return isInputActive; }
    void SetIsInputAcitve(bool val) { isInputActive = val; }
    std::string& GetTempPlayerName() { return tempPlayerName; }
    void SetTempPlayerName(const std::string& name) { tempPlayerName = name; }

    // records get and set
    int GetScore() const { return scoreEatenApples; }
    void AddScore(int points) {
        scoreEatenApples += points;
        if (scoreEatenApples > playerRecord) playerRecord = scoreEatenApples;
    }
    int GetRecord() const { return playerRecord; }

    // game over get and set
    bool IsGameFinished() const { return isGameFinished; }
    void SetGameFinished(bool value) { isGameFinished = value; }

    float GetGameFinishTime() const { return timeSinceGameFinish; }
    void AddGameFinishTime(float delta) { timeSinceGameFinish += delta; }
    void SetTimeSinceGameFinish(float val) { timeSinceGameFinish = val; } 

    // settings get and set
    bool IsSoundOn() const { return isSoundOn; }
    bool IsMusicOn() const { return isMusicOn; }
    bool IsHard() const { return isHard; }

    // audio get and set
    void PlayHitSound() { if (isSoundOn) HitSound.play(); }
    void PlayLoseSound() { if (isSoundOn) LoseSound.play(); }
    void PlayMusic() { if (isMusicOn && playinStateMusic.getStatus() != sf::Music::Playing) playinStateMusic.play(); }
    void StopMusic() { playinStateMusic.stop(); }

    
    void SetBackgroundColor(sf::Color color) { background.setFillColor(color); }


    //block
    std::vector<std::unique_ptr<Block>>& GetBlocks() { return blocks; }




private:
    // local logic
    void SwitchGameStateInternal(GameState oldState, GameState newState);

    void LoadRecords();
    void SaveRecords();

private:
    
    Paddle paddle;
    Ball ball;
    UI ui;

  
    math Math;
    sf::RectangleShape background;
    sf::RectangleShape wallTop;



    uint32_t gameMode = 0;
    bool isGameFinished = false;
    float timeSinceGameFinish{};
    std::vector<GameState> gameStateStack;

    int scoreEatenApples = 0; 
    int playerRecord = 0;    

    
    sf::Music playinStateMusic;
    sf::SoundBuffer Sound1, Sound2;
    sf::Sound HitSound, LoseSound;


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

    friend class Application;


    std::vector<std::unique_ptr<Block>> blocks;

};
