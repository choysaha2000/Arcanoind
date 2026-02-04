#include "game.h"
#include "UI.h"
#include <cassert>
#include <map>
#include <string>

// ------------UI 
void UI::InitUI(Game& game)
{
    assert(font.loadFromFile(RESOURCES_PATH + "\\Fonts\\stencilbtrusbyme.otf"));

    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition(30.f, 20.f);

    PlayerRecord.setFont(font);
    PlayerRecord.setCharacterSize(20);
    PlayerRecord.setFillColor(sf::Color::Blue);
    PlayerRecord.setPosition(250.f, 20.f);
    PlayerRecord.setString("Record: " + std::to_string(game.playerRecord));

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
        auto dotsNeeded = dotsNeed - line.length();
        for (int i = 0; i < dotsNeeded; ++i)
            line += ".";
        line += " " + std::to_string(score);

        result += line + "\n";
    }

    result += "=======================\n";
    return result;
}
void UI::InitializeLeaderBoard()
{
    recordsTable.clear();
    std::vector<std::string> names = {
        "Alice", "Bob", "Carol", "Dave", "Eve"
    };

    for (const auto& name : names)
    {
        int randomScore = 1 + (rand() % ScorePlayers);
        recordsTable[name] = randomScore;
    }
}
// -------------

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


    PauseMenuElement.setString("Your score: " + std::to_string(game.scoreEatenApples));

    PauseMenuElement.setPosition(
        (SCREEN_WIDTH - PauseMenuElement.getGlobalBounds().width) / 2.f,
        100.f
    );
}

