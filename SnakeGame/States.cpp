#include "UI.h"
#include "Game.h"
#include <cassert>
#include <map>
#include <string>

// main update
void UI::Update(float deltaTime, Game& game, sf::RenderWindow& window)
{
    GameState state = game.GetCurrentGameState();
    switch (state)
    {
    case GameState::Menu:        UpdateMenuState(game, deltaTime); break;
    case GameState::Playing:     break;
    case GameState::PauseMenu:   PauseMenuUpdate(game, deltaTime); break;
    case GameState::Options:     UpdateOptionsState(game, deltaTime); break;
    case GameState::Diffcult:    UpdateDiffState(game, deltaTime); break;
    case GameState::Cin:         UpdateCinState(game, deltaTime); break;
    case GameState::LeaderBoard: UpdateLeaderBoardState(game, deltaTime); break;
    case GameState::GameOver: UpdateGameOverState(game, deltaTime); break;
    case GameState::Win: UpdateWinState(game, deltaTime); break;
    }
}

void UI::Draw(sf::RenderWindow& window)
{
 
}


void UI::Init(Game& game)
{
    assert(font.loadFromFile(RESOURCES_PATH + "Fonts/stencilbtrusbyme.otf"));

    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition(30.f, 20.f);

    PlayerRecord.setFont(font);
    PlayerRecord.setCharacterSize(20);
    PlayerRecord.setFillColor(sf::Color::Blue);
    PlayerRecord.setPosition(250.f, 20.f);
    PlayerRecord.setString("Record: " + std::to_string(game.GetRecord())); 

    BackToMenu.setFont(font);
    BackToMenu.setCharacterSize(30);
    BackToMenu.setFillColor(sf::Color::White);
    BackToMenu.setPosition(20.f,
        SCREEN_HEIGHT - BackToMenu.getGlobalBounds().height - 40.f);
    BackToMenu.setString("Press Backspace to back to menu");
}

std::string UI::GetLeaderboardString(const std::unordered_map<std::string, int>& records)
{
    std::multimap<int, std::string> sortedRecords;
    for (const auto& item : records)
    {
        sortedRecords.insert({ item.second, item.first });
    }

    std::string result = "===== LEADERBOARD =====\n";

    int position = 1;
    for (auto it = sortedRecords.rbegin(); it != sortedRecords.rend() && position <= 5; ++it, ++position)
    {
        int score = it->first;
        const std::string& name = it->second;
        std::string line = std::to_string(position) + ". " + name;

        long long dotsNeeded = dotsNeed - (long long)line.length();
        if (dotsNeeded > 0) {
            for (int i = 0; i < dotsNeeded; ++i)
                line += ".";
        }

        line += " " + std::to_string(score);
        result += line + "\n";
    }

    result += "=======================\n";
    return result;
}

void UI::InitializeLeaderBoard()
{
    recordsTable.clear();
    std::vector<std::string> names = { "Alice", "Bob", "Carol", "Dave", "Eve" };

    for (const auto& name : names)
    {
        int randomScore = 1 + (rand() % ScorePlayers);
        recordsTable[name] = randomScore;
    }
}

// pause state

void UI::PauseState(Game& game)
{
    PauseMenuPlay.setFont(font);
    PauseMenuPlay.setCharacterSize(30);
    PauseMenuPlay.setFillColor(sf::Color::White);
    PauseMenuPlay.setString("Space to play");
    PauseMenuPlay.setPosition(
        (SCREEN_WIDTH - PauseMenuPlay.getGlobalBounds().width) / 2.f,
        SCREEN_HEIGHT - PauseMenuPlay.getGlobalBounds().height - bottomPadding
    );

    PauseMenuMenu.setFont(font);
    PauseMenuMenu.setCharacterSize(30);
    PauseMenuMenu.setFillColor(sf::Color::White);
    PauseMenuMenu.setString("Backspace to menu");
    PauseMenuMenu.setPosition(
        (SCREEN_WIDTH - PauseMenuMenu.getGlobalBounds().width) / 2.f,
        PauseMenuPlay.getPosition().y - PauseMenuMenu.getGlobalBounds().height - spacing
    );

    PauseMenuElement.setFont(font);
    PauseMenuElement.setCharacterSize(50);
    PauseMenuElement.setFillColor(sf::Color::White);

    PauseMenuElement.setString("Your score: " + std::to_string(game.GetScore())); 

    PauseMenuElement.setPosition(
        (SCREEN_WIDTH - PauseMenuElement.getGlobalBounds().width) / 2.f,
        100.f
    );
}

