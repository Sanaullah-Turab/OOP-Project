#include "PlayerManager.h"
#include "Global.h"
#include <iostream>

PlayerManager::PlayerManager(RenderWindow &window) : window(window), numPlayers(1)
{
    // Create initial player
    players.push_back(new Frog(window));
    scores.push_back(0);
    scoringActive.push_back(false); // Start with scoring inactive
    deathClocks.push_back(Clock());
    playerRemoved.push_back(false);

    // Initialize font
    if (!messageFont.loadFromFile("Resources/Fonts/Goldman-Regular.ttf"))
    {
        std::cout << "Could not load font for death messages" << std::endl;
    }

    // Create death message (initialized but not shown yet)
    Text deathText;
    deathText.setFont(messageFont);
    deathText.setCharacterSize(24);
    deathText.setFillColor(Color::Red);
    deathText.setString("Player 1 Died!");
    deathText.setPosition(WINDOW_WIDTH / 2 - deathText.getGlobalBounds().width / 2, 100);
    deathMessages.push_back(deathText);

    gameClock.restart();
}

PlayerManager::~PlayerManager()
{
    // Clean up player objects
    for (auto *player : players)
    {
        delete player;
    }
    players.clear();
    scores.clear();
    scoringActive.clear();
    deathClocks.clear();
    playerRemoved.clear();
    deathMessages.clear();
}

void PlayerManager::setNumPlayers(int num)
{
    if (num < 1)
        num = 1;
    if (num > 2)
        num = 2;

    // Reset existing players
    for (auto *player : players)
    {
        delete player;
    }
    players.clear();
    scores.clear();
    scoringActive.clear(); // Clear scoring active flags
    deathClocks.clear();
    playerRemoved.clear();
    deathMessages.clear();

    numPlayers = num;

    // Create new players
    for (int i = 0; i < numPlayers; i++)
    {
        players.push_back(new Frog(window));
        scores.push_back(0);
        scoringActive.push_back(false); // Start with scoring inactive
        deathClocks.push_back(Clock());
        playerRemoved.push_back(false);

        // Create death message for each player
        Text deathText;
        deathText.setFont(messageFont);
        deathText.setCharacterSize(24);
        deathText.setFillColor(Color::Red);
        deathText.setString("Player " + std::to_string(i + 1) + " Died!");
        deathText.setPosition(WINDOW_WIDTH / 2 - deathText.getGlobalBounds().width / 2, 100 + i * 30);
        deathMessages.push_back(deathText);
    }

    // If two players, offset the second player's starting position
    if (numPlayers == 2)
    {
        // Position player 2 a bit to the right of player 1
        players[1]->Reset(WINDOW_WIDTH / 2 + CELL_SIZE * 4);
    }

    gameClock.restart();
}

void PlayerManager::resetPlayers()
{
    for (int i = 0; i < numPlayers; i++)
    {
        if (i == 0)
        {
            players[i]->Reset(); // Default reset position
        }
        else
        {
            players[i]->Reset(WINDOW_WIDTH / 2 + CELL_SIZE * 4); // Offset for player 2
        }

        players[i]->isDead = false;
        playerRemoved[i] = false;
        scoringActive[i] = false; // Reset scoring status
        deathClocks[i].restart();
    }

    scores.clear();
    for (int i = 0; i < numPlayers; i++)
    {
        scores.push_back(0);
    }

    gameClock.restart();
}

// Add new method to check if players have left spawn area
void PlayerManager::checkScoringStatus()
{
    for (int i = 0; i < numPlayers; i++)
    {
        if (!scoringActive[i] && !players[i]->isDead && !players[i]->hasWon)
        {
            // Check if player has moved away from the spawn area
            // The spawn area is at the bottom of the screen, so we check if they've moved up
            float spawnAreaY = WINDOW_HEIGHT - players[i]->getSprite().getGlobalBounds().height * 1.5;
            float currentY = players[i]->getSprite().getPosition().y;

            // If the player has moved up at least one lane from spawn
            if (currentY < spawnAreaY - LANE_HEIGHT)
            {
                scoringActive[i] = true;
            }
        }
    }
}

