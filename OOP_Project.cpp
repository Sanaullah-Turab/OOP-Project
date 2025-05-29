#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <cstdlib>
#include "Frog.h"
#include "Global.h"
#include "Car.h"
#include "Log.h"
#include "PlayerManager.h"

using namespace sf;
using namespace std;

void StartScreen(RenderWindow &window);
void PlayerSelectScreen(RenderWindow &window, int &numPlayers);
void StartGame(RenderWindow &window, int numPlayers);
void PlayerMovement(Frog &player, IntRect &texRect, int &keyCooldown, int &keyTimer, Sound &sound, int playerIndex);
void CarSpawner(int &carSpawnTimer, int &carSpawnCooldown, vector<Car> &cars);
void ObjectRemover(vector<Car> &cars);
void ObjectRemover(vector<Log> &logs);
void LogSpawner(int &logSpawnTimer, int &logSpawnCooldown, vector<Log> &logs);
bool GameOver(RenderWindow &window, PlayerManager &playerManager);

int main()
{
    srand(time(0));
    RenderWindow window(VideoMode(800, 600), "OOP Project", Style::Default);
    window.setFramerateLimit(30);

    int numPlayers = 1;

    StartScreen(window);
    PlayerSelectScreen(window, numPlayers);
    StartGame(window, numPlayers);

    return 0;
}

void StartScreen(RenderWindow &window)
{
    Texture backgroundImage;
    Sprite backgroundSprite;
    if (!backgroundImage.loadFromFile("Resources/Images/StartScreen.png"))
        cout << "could not load main menu image" << endl;
    backgroundSprite.setTexture(backgroundImage);

    Font mainMenuFont;
    if (!mainMenuFont.loadFromFile("Resources/Fonts/Goldman-Regular.ttf"))
        cout << "could not load main menu font" << endl;
    Text startGameText;
    startGameText.setCharacterSize(50);
    startGameText.setFillColor(Color::White);
    startGameText.setFont(mainMenuFont);
    Text creditsText = startGameText;
    Text exitText = startGameText;

    startGameText.setString("Start Game");
    startGameText.setPosition((WINDOW_WIDTH - startGameText.getGlobalBounds().width) / 2, 300);
    creditsText.setString("Credits");
    creditsText.setPosition((WINDOW_WIDTH - creditsText.getGlobalBounds().width) / 2, 370);
    exitText.setString("Exit");
    exitText.setPosition((WINDOW_WIDTH - exitText.getGlobalBounds().width) / 2, 440);

    SoundBuffer menuSelectSoundBuffer;
    menuSelectSoundBuffer.loadFromFile("Resources/Sounds/gta-menu.wav");
    SoundBuffer menuMoveSoundBuffer;
    menuMoveSoundBuffer.loadFromFile("Resources/Sounds/gta-menu_2.wav");
    Sound sound;

    int choice = 0;
    int keyTimer = 0;
    int keyCooldown = 10;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (Keyboard::isKeyPressed(Keyboard::Enter) && choice == 0)
            {
                sound.setBuffer(menuSelectSoundBuffer);
                sound.play();
                sleep(milliseconds(300));
                return;
            }
            else if (Keyboard::isKeyPressed(Keyboard::Enter) && choice == 1)
            {
                sound.setBuffer(menuSelectSoundBuffer);
                sound.play();
            }
            else if (Keyboard::isKeyPressed(Keyboard::Enter) && choice == 2)
            {
                sound.setBuffer(menuSelectSoundBuffer);
                sound.play();
                sleep(milliseconds(300));
                window.close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Up) && keyTimer >= keyCooldown)
        {
            sound.setBuffer(menuMoveSoundBuffer);
            sound.play();
            choice--;
            if (choice < 0)
                choice = 2;
            keyTimer = 0;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down) && keyTimer >= keyCooldown)
        {
            sound.setBuffer(menuMoveSoundBuffer);
            sound.play();
            choice++;
            if (choice > 2)
                choice = 0;
            keyTimer = 0;
        }
        keyTimer++;
        switch (choice)
        {
        case 0:
            startGameText.setFillColor(Color::Red);
            creditsText.setFillColor(Color::White);
            exitText.setFillColor(Color::White);
            break;
        case 1:
            startGameText.setFillColor(Color::White);
            creditsText.setFillColor(Color::Red);
            exitText.setFillColor(Color::White);
            break;
        case 2:
            startGameText.setFillColor(Color::White);
            creditsText.setFillColor(Color::White);
            exitText.setFillColor(Color::Red);
            break;
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(startGameText);
        window.draw(creditsText);
        window.draw(exitText);
        window.display();
    }
}