void UI::PauseMenuUpdate(Game& game, float deltaTime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (game.IsGameFinished())
        {
            game.Restart();
            game.SwitchGameState(GameState::Playing);
        }
        else
        {
            game.SetGameFinished(false); 
     
            game.SwitchGameState(GameState::Playing);
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
    {
        game.SwitchGameState(GameState::Menu);
    }
}

void UI::DrawPause(sf::RenderWindow& window)
{
    window.draw(PauseMenuMenu);
    window.draw(PauseMenuPlay);
    window.draw(PauseMenuElement);
}

// cin state

void UI::CinState(Game& game)
{

    game.GetTempPlayerName() = ""; 

    cinItems.clear();
    sf::Text YES, NO;

    auto setTextParameters = [this](sf::Text& itemText, const std::string& title, int
        fontSize, sf::Color color = sf::Color::Transparent)
        {
            itemText.setString(title);
            itemText.setFont(font);
            itemText.setCharacterSize(fontSize);
            if (color != sf::Color::Transparent)
            {
                itemText.setFillColor(color);
            }
        };

    setTextParameters(cinElem, "PRESS YOUR NAME", 50, sf::Color::Red);
    cinElem.setPosition(
        (SCREEN_WIDTH - cinElem.getGlobalBounds().width) / 2.f,
        SCREEN_HEIGHT / 2.f - 100.f);

    setTextParameters(YES, "YES", 30, sf::Color::White);
    setTextParameters(NO, "NO", 30, sf::Color::White);

    YES.setPosition((SCREEN_WIDTH - YES.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f);
    NO.setPosition((SCREEN_WIDTH - NO.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 50.f);

    cinItems.push_back({ YES, Cin::YES });
    cinItems.push_back({ NO, Cin::NO });
    cinIndex = 0;
}

void UI::UpdateCinState(Game& game, float deltaTime)
{
    static float inputTimer = 0.0f;
    inputTimer += deltaTime;

    if (game.GetIsInputActive()) 
    {
        if (inputTimer > 0.2f && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            std::string finalName = game.GetTempPlayerName();
            if (finalName.empty()) finalName = "Unknown";

            UpdateLeaderboardGameOver(game.GetScore(), finalName); 
            game.SwitchGameState(GameState::PauseMenu);
            inputTimer = 0.0f;
        }
        return;
    }

    if (inputTimer < 0.2f) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        cinIndex++;
        if (cinIndex >= (int)cinItems.size()) cinIndex = 0;
        inputTimer = 0.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        cinIndex--;
        if (cinIndex < 0) cinIndex = (int)cinItems.size() - 1;
        inputTimer = 0.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        Cin selectedItem = cinItems[cinIndex].second;

        switch (selectedItem) {
        case Cin::YES:
            
            game.GetTempPlayerName() = "";
            break;

        case Cin::NO:
            UpdateLeaderboardGameOver(game.GetScore(), "Player");
            game.SwitchGameState(GameState::PauseMenu);
            break;
        }
        inputTimer = 0.0f;
    }
}

void UI::DrawCin(Game& game, sf::RenderWindow& window)
{
    if (game.GetIsInputActive())
    {
        cinElem.setString("ENTER NAME: " + game.GetTempPlayerName() + "_");
        cinElem.setFillColor(sf::Color::White);

        cinElem.setPosition(
            (SCREEN_WIDTH - cinElem.getGlobalBounds().width) / 2.f,
            SCREEN_HEIGHT / 2.f
        );
        window.draw(cinElem);
    }
    else
    {
        cinElem.setString("PRESS YOUR NAME");
        cinElem.setFillColor(sf::Color::Red);
        cinElem.setPosition(
            (SCREEN_WIDTH - cinElem.getGlobalBounds().width) / 2.f,
            SCREEN_HEIGHT / 2.f - 100.f);

        window.draw(cinElem);

        for (int i = 0; i < cinItems.size(); ++i) {
            sf::Text& text = cinItems[i].first;
            if (i == cinIndex) text.setFillColor(sf::Color::Yellow);
            else text.setFillColor(sf::Color::White);
            window.draw(text);
        }
    }
}

// option state

void UI::OptionsState(Game& game)
{
    optionItems.clear();
    sf::Text tSound, tMusic, tBack;

    auto setTextParameters = [this](sf::Text& itemText, const std::string& title, int
        fontSize, sf::Color color = sf::Color::Transparent)
        {
            itemText.setString(title);
            itemText.setFont(font);
            itemText.setCharacterSize(fontSize);
            if (color != sf::Color::Transparent)
            {
                itemText.setFillColor(color);
            }
        };

    setTextParameters(OptionsElem, "OPTIONS", 50, sf::Color::Red);
    OptionsElem.setPosition(
        (SCREEN_WIDTH - OptionsElem.getGlobalBounds().width) / 2.f,
        SCREEN_HEIGHT / 2.f - 100.f);

    setTextParameters(tSound, "SOUND", 30, sf::Color::White);
    setTextParameters(tMusic, "MUSIC", 30, sf::Color::White);
    setTextParameters(tBack, "BACK", 30, sf::Color::White);

    tSound.setPosition((SCREEN_WIDTH - tSound.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f);
    tMusic.setPosition((SCREEN_WIDTH - tMusic.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 50.f);
    tBack.setPosition((SCREEN_WIDTH - tBack.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 100.f);

    optionItems.push_back({ tSound, Options::Sound });
    optionItems.push_back({ tMusic, Options::Music });
    optionItems.push_back({ tBack, Options::Back });

    optionIndex = 0;
}

void UI::UpdateOptionsState(Game& game, float deltaTime)
{
    static float inputTimer = 0.0f;
    inputTimer += deltaTime;
    if (inputTimer < 0.2f) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        optionIndex++;
        if (optionIndex >= (int)optionItems.size()) optionIndex = 0;
        inputTimer = 0.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        optionIndex--;
        if (optionIndex < 0) optionIndex = (int)optionItems.size() - 1;
        inputTimer = 0.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        Options selectedItem = optionItems[optionIndex].second;

        switch (selectedItem) {
        case Options::Sound:
       
            break;
        case Options::Music:
    
            if (game.IsMusicOn()) game.PlayMusic(); else game.StopMusic();
            break;
        case Options::Back:
            game.SwitchGameState(GameState::Menu);
            break;
        }
        inputTimer = 0.0f;
    }
}

void UI::DrawOptions(sf::RenderWindow& window)
{
    window.draw(OptionsElem);
    for (int i = 0; i < optionItems.size(); ++i) {
        sf::Text& text = optionItems[i].first;
        if (i == optionIndex) text.setFillColor(sf::Color::Yellow);
        else text.setFillColor(sf::Color::White);
        window.draw(text);
    }
}

// menu state

void UI::StartMenuState()
{
    menuItems.clear();
    sf::Text tStart, tDiff, tOptions, tLeader, tExit;

    auto setTextParameters = [this](sf::Text& itemText, const std::string& title, int
        fontSize, sf::Color color = sf::Color::Transparent)
        {
            itemText.setString(title);
            itemText.setFont(font);
            itemText.setCharacterSize(fontSize);
            if (color != sf::Color::Transparent) itemText.setFillColor(color);
        };

    setTextParameters(menuText, "ARKANOID", 50, sf::Color::Green);
    menuText.setPosition(
        (SCREEN_WIDTH - menuText.getGlobalBounds().width) / 2.f,
        SCREEN_HEIGHT / 2.f - 100.f);

    setTextParameters(tStart, "START", 30, sf::Color::White);
    setTextParameters(tDiff, "DIFFICULTY", 30, sf::Color::White);
    setTextParameters(tOptions, "OPTIONS", 30, sf::Color::White);
    setTextParameters(tLeader, "LEADERBOARD", 30, sf::Color::White);
    setTextParameters(tExit, "EXIT", 30, sf::Color::White);

    tStart.setPosition((SCREEN_WIDTH - tStart.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f);
    tDiff.setPosition((SCREEN_WIDTH - tDiff.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 50.f);
    tOptions.setPosition((SCREEN_WIDTH - tOptions.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 100.f);
    tLeader.setPosition((SCREEN_WIDTH - tLeader.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 150.f);
    tExit.setPosition((SCREEN_WIDTH - tExit.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 200.f);

    menuItems.push_back({ tStart, MenuItem::Start });
    menuItems.push_back({ tDiff, MenuItem::Diffucult });
    menuItems.push_back({ tOptions, MenuItem::Options });
    menuItems.push_back({ tLeader, MenuItem::Leaderboard });
    menuItems.push_back({ tExit, MenuItem::Exit });

    menuIndex = 0;
}

void UI::UpdateMenuState(Game& game, float deltaTime)
{
    static float inputTimer = 0.0f;
    inputTimer += deltaTime;
    if (inputTimer < 0.2f) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        menuIndex++;
        if (menuIndex >= (int)menuItems.size()) menuIndex = 0;
        inputTimer = 0.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        menuIndex--;
        if (menuIndex < 0) menuIndex = (int)menuItems.size() - 1;
        inputTimer = 0.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        MenuItem selectedItem = menuItems[menuIndex].second;
        switch (selectedItem) {
        case MenuItem::Start: game.SwitchGameState(GameState::Playing); break;
        case MenuItem::Leaderboard: game.SwitchGameState(GameState::LeaderBoard); break;
        case MenuItem::Options: game.SwitchGameState(GameState::Options); break;
        case MenuItem::Diffucult: game.SwitchGameState(GameState::Options); break;
        case MenuItem::Exit: exit(0); break;
        }
        inputTimer = 0.0f;
    }
}

void UI::DrawMenu(sf::RenderWindow& window)
{
    window.draw(menuText);
    for (int i = 0; i < menuItems.size(); ++i) {
        sf::Text& text = menuItems[i].first;
        if (i == menuIndex) text.setFillColor(sf::Color::Yellow);
        else text.setFillColor(sf::Color::White);
        window.draw(text);
    }
}



void UI::StartDiffState(Game& game)
{

}

void UI::UpdateDiffState(Game& game, float deltaTime)
{
   
}

void UI::DrawDiff(sf::RenderWindow& window)
{
  
}

// leaderboard 

void UI::UpdateLeaderBoardState(Game& game, float deltaTime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
    {
        game.SwitchGameState(GameState::Menu);
    }
}

void UI::DrawLeaderBoard(sf::RenderWindow& window)
{
    window.draw(BackToMenu);
}

// gameover state

void UI::StartGameOverState(Game& game)
{
    game.StopMusic();
    game.SetGameFinished(true);
    game.SetTimeSinceGameFinish(0.f);
}


void UI::GameOVERUI(Game& game)
{
    game.StopMusic();
    game.SetGameFinished(true);
    game.SetTimeSinceGameFinish(0.f);

    gameOverText.setString("Game Over");
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(100);
    gameOverText.setFillColor(sf::Color::White);

    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);
    gameOverText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
}

void UI::UpdateGameOverState(Game& game, float deltaTime)
{
    if (game.GetGameFinishTime() <= PAUSE_LENGTH)
    {
        game.AddGameFinishTime(deltaTime);
        game.SetBackgroundColor(sf::Color::Red);
    }
    else
    {
        game.SetBackgroundColor(sf::Color::Black);
        if (game.GetScore() >= 10)
            game.SwitchGameState(GameState::Cin);
        else
            game.SwitchGameState(GameState::PauseMenu);
    }
}

// leaderboard state

void UI::UpdateLeaderboardGameOver(int playerScore, std::string name)
{
    recordsTable[name] = playerScore;
    std::string leaderBoardText = GetLeaderboardString(recordsTable);
    gameOverText.setString(leaderBoardText);
    gameOverText.setCharacterSize(18);
    gameOverText.setFont(font);
    gameOverText.setOrigin(0.f, 0.f);
    gameOverText.setPosition(50.f, 100.f);
}


void UI::DrawPlaying(Game& game, sf::RenderWindow& window)
{
    window.draw(scoreText);
    window.draw(PlayerRecord);
}

void UI::StartPlayinState(Game& game)
{
    game.GetPaddle().Init(game); 
    game.GetBall().Init(game);
    game.InitLevel();

    // game.SetScore(0);
    scoreText.setString("Score: 0"); 

    if (game.IsMusicOn()) {
        game.PlayMusic();
    }
}

void UI::StartWinState(Game& game)
{

    game.StopMusic();
    game.SetGameFinished(true);
    game.SetTimeSinceGameFinish(0.f);

    winText.setFont(font);
    winText.setString("YOU WIN!");
    winText.setCharacterSize(80);
    winText.setFillColor(sf::Color::White);

    sf::FloatRect textRect = winText.getLocalBounds();
    winText.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);
    winText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

 

}

void UI::UpdateWinState(Game& game, float deltaTime)
{

    if (game.GetGameFinishTime() <= PAUSE_LENGTH)
    {
        game.AddGameFinishTime(deltaTime);
        game.SetBackgroundColor(sf::Color::Green);
    }
    else
    {
        game.SetBackgroundColor(sf::Color::Black);
        if (game.GetScore() >= 100)
            game.SwitchGameState(GameState::Cin);
        else
            game.SwitchGameState(GameState::PauseMenu);
    }
  
}

void UI::DrawWin(sf::RenderWindow& window)
{
    window.draw(winText);
}
