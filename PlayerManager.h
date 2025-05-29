#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Frog.h"
#include <string>
#include <fstream>
#include <vector>

using namespace sf;

class PlayerManager
{
private:
    int numPlayers;
    std::vector<Frog *> players;
    std::vector<int> scores;
    std::vector<bool> scoringActive; // New flag to track if scoring is active for each player
    std::vector<Clock> deathClocks;
    std::vector<bool> playerRemoved;
    std::vector<Text> deathMessages;
    Font messageFont;
    Clock gameClock;
    RenderWindow &window;

public:
    PlayerManager(RenderWindow &window);
    ~PlayerManager();

    void setNumPlayers(int num);
    int getNumPlayers() const;

    void resetPlayers();
    void updateScores();
    void saveScores() const;

    Frog *getPlayer(int index);
    int getScore(int index) const;

    void checkForDeaths();
    void updateDeathTimers();
    void checkScoringStatus(); // New method to check if players have left spawn area
    void drawPlayers();
    void drawScores();
    void drawDeathMessages();

    bool isAnyPlayerAlive() const;
    bool haveAllPlayersWon() const;
};