void PlayerSelectScreen(RenderWindow &window, int &numPlayers)
{
    Texture backgroundImage;
    Sprite backgroundSprite;
    if (!backgroundImage.loadFromFile("Resources/Images/StartScreen.png"))
        cout << "could not load player select image" << endl;
    backgroundSprite.setTexture(backgroundImage);

    Font font;
    if (!font.loadFromFile("Resources/Fonts/Goldman-Regular.ttf"))
        cout << "could not load font" << endl;

    Text titleText;
    titleText.setCharacterSize(60);
    titleText.setFillColor(Color::White);
    titleText.setFont(font);
    titleText.setString("Select Players");
    titleText.setPosition((WINDOW_WIDTH - titleText.getGlobalBounds().width) / 2, 150);

    Text onePlayerText;
    onePlayerText.setCharacterSize(50);
    onePlayerText.setFillColor(Color::White);
    onePlayerText.setFont(font);
    onePlayerText.setString("1 Player");
    onePlayerText.setPosition((WINDOW_WIDTH - onePlayerText.getGlobalBounds().width) / 2, 300);

    Text twoPlayerText;
    twoPlayerText.setCharacterSize(50);
    twoPlayerText.setFillColor(Color::White);
    twoPlayerText.setFont(font);
    twoPlayerText.setString("2 Players");
    twoPlayerText.setPosition((WINDOW_WIDTH - twoPlayerText.getGlobalBounds().width) / 2, 370);

    SoundBuffer menuSelectSoundBuffer;
    menuSelectSoundBuffer.loadFromFile("Resources/Sounds/gta-menu.wav");
    SoundBuffer menuMoveSoundBuffer;
    menuMoveSoundBuffer.loadFromFile("Resources/Sounds/gta-menu_2.wav");
    Sound sound;

    int choice = 0;
    int keyTimer = 0;
    int keyCooldown = 10;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (Keyboard::isKeyPressed(Keyboard::Enter))
            {
                sound.setBuffer(menuSelectSoundBuffer);
                sound.play();
                sleep(milliseconds(300));
                numPlayers = choice + 1;
                return;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Up) && keyTimer >= keyCooldown)
        {
            sound.setBuffer(menuMoveSoundBuffer);
            sound.play();
            choice--;
            if (choice < 0)
                choice = 1;
            keyTimer = 0;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down) && keyTimer >= keyCooldown)
        {
            sound.setBuffer(menuMoveSoundBuffer);
            sound.play();
            choice++;
            if (choice > 1)
                choice = 0;
            keyTimer = 0;
        }
        keyTimer++;

        switch (choice)
        {
        case 0:
            onePlayerText.setFillColor(Color::Red);
            twoPlayerText.setFillColor(Color::White);
            break;
        case 1:
            onePlayerText.setFillColor(Color::White);
            twoPlayerText.setFillColor(Color::Red);
            break;
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(titleText);
        window.draw(onePlayerText);
        window.draw(twoPlayerText);
        window.display();
    }
}

