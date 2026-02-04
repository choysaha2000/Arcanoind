#include "Game.h"
#include <cassert>
#include <algorithm> 

// RAII Inisitialization in Class constructor 
Game::Game()
{
   // load resourse
    if (playinStateMusic.openFromFile(RESOURCES_PATH + "PlayingState.ogg")) {
        playinStateMusic.setLoop(true);
    }

    assert(Sound1.loadFromFile(RESOURCES_PATH + "AppleEat.wav")); 
    assert(Sound2.loadFromFile(RESOURCES_PATH + "Lose.wav"));     

    HitSound.setBuffer(Sound1);
    LoseSound.setBuffer(Sound2);

    background.setFillColor(sf::Color::Black); 
    background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    background.setPosition(0.f, 0.f);

    paddle.Init();
    ball.Init();

    ui.InitUI(*this);

    LoadRecords(); 
    SetGameSettings();
    PushGameState(GameState::Menu);
}


void Game::Restart()
{
    paddle.Init();
    ball.Init();
    // ui.Init(*this); // UI пересоздавать не обязательно, только если сбросить текст
    SetGameSettings();
    // Сброс счета
    scoreEatenApples = 0; // Можно переименовать в currentScore
    PushGameState(GameState::Menu);
}

Game::~Game()
{
    // vectors have autoclear. 
}


// UPDATE HAVE WINDOW ARGUMENT CUZ THERE'S MOVING BY PLAYER'S MOUSE
// ALL UPDATE STATE
void Game::Update(float deltaTime, sf::RenderWindow& window)
{
    GameState gameState = GetCurrentGameState();
    switch (gameState)
    {
    case GameState::Playing:
        ui.UpdatePlayingState(*this, deltaTime, window);
        break;
    case GameState::GameOver:
        ui.UpdateGameOverState(*this, deltaTime);
        break;
    case GameState::Menu:
        ui.UpdateMenuState(*this, deltaTime);   
        break;
    case GameState::LeaderBoard:
        ui.UpdateLeaderBoardState(*this, deltaTime);
        break;
    case GameState::PauseMenu:
        ui.PauseMenuUpdate(*this, deltaTime);
        break;
    case GameState::Options:
        ui.UpdateOptionsState(*this, deltaTime);
        break;
    case GameState::Diffcult:
        ui.UpdateDiffState(*this, deltaTime);
        break;
    case GameState::Cin:
        ui.UpdateCinState(*this, deltaTime);
        break;
    }
}

void Game::Draw(sf::RenderWindow& window)
{
    window.clear();
    window.draw(background);
   
    GameState state = GetCurrentGameState();

    if (state == GameState::Playing)
    {
        paddle.Draw(window);
        ball.Draw(window);
        ui.DrawPlaying(*this, window);
    }
    else if (state == GameState::GameOver)
    {
        window.draw(ui.gameOverText);
    }
    else if (state == GameState::Menu)
    {
        ui.DrawMenu(window);
    }
    else if (state == GameState::LeaderBoard)
    {
        ui.UpdateLeaderboardGameOver(playerRecord, tempPlayerName);
        window.draw(ui.gameOverText);
        window.draw(ui.gameOverScoreText);
        ui.DrawLeaderBoard(window);
    }
    else if (state == GameState::PauseMenu)
    {
        ui.UpdateLeaderboardGameOver(playerRecord, tempPlayerName);
        window.draw(ui.gameOverText);
        window.draw(ui.gameOverScoreText);
        ui.DrawPause(window);
    }
    else if (state == GameState::Options)
    {
        ui.DrawOptions(window);
    }
    else if (state == GameState::Diffcult)
    {
        ui.DrawDiff(window);
    }
    else if (state == GameState::Cin)
    {
        ui.DrawCin(*this, window);
    }
}

void Game::SetGameSettings()
{
    
    isGameFinished = false;
    timeSinceGameFinish = 0.f;
    scoreEatenApples = 0;


    isEasy = gameMode & static_cast <uint32_t> (GameSettingBits::EasyMode);
    isMiddle = gameMode & static_cast <uint32_t> (GameSettingBits::MiddleMode);
    isHard = gameMode & static_cast <uint32_t> (GameSettingBits::HardMode);
    isVeryHard = gameMode & static_cast <uint32_t> (GameSettingBits::VeryHardMode);
    isHardcore = gameMode & static_cast <uint32_t> (GameSettingBits::HardcoreMode);


}


// LIFO
void Game::PushGameState(GameState state)
{
    GameState oldState = GetCurrentGameState();
    SwitchGameStateInternal(oldState, state);
    gameStateStack.push_back(state);
}

void Game::PopGameState()
{
    if (gameStateStack.empty()) return;
    GameState oldState = gameStateStack.back();
    gameStateStack.pop_back();
    GameState newState = GetCurrentGameState();
    SwitchGameStateInternal(oldState, newState);
}

void Game::SwitchGameState(GameState newState)
{
    if (!gameStateStack.empty()) {
        GameState oldState = gameStateStack.back();
        gameStateStack.pop_back();
        gameStateStack.push_back(newState);
        SwitchGameStateInternal(oldState, newState);
    } else {
        PushGameState(newState);
    }
}

GameState Game::GetCurrentGameState() const
{
    return gameStateStack.empty() ? GameState::None : gameStateStack.back();
}

void Game::SwitchGameStateInternal(GameState oldState, GameState newState)
{
    switch (newState)
    {
    case GameState::Playing: ui.StartPlayinState(*this); break;
    case GameState::GameOver: ui.GameOVERUI(*this); break;
    case GameState::Menu: ui.StartMenuState(); break;
    case GameState::LeaderBoard: ui.InitUI(*this); break;
    case GameState::PauseMenu: ui.PauseState(*this); break;
    case GameState::Diffcult: ui.StartDiffState(*this); break;
    case GameState::Options: ui.OptionsState(*this); break;
    case GameState::Cin: ui.CinState(*this); break;
    }
}



// SERIALIZATION AND DESEREILIZATION
void Game::LoadRecords()
{
    std::ifstream file(SAVE_FILE);
    if (file.is_open()) {
        ui.recordsTable.clear();
        std::string name;
        int score;
        while (file >> name >> score) {
            ui.recordsTable[name] = score;
        }
        file.close();
    } else {
        ui.InitializeLeaderBoard();
    }
}

void Game::SaveRecords()
{
    std::ofstream file(SAVE_FILE);
    if (file.is_open()) {
        for (const auto& entry : ui.recordsTable) {
            file << entry.first << " " << entry.second << std::endl;
        }
        file.close();
    }
}


//  REMOVE TO STATES
