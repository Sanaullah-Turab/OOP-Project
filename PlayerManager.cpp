#include "PlayerManager.h"
#include "Global.h"
#include <iostream>

using namespace std;

PlayerManager::PlayerManager(RenderWindow &window) : window(window), numPlayers(1)
{
    players.push_back(new Frog(window));
    scores.push_back(0);
    scoringActive.push_back(false);
    deathClocks.push_back(Clock());
    playerRemoved.push_back(false);

    if (!messageFont.loadFromFile("Resources/Fonts/Goldman-Regular.ttf"))
    {
        cout << "Could not load font for death messages" << endl;
    }

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

    numPlayers = num;

    for (int i = 0; i < numPlayers; i++)
    {
        players.push_back(new Frog(window));
        scores.push_back(0);
        scoringActive.push_back(false);
        deathClocks.push_back(Clock());
        playerRemoved.push_back(false);

        Text deathText;
        deathText.setFont(messageFont);
        deathText.setCharacterSize(24);
        deathText.setFillColor(Color::Red);
        deathText.setString("Player " + to_string(i + 1) + " Died!");
        deathText.setPosition(WINDOW_WIDTH / 2 - deathText.getGlobalBounds().width / 2, 100 + i * 30);
        deathMessages.push_back(deathText);
    }

    if (numPlayers == 2)
    {
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
            players[i]->Reset();
        }
        else
        {
            players[i]->Reset(WINDOW_WIDTH / 2 + CELL_SIZE * 4);
        }

        players[i]->isDead = false;
        playerRemoved[i] = false;
        scoringActive[i] = false;
        deathClocks[i].restart();
    }

    scores.clear();
    for (int i = 0; i < numPlayers; i++)
    {
        scores.push_back(0);
    }

    gameClock.restart();
}

void PlayerManager::checkScoringStatus()
{
    for (int i = 0; i < numPlayers; i++)
    {
        if (!players[i]->isDead && !players[i]->hasWon)
        {
            float spawnAreaY = WINDOW_HEIGHT - players[i]->getSprite().getGlobalBounds().height * 1.5;
            float currentY = players[i]->getSprite().getPosition().y;

            // Activate scoring when player moves away from spawn area
            if (!scoringActive[i] && currentY < spawnAreaY - LANE_HEIGHT / 2)
            {
                scoringActive[i] = true;
            }

            // Deactivate scoring if player returns to spawn area
            if (scoringActive[i] && currentY >= spawnAreaY)
            {
                scoringActive[i] = false;
            }
        }
    }
}

void PlayerManager::updateScores()
{
    float timeElapsed = gameClock.getElapsedTime().asSeconds();

    for (int i = 0; i < numPlayers; i++)
    {
        if (!players[i]->hasWon && !players[i]->isDead && scoringActive[i])
        {
            scores[i] = static_cast<int>(timeElapsed);
        }
    }
}

void PlayerManager::checkForDeaths()
{
    for (int i = 0; i < numPlayers; i++)
    {
        if (players[i]->isDead && !playerRemoved[i])
        {
            if (deathClocks[i].getElapsedTime().asSeconds() > 10)
            {
                deathClocks[i].restart();
            }
        }
    }
}

void PlayerManager::updateDeathTimers()
{
    for (int i = 0; i < numPlayers; i++)
    {
        if (players[i]->isDead && !playerRemoved[i])
        {
            if (deathClocks[i].getElapsedTime().asSeconds() >= 3.0f)
            {
                playerRemoved[i] = true;
            }
        }
    }
}

void PlayerManager::drawDeathMessages()
{
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

    ofstream scoreFile("scores.txt", ios::app);
    if (scoreFile.is_open())
    {
        scoreFile << "Game with " << numPlayers << " player(s):" << endl;
        for (int i = 0; i < numPlayers; i++)
        {
            if (players[i]->hasWon)
            {
                scoreFile << "Player " << (i + 1) << ": " << scores[i] << " points" << endl;
            }
        }
        scoreFile << "-------------------------" << endl;
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

int PlayerManager::getNumPlayers() const
{
    return numPlayers;
}

void PlayerManager::drawPlayers()
{
    for (int i = 0; i < numPlayers; i++)
    {
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
        cout << "Could not load font for scores" << endl;
        return;
    }

    for (int i = 0; i < numPlayers; i++)
    {
        Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(Color::White);

        string scoreStr = "Player " + to_string(i + 1) + ": " + to_string(scores[i]);

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
        scoreText.setPosition(10, 10 + i * 30);
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