void StartGame(RenderWindow &window, int numPlayers)
{
    PlayerManager playerManager(window);
    playerManager.setNumPlayers(numPlayers);

    bool isGameOver = false;

    Texture backgroundImage;
    Sprite backgroundSprite;
    backgroundImage.loadFromFile("Resources/Images/Background.png");
    backgroundSprite.setTexture(backgroundImage);

    IntRect texRect1(0, CELL_SIZE, CELL_SIZE, CELL_SIZE);
    IntRect texRect2(0, CELL_SIZE, CELL_SIZE, CELL_SIZE);
    int keyCooldown = 5;
    int keyTimer1 = 0;
    int keyTimer2 = 0;

    vector<Car> cars;
    int carSpawnCooldown = 20;
    int carSpawnTimer = 0;

    vector<Log> logs;
    int logSpawnCooldown = 20;
    int logSpawnTimer = 0;

    SoundBuffer moveSoundBuffer;
    moveSoundBuffer.loadFromFile("Resources/Sounds/jump-sound2.wav");
    Sound sound;
    sound.setBuffer(moveSoundBuffer);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        playerManager.checkScoringStatus();
        playerManager.updateScores();
        playerManager.checkForDeaths();
        playerManager.updateDeathTimers();

        Frog *player1 = playerManager.getPlayer(0);
        if (player1 && !player1->isDead && !player1->hasWon)
        {
            player1->texRect = texRect1;
            PlayerMovement(*player1, texRect1, keyCooldown, keyTimer1, sound, 0);
        }

        if (numPlayers == 2)
        {
            Frog *player2 = playerManager.getPlayer(1);
            if (player2 && !player2->isDead && !player2->hasWon)
            {
                player2->texRect = texRect2;
                PlayerMovement(*player2, texRect2, keyCooldown, keyTimer2, sound, 1);
            }
        }

        // Check collisions and update game state for all players
        for (int i = 0; i < numPlayers; i++)
        {
            Frog *player = playerManager.getPlayer(i);
            bool isOnLog = false;

            if (player->hasWon || player->isDead)
                continue;

            for (int j = 0; j < logs.size(); j++)
            {
                if (player->getSprite().getGlobalBounds().intersects(logs[j].getSprite().getGlobalBounds()))
                {
                    player->MoveWithLog(logs[j]);
                    isOnLog = true;
                }
            }

            for (int j = 0; j < cars.size(); j++)
            {
                if (player->getSprite().getGlobalBounds().intersects(cars[j].getSprite().getGlobalBounds()))
                {
                    player->isDead = true;
                    break;
                }
            }

            if (player->getSprite().getPosition().y < 280 && !isOnLog)
            {
                player->isDead = true;
            }
        }

        CarSpawner(carSpawnTimer, carSpawnCooldown, cars);
        ObjectRemover(cars);
        LogSpawner(logSpawnTimer, logSpawnCooldown, logs);
        ObjectRemover(logs);

        window.clear();
        window.draw(backgroundSprite);

        for (int i = 0; i < logs.size(); i++)
        {
            logs[i].Move();
            window.draw(logs[i].getSprite());
        }

        playerManager.drawPlayers();

        for (int i = 0; i < cars.size(); i++)
        {
            cars[i].Move();
            window.draw(cars[i].getSprite());
        }

        playerManager.drawScores();
        playerManager.drawDeathMessages();
        window.display();

        // Check if game is over (all players either dead or won)
        bool allPlayersFinished = true;
        for (int i = 0; i < numPlayers; i++)
        {
            Frog *player = playerManager.getPlayer(i);
            if (!player->isDead && !player->hasWon)
            {
                allPlayersFinished = false;
                break;
            }
        }

        if (allPlayersFinished)
        {
            playerManager.saveScores();

            if (GameOver(window, playerManager))
            {
                for (int i = logs.size() - 1; i >= 0; i--)
                {
                    logs.erase(logs.begin() + i);
                }
                for (int i = cars.size() - 1; i >= 0; i--)
                {
                    cars.erase(cars.begin() + i);
                }
                playerManager.resetPlayers();
                isGameOver = false;
            }
            else
            {
                break;
            }
        }
    }
}

