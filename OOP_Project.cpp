#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <cstdlib>
// #include <Windows.h>
#include "Frog.h"
#include "Global.h"
#include "Car.h"
#include "Log.h"
#include "PlayerManager.h" // Include the new PlayerManager

using namespace sf;
using namespace std;

// Function Prototypes
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
    // creating window
    RenderWindow window(VideoMode(800, 600), "OOP Project", Style::Default);
    window.setFramerateLimit(30);

    int numPlayers = 1; // Default to 1 player

    StartScreen(window);
    PlayerSelectScreen(window, numPlayers); // New player selection screen
    StartGame(window, numPlayers);

    return 0;
}

void StartScreen(RenderWindow &window)
{
    // Gameobjects Initialization
    Texture backgroundImage;
    Sprite backgroundSprite;
    if (!backgroundImage.loadFromFile("Resources/Images/StartScreen.png"))
        cout << "could not load main menu image" << endl;
    backgroundSprite.setTexture(backgroundImage);
    // Font initialization
    Font mainMenuFont;
    if (!mainMenuFont.loadFromFile("Resources/Fonts/Goldman-Regular.ttf"))
        cout << "could not load main menu font" << endl;
    Text startGameText;
    startGameText.setCharacterSize(50);
    startGameText.setFillColor(Color::White);
    startGameText.setFont(mainMenuFont);
    Text creditsText = startGameText;
    Text exitText = startGameText;

    // Text initialization
    startGameText.setString("Start Game");
    startGameText.setPosition((WINDOW_WIDTH - startGameText.getGlobalBounds().width) / 2, 300);
    creditsText.setString("Credits");
    creditsText.setPosition((WINDOW_WIDTH - creditsText.getGlobalBounds().width) / 2, 370);
    exitText.setString("Exit");
    exitText.setPosition((WINDOW_WIDTH - exitText.getGlobalBounds().width) / 2, 440);

    // sounds initialization
    SoundBuffer menuSelectSoundBuffer;
    menuSelectSoundBuffer.loadFromFile("Resources/Sounds/gta-menu.wav");
    SoundBuffer menuMoveSoundBuffer;
    menuMoveSoundBuffer.loadFromFile("Resources/Sounds/gta-menu_2.wav");
    Sound sound;

    int choice = 0;
    int keyTimer = 0;
    int keyCooldown = 10;

    // Main game loop
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
                // credits
            }
            else if (Keyboard::isKeyPressed(Keyboard::Enter) && choice == 2)
            {
                sound.setBuffer(menuSelectSoundBuffer);
                sound.play();
                sleep(milliseconds(300));
                window.close();
            }
        }
        // Menu selection
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

        // Clear
        window.clear();
        // Draw Stuff
        window.draw(backgroundSprite);
        window.draw(startGameText);
        window.draw(creditsText);
        window.draw(exitText);
        // Display
        window.display();
    }
}