// Modify updateScores to only increase score if scoring is active
void PlayerManager::updateScores()
{
    float timeElapsed = gameClock.getElapsedTime().asSeconds();

    // Update scores for active players (not won yet, not dead, and scoring active)
    for (int i = 0; i < numPlayers; i++)
    {
        if (!players[i]->hasWon && !players[i]->isDead && scoringActive[i])
        {
            // Points based on time survived (1 point per second)
            scores[i] = static_cast<int>(timeElapsed);
        }
    }
}

void PlayerManager::checkForDeaths()
{
    // Check for new deaths and restart their death clocks
    for (int i = 0; i < numPlayers; i++)
    {
        if (players[i]->isDead && !playerRemoved[i])
        {
            // If this is a new death, restart the death clock
            if (deathClocks[i].getElapsedTime().asSeconds() > 10)
            {
                deathClocks[i].restart();
            }
        }
    }
}

void PlayerManager::updateDeathTimers()
{
    // Check death timers and remove players after delay
    for (int i = 0; i < numPlayers; i++)
    {
        if (players[i]->isDead && !playerRemoved[i])
        {
            // If 3 seconds have passed since death, remove the player
            if (deathClocks[i].getElapsedTime().asSeconds() >= 3.0f)
            {
                playerRemoved[i] = true;
            }
        }
    }
}

void PlayerManager::drawDeathMessages()
{
    // Draw death messages for dead players that haven't been removed yet
    for (int i = 0; i < numPlayers; i++)
    {
        if (players[i]->isDead && !playerRemoved[i])
        {
            window.draw(deathMessages[i]);
        }
    }
}

void PlayerManager::saveScores() const
{
    // Only save scores if any player has won
    bool anyWinner = false;
    for (int i = 0; i < numPlayers; i++)
    {
        if (players[i]->hasWon)
        {
            anyWinner = true;
            break;
        }
    }

    if (!anyWinner)
        return;

    std::ofstream scoreFile("scores.txt", std::ios::app);
    if (scoreFile.is_open())
    {
        scoreFile << "Game with " << numPlayers << " player(s):" << std::endl;
        for (int i = 0; i < numPlayers; i++)
        {
            if (players[i]->hasWon)
            {
                scoreFile << "Player " << (i + 1) << ": " << scores[i] << " points" << std::endl;
            }
        }
        scoreFile << "-------------------------" << std::endl;
        scoreFile.close();
    }
}

Frog *PlayerManager::getPlayer(int index)
{
    if (index >= 0 && index < numPlayers)
    {
        return players[index];
    }
    return nullptr;
}

int PlayerManager::getScore(int index) const
{
    if (index >= 0 && index < numPlayers)
    {
        return scores[index];
    }
    return 0;
}

// Make sure this method is properly implemented:
int PlayerManager::getNumPlayers() const
{
    return numPlayers;
}

void PlayerManager::drawPlayers()
{
    for (int i = 0; i < numPlayers; i++)
    {
        // Only draw players that are not dead or not yet removed
        if (!players[i]->isDead || !playerRemoved[i])
        {
            players[i]->Draw();
        }
    }
}

void PlayerManager::drawScores()
{
    Font font;
    if (!font.loadFromFile("Resources/Fonts/Goldman-Regular.ttf"))
    {
        std::cout << "Could not load font for scores" << std::endl;
        return;
    }

    for (int i = 0; i < numPlayers; i++)
    {
        Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(Color::White);

        std::string scoreStr = "Player " + std::to_string(i + 1) + ": " + std::to_string(scores[i]);

        // Add status if player is dead or has won
        if (players[i]->isDead)
        {
            scoreStr += " (Dead)";
        }
        else if (players[i]->hasWon)
        {
            scoreStr += " (Won!)";
        }
        else if (!scoringActive[i])
        {
            scoreStr += " (Move to start scoring)";
        }

        scoreText.setString(scoreStr);

        // Position the score text
        scoreText.setPosition(10, 10 + i * 30);

        // Draw the score
        window.draw(scoreText);
    }
}

bool PlayerManager::isAnyPlayerAlive() const
{
    for (int i = 0; i < numPlayers; i++)
    {
        if (!players[i]->isDead && !players[i]->hasWon)
        {
            return true;
        }
    }
    return false;
}

bool PlayerManager::haveAllPlayersWon() const
{
    for (int i = 0; i < numPlayers; i++)
    {
        if (!players[i]->hasWon && !players[i]->isDead)
        {
            return false;
        }
    }
    return true;
}