void PlayerMovement(Frog &player, IntRect &texRect, int &keyCooldown, int &keyTimer, Sound &sound, int playerIndex)
{
    if (player.isDead || player.hasWon)
        return;

    if (playerIndex == 0)
    {
        if ((Keyboard::isKeyPressed(Keyboard::W)) && keyTimer >= keyCooldown)
        {
            sound.play();
            texRect.top = 1 * CELL_SIZE;
            texRect.left = 1 * CELL_SIZE;
            player.texRect = texRect;
            texRect.left = 0 * CELL_SIZE;
            player.Move(0, -1);
            keyTimer = 0;
        }
        else if ((Keyboard::isKeyPressed(Keyboard::S)) && keyTimer >= keyCooldown)
        {
            sound.play();
            texRect.top = 3 * CELL_SIZE;
            texRect.left = 1 * CELL_SIZE;
            player.texRect = texRect;
            texRect.left = 0 * CELL_SIZE;
            player.Move(0, 1);
            keyTimer = 0;
        }
        else if ((Keyboard::isKeyPressed(Keyboard::A)) && keyTimer >= keyCooldown)
        {
            sound.play();
            texRect.top = 2 * CELL_SIZE;
            texRect.left = 1 * CELL_SIZE;
            player.texRect = texRect;
            texRect.left = 0 * CELL_SIZE;
            player.Move(-1, 0);
            keyTimer = 0;
        }
        else if ((Keyboard::isKeyPressed(Keyboard::D)) && keyTimer >= keyCooldown)
        {
            sound.play();
            texRect.top = 0 * CELL_SIZE;
            texRect.left = 1 * CELL_SIZE;
            player.texRect = texRect;
            texRect.left = 0 * CELL_SIZE;
            player.Move(1, 0);
            keyTimer = 0;
        }
        else
        {
            keyTimer++;
        }
    }
    else if (playerIndex == 1)
    {
        if ((Keyboard::isKeyPressed(Keyboard::Up)) && keyTimer >= keyCooldown)
        {
            sound.play();
            texRect.top = 1 * CELL_SIZE;
            texRect.left = 1 * CELL_SIZE;
            player.texRect = texRect;
            texRect.left = 0 * CELL_SIZE;
            player.Move(0, -1);
            keyTimer = 0;
        }
        else if ((Keyboard::isKeyPressed(Keyboard::Down)) && keyTimer >= keyCooldown)
        {
            sound.play();
            texRect.top = 3 * CELL_SIZE;
            texRect.left = 1 * CELL_SIZE;
            player.texRect = texRect;
            texRect.left = 0 * CELL_SIZE;
            player.Move(0, 1);
            keyTimer = 0;
        }
        else if ((Keyboard::isKeyPressed(Keyboard::Left)) && keyTimer >= keyCooldown)
        {
            sound.play();
            texRect.top = 2 * CELL_SIZE;
            texRect.left = 1 * CELL_SIZE;
            player.texRect = texRect;
            texRect.left = 0 * CELL_SIZE;
            player.Move(-1, 0);
            keyTimer = 0;
        }
        else if ((Keyboard::isKeyPressed(Keyboard::Right)) && keyTimer >= keyCooldown)
        {
            sound.play();
            texRect.top = 0 * CELL_SIZE;
            texRect.left = 1 * CELL_SIZE;
            player.texRect = texRect;
            texRect.left = 0 * CELL_SIZE;
            player.Move(1, 0);
            keyTimer = 0;
        }
        else
        {
            keyTimer++;
        }
    }
}

void CarSpawner(int &carSpawnTimer, int &carSpawnCooldown, vector<Car> &cars)
{
    if (carSpawnTimer < carSpawnCooldown)
    {
        carSpawnTimer++;
        return;
    }
    carSpawnTimer = 0;
    int randomCarType = rand() % 6;
    int randomLane = (rand() % 4) + 4;
    cars.push_back(Car(randomCarType, randomLane));
}

void ObjectRemover(vector<Car> &cars)
{
    for (int i = 0; i < cars.size(); i++)
    {
        if ((cars[i].getSprite().getPosition().x > 900) || (cars[i].getSprite().getPosition().x + cars[i].getSprite().getGlobalBounds().width < 0))
        {
            cars.erase(cars.begin() + i);
        }
    }
}

void ObjectRemover(vector<Log> &logs)
{
    for (int i = 0; i < logs.size(); i++)
    {
        if ((logs[i].getSprite().getPosition().x > 800 + logs[i].getSprite().getGlobalBounds().width) || (logs[i].getSprite().getPosition().x + logs[i].getSprite().getGlobalBounds().width < 0))
        {
            logs.erase(logs.begin() + i);
        }
    }
}

