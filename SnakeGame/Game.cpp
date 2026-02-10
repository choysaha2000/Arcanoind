#include "Game.h"
#include <cassert>
#include <algorithm> 

// RAII INIT
Game::Game()
{
  // resourse load
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

    paddle.Init(*this);
    ball.Init(*this);

    ui.Init(*this);

    LoadRecords(); 
    SetGameSettings();
    PushGameState(GameState::Menu);
}

void Game::InitLevel()
{
    blocks.clear();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Block b;
            // colour set by position
            sf::Color color = (i % 2 == 0) ? sf::Color::Yellow : sf::Color::Green;
            b.Spawn({ startX + j * gapX, startY + i * gapY }, color);
            blocks.push_back(b);
        }
    }
}

void Game::Restart()
{
    paddle.Init(*this);
    ball.Init(*this);
    SetGameSettings();
    scoreEatenApples = 0; 
    PushGameState(GameState::Menu);
}

Game::~Game()
{
}


// UPDATE WINDOW ARGUMENT HERE CUZ THERE'S MOUSE INPUT 
void Game::Update(float deltaTime, sf::RenderWindow& window)
{
    GameState state = GetCurrentGameState();


    ui.Update(deltaTime, *this, window);

   
    if (state == GameState::Playing)
    {
        paddle.Update(deltaTime, *this, window);
        ball.Update(deltaTime, *this, window);

        // collision
        sf::Vector2f ballPos = ball.GetPosition();
        sf::FloatRect ballBounds = ball.GetBounds();
        sf::FloatRect paddleBounds = paddle.GetBounds();
            
        
        if (ballPos.x - BALL_RADIUS < 0) ball.BounceX();
        if (ballPos.x + BALL_RADIUS > SCREEN_WIDTH) ball.BounceX();
        if (ballPos.y - BALL_RADIUS < 0) ball.BounceY();

        // game over 
        if (ballPos.y + BALL_RADIUS > SCREEN_HEIGHT)
        {
            if (isSoundOn) LoseSound.play();
            isGameFinished = true;
            SwitchGameState(GameState::GameOver);
            return;
        }

        // paddle
        if (ballBounds.intersects(paddleBounds) && ball.GetVelocity().y > 0)
        {
            ball.BounceY();
            if (isSoundOn) HitSound.play();

            scoreEatenApples += 10;
            if (scoreEatenApples > playerRecord) playerRecord = scoreEatenApples;
            ui.scoreText.setString("Score: " + std::to_string(scoreEatenApples));
        }

        // blocks
        int activeBlocks = 0; 

        for (auto& block : blocks)
        {
            if (!block.IsDestroyed())
            {
                activeBlocks++;
                if (ballBounds.intersects(block.GetBounds()))
                {
                    block.Destroy();
                    ball.BounceY(); 

                    PlayHitSound();
                    AddScore(10); 
                }
            }
            else if (activeBlocks == 0)
            {
                SwitchGameState(GameState::Win);
            }
        }

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
        for (auto& block : blocks)
        {
            block.Draw(window);
        }
        ui.DrawPlaying(*this, window);


      
    }
    else if (state == GameState::GameOver)
    {
        window.draw(ui.gameOverText);
    }

    else if (state == GameState::Win) 
    {
        ui.DrawWin(window); 
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


// lifo
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
    case GameState::Playing:
        ui.StartPlayinState(*this);         
        break;
    case GameState::GameOver:
        ui.GameOVERUI(*this);
        break;
    case GameState::Menu:
        ui.StartMenuState();
        break;
    case GameState::LeaderBoard:
        ui.Init(*this);             
        break;
    case GameState::PauseMenu:
        ui.PauseState(*this);
        break;
    case GameState::Diffcult:
        ui.StartDiffState(*this);
        break;
    case GameState::Options:
        ui.OptionsState(*this);
        break;
    case GameState::Cin:
        ui.CinState(*this);
        break;

    case GameState::Win:
        ui.StartWinState(*this);
        break;
    default:
        break;
    }
}


// load and save resources
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



