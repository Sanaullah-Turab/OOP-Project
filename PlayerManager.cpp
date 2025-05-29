#include "PlayerManager.h"
#include "Global.h"
#include <iostream>

PlayerManager::PlayerManager(RenderWindow &window) : window(window), numPlayers(1)
{
    // Create initial player
    players.push_back(new Frog(window));
    scores.push_back(0);
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

    numPlayers = num;

    // Create new players
    for (int i = 0; i < numPlayers; i++)
    {
        players.push_back(new Frog(window));
        scores.push_back(0);
    }

    // If two players, offset the second player's starting position
    if (numPlayers == 2)
    {
        // Position player 2 a bit to the right of player 1
        players[1]->Reset(WINDOW_WIDTH / 2 + CELL_SIZE * 4);
    }

    gameClock.restart();
}

int PlayerManager::getNumPlayers() const
{
    return numPlayers;
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
    }

    scores.clear();
    for (int i = 0; i < numPlayers; i++)
    {
        scores.push_back(0);
    }

    gameClock.restart();
}

void PlayerManager::updateScores()
{
    float timeElapsed = gameClock.getElapsedTime().asSeconds();

    // Update scores for active players (not won yet and not dead)
    for (int i = 0; i < numPlayers; i++)
    {
        if (!players[i]->hasWon)
        {
            // Points based on time survived (1 point per second)
            scores[i] = static_cast<int>(timeElapsed);
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

void PlayerManager::drawPlayers()
{
    for (auto *player : players)
    {
        player->Draw();
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
        scoreText.setString(scoreStr);

        // Position the score text
        scoreText.setPosition(10, 10 + i * 30);

        // Draw the score
        window.draw(scoreText);
    }
}

bool PlayerManager::isAnyPlayerAlive() const
{
    for (auto *player : players)
    {
        if (!player->hasWon) // If a player hasn't won yet, they're still in play
        {
            return true;
        }
    }
    return false;
}

bool PlayerManager::haveAllPlayersWon() const
{
    for (auto *player : players)
    {
        if (!player->hasWon) // If any player hasn't won, return false
        {
            return false;
        }
    }
    return true;
}