void PlayerSelectScreen(RenderWindow &window, int &numPlayers)
{
    // Gameobjects Initialization
    Texture backgroundImage;
    Sprite backgroundSprite;
    if (!backgroundImage.loadFromFile("Resources/Images/StartScreen.png"))
        cout << "could not load player select image" << endl;
    backgroundSprite.setTexture(backgroundImage);

    // Font initialization
    Font font;
    if (!font.loadFromFile("Resources/Fonts/Goldman-Regular.ttf"))
        cout << "could not load font" << endl;

    // Title text
    Text titleText;
    titleText.setCharacterSize(60);
    titleText.setFillColor(Color::White);
    titleText.setFont(font);
    titleText.setString("Select Players");
    titleText.setPosition((WINDOW_WIDTH - titleText.getGlobalBounds().width) / 2, 150);

    // Option texts
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

    // sounds initialization
    SoundBuffer menuSelectSoundBuffer;
    menuSelectSoundBuffer.loadFromFile("Resources/Sounds/gta-menu.wav");
    SoundBuffer menuMoveSoundBuffer;
    menuMoveSoundBuffer.loadFromFile("Resources/Sounds/gta-menu_2.wav");
    Sound sound;

    int choice = 0;
    int keyTimer = 0;
    int keyCooldown = 10;

    // Main loop
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
                numPlayers = choice + 1; // Set number of players (1 or 2)
                return;
            }
        }

        // Menu selection
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

        // Update text colors based on selection
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

        // Draw
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
    // Initialize PlayerManager instead of a single player
    PlayerManager playerManager(window);
    playerManager.setNumPlayers(numPlayers);

    bool isGameOver = false;

    // Gameobjects Initialization
    Texture backgroundImage;
    Sprite backgroundSprite;
    backgroundImage.loadFromFile("Resources/Images/Background.png");
    backgroundSprite.setTexture(backgroundImage);

    // player movement support
    IntRect texRect(0, CELL_SIZE, CELL_SIZE, CELL_SIZE);
    int keyCooldown = 5;
    int keyTimer = 0;
    int keyTimer2 = 0; // Separate timer for player 2

    // car spawner support
    vector<Car> cars;
    int carSpawnCooldown = 20;
    int carSpawnTimer = 0;

    // Log spawner support
    vector<Log> logs;
    int logSpawnCooldown = 20;
    int logSpawnTimer = 0;

    // sounds initialization
    SoundBuffer moveSoundBuffer;
    moveSoundBuffer.loadFromFile("Resources/Sounds/jump-sound2.wav");
    Sound sound;
    sound.setBuffer(moveSoundBuffer);

    // Main game loop
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        // Update player scores
        playerManager.updateScores();

        // Handle player 1 movement
        Frog *player1 = playerManager.getPlayer(0);
        player1->texRect = texRect;
        PlayerMovement(*player1, texRect, keyCooldown, keyTimer, sound, 0);

        // Handle player 2 movement if 2-player mode
        if (numPlayers == 2)
        {
            Frog *player2 = playerManager.getPlayer(1);
            player2->texRect = texRect;
            PlayerMovement(*player2, texRect, keyCooldown, keyTimer2, sound, 1);
        }

        // Check collisions and update game state for all players
        for (int i = 0; i < numPlayers; i++)
        {
            Frog *player = playerManager.getPlayer(i);
            bool isOnLog = false;

            // Skip if player has already won or is dead
            if (player->hasWon || player->isDead)
                continue;

            // Collision detection with logs
            for (int j = 0; j < logs.size(); j++)
            {
                if (player->getSprite().getGlobalBounds().intersects(logs[j].getSprite().getGlobalBounds()))
                {
                    player->MoveWithLog(logs[j]);
                    isOnLog = true;
                }
            }

            // Collision detection with cars
            for (int j = 0; j < cars.size(); j++)
            {
                if (player->getSprite().getGlobalBounds().intersects(cars[j].getSprite().getGlobalBounds()))
                {
                    player->isDead = true;
                }
            }

            // Collision detection with water
            if (player->getSprite().getPosition().y < 280 && !isOnLog)
            {
                player->isDead = true;
            }
        }

        // spawners
        CarSpawner(carSpawnTimer, carSpawnCooldown, cars);
        ObjectRemover(cars);
        LogSpawner(logSpawnTimer, logSpawnCooldown, logs);
        ObjectRemover(logs);

        // Clear
        window.clear();

        // Draw Stuff
        window.draw(backgroundSprite);

        // drawing logs
        for (int i = 0; i < logs.size(); i++)
        {
            logs[i].Move();
            window.draw(logs[i].getSprite());
        }

        // Draw all players
        playerManager.drawPlayers();

        // drawing cars
        for (int i = 0; i < cars.size(); i++)
        {
            cars[i].Move();
            window.draw(cars[i].getSprite());
        }

        // Draw scores
        playerManager.drawScores();

        // Display
        window.display();

        // Check game over conditions
        bool allPlayersDead = true;
        for (int i = 0; i < numPlayers; i++)
        {
            Frog *player = playerManager.getPlayer(i);
            if (!player->isDead && !player->hasWon)
            {
                allPlayersDead = false;
                break;
            }
        }

        // Game is over if all players are either dead or have won
        if (allPlayersDead || playerManager.haveAllPlayersWon())
        {
            // Save scores if any player has won
            playerManager.saveScores();

            if (GameOver(window, playerManager))
            {
                // Reset game for a new round
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
    // Skip if player is dead or has won
    if (player.isDead || player.hasWon)
        return;

    if (playerIndex == 0)
    {
        // Player 1 controls (WASD)
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
        // Player 2 controls (Arrow keys)
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
    // collision free spawning
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
    // sound initialization
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

    // Game Over background sprite initialization
    Texture backgroundTexture;
    backgroundTexture.loadFromFile("Resources/Images/Background.png");
    backgroundTexture.update(window);
    Sprite gameOverSprite;
    gameOverSprite.setTexture(backgroundTexture);

    // initializing game over rectangle
    RectangleShape gameOverRectangele;
    gameOverRectangele.setSize(Vector2f(500, 400));
    gameOverRectangele.setFillColor(Color(0, 0, 0, 200));
    gameOverRectangele.setPosition((WINDOW_WIDTH - gameOverRectangele.getGlobalBounds().width) / 2, (WINDOW_HEIGHT - gameOverRectangele.getGlobalBounds().height) / 2);

    // font initialization
    Font gameOverFont;
    if (!gameOverFont.loadFromFile("Resources/Fonts/Goldman-Regular.ttf"))
        cout << "could not load game over menu font" << endl;

    // game over menu button text initialization
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

    // GameOver Text initialization
    Text gameOverText;
    gameOverText.setCharacterSize(80);
    gameOverText.setFillColor(Color::Green);
    gameOverText.setFont(gameOverFont);

    if (anyPlayerWon)
    {
        // Display which player(s) won
        std::string winText = "";
        for (int i = 0; i < playerManager.getNumPlayers(); i++)
        {
            if (playerManager.getPlayer(i)->hasWon)
            {
                if (winText != "")
                    winText += " & ";
                winText += "P" + std::to_string(i + 1);
            }
        }
        gameOverText.setString(winText + " Won!");
    }
    else
    {
        gameOverText.setString("Game Over");
    }
    gameOverText.setPosition((WINDOW_WIDTH - gameOverText.getGlobalBounds().width) / 2, 130);

    // Show scores if any player won
    Text scoreText;
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(Color::White);
    scoreText.setFont(gameOverFont);

    if (anyPlayerWon)
    {
        std::string scoreStr = "Scores:";
        for (int i = 0; i < playerManager.getNumPlayers(); i++)
        {
            if (playerManager.getPlayer(i)->hasWon)
            {
                scoreStr += "\nPlayer " + std::to_string(i + 1) + ": " + std::to_string(playerManager.getScore(i));
            }
        }
        scoreText.setString(scoreStr);
        scoreText.setPosition((WINDOW_WIDTH - scoreText.getGlobalBounds().width) / 2, 220);
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
        // Menu selection
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

        // Draw
        window.draw(gameOverSprite);
        window.draw(gameOverRectangele);
        window.draw(gameOverText);
        if (anyPlayerWon)
            window.draw(scoreText);
        window.draw(retryText);
        window.draw(exitText);

        // display
        window.display();
    }
}