void UI::PauseMenuUpdate(Game& game, float deltaTime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (game.isGameFinished) // friends status with game.class
        {
            game.Restart();
            game.SwitchGameState(GameState::Playing);
        }
        else
        {
            game.isGameFinished = false;
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

// STD CIN STATE

void UI::CinState(Game& game)
{
    game.isInputActive = false;
    game.tempPlayerName = "";

    cinItems.clear();
    sf::Text YES, NO;

    // LYMBDA CMP
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

    if (game.isInputActive)
    {
        if (inputTimer > 0.2f && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            std::string finalName = game.tempPlayerName;
            if (finalName.empty()) finalName = "Unknown";

            UpdateLeaderboardGameOver(game.scoreEatenApples, finalName);
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
            game.isInputActive = true;
            game.tempPlayerName = "";
            break;

        case Cin::NO:
            UpdateLeaderboardGameOver(game.scoreEatenApples, "Player");
            game.SwitchGameState(GameState::PauseMenu);
            break;
        }
        inputTimer = 0.0f;
    }
}

void UI::DrawCin(Game& game, sf::RenderWindow& window)
{
    if (game.isInputActive)
    {
      
        cinElem.setString("ENTER NAME: " + game.tempPlayerName + "_");
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

// OPTIONS STATE
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
        if (optionIndex >= (int)optionItems.size()) 
            optionIndex = 0;
        inputTimer = 0.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        optionIndex--;
        if (optionIndex < 0)
            optionIndex = (int)optionItems.size() - 1; 
        inputTimer = 0.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {

        Options selectedItem = optionItems[optionIndex].second;

        switch (selectedItem) {
        case Options::Sound:
            game.isSoundOn = !game.isSoundOn;
            break;
        case Options::Music:
            game.isMusicOn = !game.isMusicOn;
            if (game.isMusicOn) game.playinStateMusic.play();
            else game.playinStateMusic.stop();
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

        if (i == optionIndex) {
            text.setFillColor(sf::Color::Yellow);
        }
        else {
            text.setFillColor(sf::Color::White);
        }
        window.draw(text);
    }
}

// MENU STATE

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
            if (color != sf::Color::Transparent)
            {
                itemText.setFillColor(color);
            }
        };

    setTextParameters(menuText, "ARCANOID", 50, sf::Color::Green);
    menuText.setPosition(
        (SCREEN_WIDTH - menuText.getGlobalBounds().width) / 2.f,
        SCREEN_HEIGHT / 2.f - 100.f);

    setTextParameters(tStart, "START", 30, sf::Color::White);
    setTextParameters(tDiff, "DIFFUCULT", 30, sf::Color::White);
    setTextParameters(tOptions, "OPTION", 30, sf::Color::White);
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
        if (menuIndex >= (int)menuItems.size())
            menuIndex = 0;
        inputTimer = 0.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        menuIndex--;
        if (menuIndex < 0)
            menuIndex = (int)menuItems.size() - 1;
        inputTimer = 0.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {

        MenuItem selectedItem = menuItems[menuIndex].second;

        switch (selectedItem) {
        case MenuItem::Start:
            game.SwitchGameState(GameState::Playing);
            break;
        case MenuItem::Leaderboard:
            game.SwitchGameState(GameState::LeaderBoard);
            break;
        case MenuItem::Options:
            game.SwitchGameState(GameState::Options);
            break;
        case MenuItem::Diffucult:
            game.SwitchGameState(GameState::Diffcult);
            break;
        case MenuItem::Exit:
            exit(0);
            break;
        }
        inputTimer = 0.0f;
    }
}

void UI::DrawMenu(sf::RenderWindow& window)
{
    window.draw(menuText);

    for (int i = 0; i < menuItems.size(); ++i) {
        sf::Text& text = menuItems[i].first;

        if (i == menuIndex) {
            text.setFillColor(sf::Color::Yellow);
        }
        else {
            text.setFillColor(sf::Color::White);
        }
        window.draw(text);
    }
}

// DIFF STATE

void UI::StartDiffState(Game& game)
{
    diffItems.clear();

    sf::Text  tEasy, tEasy2, tMiddle, tHard, tVeryHard, tBack;

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

    setTextParameters(diffElem, "DIFFICULTY CHANGE", 50, sf::Color::Red);
    diffElem.setPosition(
        (SCREEN_WIDTH - diffElem.getGlobalBounds().width) / 2.f,
        SCREEN_HEIGHT / 2.f - 100.f);

    setTextParameters(tEasy, "EASY", 30, sf::Color::White);
    setTextParameters(tEasy2, "MORE HARD THAN EASY", 30, sf::Color::White);
    setTextParameters(tMiddle, "MIDDLE", 30, sf::Color::White);
    setTextParameters(tHard, "HARD", 30, sf::Color::White);
    setTextParameters(tVeryHard, "VERY HARD", 30, sf::Color::White);
    setTextParameters(tBack, "BACK", 30, sf::Color::White);

    tEasy.setPosition((SCREEN_WIDTH - tEasy.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f);
    tEasy2.setPosition((SCREEN_WIDTH - tEasy2.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 50.f);
    tMiddle.setPosition((SCREEN_WIDTH - tMiddle.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 100.f);
    tHard.setPosition((SCREEN_WIDTH - tHard.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 150.f);
    tVeryHard.setPosition((SCREEN_WIDTH - tVeryHard.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 200.f);
    tBack.setPosition((SCREEN_WIDTH - tBack.getGlobalBounds().width) / 2.f, SCREEN_HEIGHT / 2.f + 300.f);

    diffItems.push_back({ tEasy, Diffucult::Easy });
    diffItems.push_back({ tEasy2, Diffucult::Easy2 });
    diffItems.push_back({ tMiddle, Diffucult::Middle });
    diffItems.push_back({ tHard, Diffucult::Hard });
    diffItems.push_back({ tVeryHard, Diffucult::VeryHard });
    diffItems.push_back({ tBack, Diffucult::Back });

    if (game.gameMode & (uint32_t)GameSettingBits::EasyMode) selectedDiffIndex = 0;
    else if (game.gameMode & (uint32_t)GameSettingBits::MiddleMode) selectedDiffIndex = 1;
    else if (game.gameMode & (uint32_t)GameSettingBits::HardMode) selectedDiffIndex = 2;
    else if (game.gameMode & (uint32_t)GameSettingBits::VeryHardMode) selectedDiffIndex = 3;
    else if (game.gameMode & (uint32_t)GameSettingBits::HardcoreMode) selectedDiffIndex = 4;

    diffIndex = 0;
}

void UI::UpdateDiffState(Game& game, float deltaTime)
{
    static float inputTimer = 0.0f;
    inputTimer += deltaTime;
    if (inputTimer < 0.2f) return;

    bool anyKeyPressed = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        diffIndex++;
        if (diffIndex >= (int)diffItems.size())
            diffIndex = 0;
        anyKeyPressed = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        diffIndex--;
        if (diffIndex < 0)
            diffIndex = (int)diffItems.size() - 1;
        anyKeyPressed = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        Diffucult selectedItem = diffItems[diffIndex].second;

        if (selectedItem == Diffucult::Back)
        {
            game.SwitchGameState(GameState::Menu);
        }
        else
        {
            selectedDiffIndex = diffIndex;
            game.gameMode = 0;

            switch (selectedItem)
            {
            case Diffucult::Easy: game.gameMode |= (uint32_t)GameSettingBits::EasyMode; break;
            case Diffucult::Easy2: game.gameMode |= (uint32_t)GameSettingBits::MiddleMode; break;
            case Diffucult::Middle: game.gameMode |= (uint32_t)GameSettingBits::HardMode; break;
            case Diffucult::Hard: game.gameMode |= (uint32_t)GameSettingBits::VeryHardMode; break;
            case Diffucult::VeryHard: game.gameMode |= (uint32_t)GameSettingBits::HardcoreMode; break;
            }
            game.SetGameSettings();
            anyKeyPressed = true;
        }
    }
    if (anyKeyPressed) {
        inputTimer = 0.0f;
    }
}

void UI::DrawDiff(sf::RenderWindow& window)
{
    window.draw(diffElem);

    for (int i = 0; i < diffItems.size(); ++i) {
        sf::Text& text = diffItems[i].first;

        if (i == diffIndex) {
            text.setFillColor(sf::Color::Yellow);
        }
        else if (i == selectedDiffIndex)
        {
            text.setFillColor(sf::Color::Red);
        }
        else {
            text.setFillColor(sf::Color::White);
        }
        window.draw(text);
    }
}

// LEADERBOARD STATE
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

// 

void UI::UpdatePlayingState(Game& game, float deltaTime, sf::RenderWindow& window)
{
    if (game.isGameFinished) {
        UpdateGameOverState(game,deltaTime);
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Pause)) {
        game.isGamePause = true;
        game.SwitchGameState(GameState::PauseMenu);
        return;
    }

    // 1 tick. coordinate update by input information
    game.paddle.Update(deltaTime, window);


    game.ball.Update(deltaTime);

    // collision logic
    sf::Vector2f ballPos = game.ball.GetPosition();
    sf::FloatRect ballBounds = game.ball.GetBounds();
    sf::FloatRect paddleBounds = game.paddle.GetBounds();

    // wall

    // left
    if (ballPos.x - BALL_RADIUS < 0) game.ball.BounceX();
    // right
    if (ballPos.x + BALL_RADIUS > SCREEN_WIDTH) game.ball.BounceX();
    // top
    if (ballPos.y - BALL_RADIUS < 0) game.ball.BounceY();

    // down 
    if (ballPos.y + BALL_RADIUS > SCREEN_HEIGHT) {
        if (game.isSoundOn) game.LoseSound.play();
        game.SwitchGameState(GameState::GameOver);
        return;
    }

    // paddle
    if (ballBounds.intersects(paddleBounds))
    {
        // checking that ball is flying to down (stack with paddle checK)
        if (game.ball.GetVelocity().y > 0)
        {
            game.ball.BounceY();
            if (game.isSoundOn) game.HitSound.play();


            int points = 10;

            if (game.isHard) points = 20;

            game.scoreEatenApples += points;
            if (game.scoreEatenApples > game.playerRecord) game.playerRecord = game.scoreEatenApples;

            scoreText.setString("Score: " + std::to_string(game.scoreEatenApples));
        }
    }
}
void UI::DrawPlaying(Game& game, sf::RenderWindow& window)
{
    window.draw(scoreText);
    window.draw(PlayerRecord);
}
void UI::StartPlayinState(Game& game)
{
    // After lose, paddle and ball reInit their position
    game.paddle.Init();
    game.ball.Init();


    game.scoreEatenApples = 0;
    scoreText.setString("Score: " + std::to_string(game.scoreEatenApples));

    if (game.isMusicOn) {
        if (game.playinStateMusic.getStatus() != sf::Music::Playing)
            game.playinStateMusic.play();
    }
}

void UI::StartGameOverState(Game& game)
{
    game.playinStateMusic.stop();
    game.isGameFinished = true;
    game.timeSinceGameFinish = 0.f;
}
void UI::GameOVERUI(Game& game)
{
    game.playinStateMusic.stop();
    game.isGameFinished = true;
    game.timeSinceGameFinish = 0.f;

    gameOverText.setString("Game Over");
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(100);
    gameOverText.setFillColor(sf::Color::White);


    sf::FloatRect textRect = gameOverText.getLocalBounds();

    gameOverText.setOrigin(
        textRect.left + textRect.width / 2.f,
        textRect.top + textRect.height / 2.f
    );

    // set text in center
    gameOverText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
}
void UI::UpdateGameOverState(Game& game, float deltaTime)
{
    game.playinStateMusic.stop();
    if (game.timeSinceGameFinish <= PAUSE_LENGTH)
    {
        game.timeSinceGameFinish += deltaTime;
        game.background.setFillColor(sf::Color::Red);
    }
    else
    {
        game.background.setFillColor(sf::Color::Black);
        if (game.scoreEatenApples >= 10)
            game.SwitchGameState(GameState::Cin);
        else
            game.SwitchGameState(GameState::PauseMenu);
    }
}
void UI::UpdateLeaderboardGameOver(int playerScore, std::string name)
{

    const std::string PLAYER_NAME = name;
    recordsTable[PLAYER_NAME] = playerScore;


    std::string leaderBoardText = GetLeaderboardString(recordsTable);
    gameOverText.setString(leaderBoardText);
    gameOverText.setCharacterSize(18);
    gameOverText.setFont(font);
    gameOverText.setOrigin(0.f, 0.f);
    gameOverText.setPosition(50.f, 100.f);

}