void LogSpawner(int &logSpawnTimer, int &logSpawnCooldown, vector<Log> &logs)
{
    if (logSpawnTimer < logSpawnCooldown)
    {
        logSpawnTimer++;
        return;
    }
    logSpawnTimer = 0;
    int randomLogType = rand() % 3;
    int randomLane = rand() % 4;
    int tries = 0;

    // Collision-free spawning logic
    bool isIntersecting = true;
    if (logs.size() > 0)
    {
        for (int i = 0; i < logs.size(); i++)
        {
            if (Log(randomLogType, randomLane).getSprite().getGlobalBounds().intersects(logs[i].getSprite().getGlobalBounds()))
            {
                if (randomLane == 3)
                    randomLane = 0;
                else
                    randomLane++;
                isIntersecting = true;
            }
            else
                isIntersecting = false;
        }
    }
    else
        logs.push_back(Log(randomLogType, randomLane));
    if (isIntersecting == false)
    {
        logs.push_back(Log(randomLogType, randomLane));
    }
}

bool GameOver(RenderWindow &window, PlayerManager &playerManager)
{
    SoundBuffer menuSelectSoundBuffer;
    menuSelectSoundBuffer.loadFromFile("Resources/Sounds/gta-menu.wav");
    SoundBuffer menuMoveSoundBuffer;
    menuMoveSoundBuffer.loadFromFile("Resources/Sounds/gta-menu_2.wav");
    SoundBuffer gameOverSoundBuffer;
    gameOverSoundBuffer.loadFromFile("Resources/Sounds/game-over.wav");
    SoundBuffer gameWonSoundBuffer;
    gameWonSoundBuffer.loadFromFile("Resources/Sounds/game-win.wav");
    Sound sound;

    bool anyPlayerWon = false;
    for (int i = 0; i < playerManager.getNumPlayers(); i++)
    {
        if (playerManager.getPlayer(i)->hasWon)
        {
            anyPlayerWon = true;
            break;
        }
    }

    if (anyPlayerWon)
    {
        sound.setBuffer(gameWonSoundBuffer);
        sound.play();
    }
    else
    {
        sound.setBuffer(gameOverSoundBuffer);
        sound.play();
    }

    Texture backgroundTexture;
    backgroundTexture.loadFromFile("Resources/Images/Background.png");
    backgroundTexture.update(window);
    Sprite gameOverSprite;
    gameOverSprite.setTexture(backgroundTexture);

    RectangleShape gameOverRectangele;
    gameOverRectangele.setSize(Vector2f(500, 400));
    gameOverRectangele.setFillColor(Color(0, 0, 0, 200));
    gameOverRectangele.setPosition((WINDOW_WIDTH - gameOverRectangele.getGlobalBounds().width) / 2, (WINDOW_HEIGHT - gameOverRectangele.getGlobalBounds().height) / 2);

    Font gameOverFont;
    if (!gameOverFont.loadFromFile("Resources/Fonts/Goldman-Regular.ttf"))
        cout << "could not load game over menu font" << endl;

    Text retryText;
    retryText.setCharacterSize(50);
    retryText.setFillColor(Color::White);
    retryText.setFont(gameOverFont);
    Text exitText = retryText;

    if (anyPlayerWon)
        retryText.setString("Play Again");
    else
        retryText.setString("Retry");
    retryText.setPosition((WINDOW_WIDTH - retryText.getGlobalBounds().width) / 2, 300);
    exitText.setString("Exit");
    exitText.setPosition((WINDOW_WIDTH - exitText.getGlobalBounds().width) / 2, 370);

    Text gameOverText;
    gameOverText.setCharacterSize(70);
    gameOverText.setFillColor(Color::Green);
    gameOverText.setFont(gameOverFont);

    // Determine winner logic
    int winnerIndex = -1;
    int highestScore = -1;
    int numWinners = 0;

    for (int i = 0; i < playerManager.getNumPlayers(); i++)
    {
        if (playerManager.getPlayer(i)->hasWon)
        {
            numWinners++;
            int score = playerManager.getScore(i);
            if (score > highestScore)
            {
                highestScore = score;
                winnerIndex = i;
            }
        }
    }

    if (anyPlayerWon)
    {
        if (numWinners == 1)
        {
            gameOverText.setString("P" + std::to_string(winnerIndex + 1) + " Won!");
        }
        else if (numWinners > 1)
        {
            if (playerManager.getNumPlayers() == 2 &&
                playerManager.getScore(0) == playerManager.getScore(1) &&
                playerManager.getPlayer(0)->hasWon && playerManager.getPlayer(1)->hasWon)
            {
                gameOverText.setString("It's a Tie!");
            }
            else
            {
                gameOverText.setString("P" + std::to_string(winnerIndex + 1) + " Won!");
            }
        }
    }
    else
    {
        gameOverText.setString("Game Over");
    }
    gameOverText.setPosition((WINDOW_WIDTH - gameOverText.getGlobalBounds().width) / 2, 130);

    Text resultsText;
    resultsText.setCharacterSize(30);
    resultsText.setFillColor(Color::White);
    resultsText.setFont(gameOverFont);

    std::string resultsStr = "Results:";
    for (int i = 0; i < playerManager.getNumPlayers(); i++)
    {
        Frog *player = playerManager.getPlayer(i);
        resultsStr += "\nPlayer " + std::to_string(i + 1) + ": ";
        if (player->hasWon)
        {
            resultsStr += std::to_string(playerManager.getScore(i)) + " pts";
            if (numWinners > 1 && i == winnerIndex)
            {
                resultsStr += " (Best Score!)";
            }
            else
            {
                resultsStr += " (Won)";
            }
        }
        else
        {
            resultsStr += std::to_string(playerManager.getScore(i)) + " pts (Died)";
        }
    }
    resultsText.setString(resultsStr);
    resultsText.setPosition((WINDOW_WIDTH - resultsText.getGlobalBounds().width) / 2, 220);

    Text explanationText;
    explanationText.setCharacterSize(20);
    explanationText.setFillColor(Color::Yellow);
    explanationText.setFont(gameOverFont);

    if (numWinners > 1)
    {
        if (playerManager.getNumPlayers() == 2 &&
            playerManager.getScore(0) == playerManager.getScore(1))
        {
            explanationText.setString("Both players completed with the same score!");
        }
        else
        {
            explanationText.setString("Winner determined by best score!");
        }
        explanationText.setPosition((WINDOW_WIDTH - explanationText.getGlobalBounds().width) / 2, 190);
    }

    int choice = 0;
    int keyTimer = 0;
    int keyCooldown = 10;

    while (true)
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (Keyboard::isKeyPressed(Keyboard::Enter))
            {
                sound.setBuffer(menuSelectSoundBuffer);
                sound.play();
                sleep(milliseconds(300));
                if (choice == 0)
                    return true;
                if (choice == 1)
                    return false;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Up) && keyTimer >= keyCooldown)
        {
            sound.setBuffer(menuMoveSoundBuffer);
            sound.play();
            choice--;
            if (choice < 0)
                choice = 1;
            keyTimer = 0;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down) && keyTimer >= keyCooldown)
        {
            sound.setBuffer(menuMoveSoundBuffer);
            sound.play();
            choice++;
            if (choice > 1)
                choice = 0;
            keyTimer = 0;
        }
        keyTimer++;

        switch (choice)
        {
        case 0:
            retryText.setFillColor(Color::Red);
            exitText.setFillColor(Color::White);
            break;
        case 1:
            retryText.setFillColor(Color::White);
            exitText.setFillColor(Color::Red);
            break;
        }

        window.draw(gameOverSprite);
        window.draw(gameOverRectangele);
        window.draw(gameOverText);
        window.draw(resultsText);
        if (numWinners > 1)
            window.draw(explanationText);
        window.draw(retryText);
        window.draw(exitText);

        window.display();
    }
}
