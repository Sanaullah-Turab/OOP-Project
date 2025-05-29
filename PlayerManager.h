#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Frog.h"
#include <string>
#include <fstream>
#include <vector>

using namespace sf;
using namespace std;

class PlayerManager
{
private:
    int numPlayers;
    vector<Frog *> players;
    vector<int> scores;
    vector<bool> scoringActive; // Tracks if scoring is active for each player
    vector<Clock> deathClocks;
    vector<bool> playerRemoved;
    vector<Text> deathMessages;
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
    void checkScoringStatus();
    void drawPlayers();
    void drawScores();
    void drawDeathMessages();

    bool isAnyPlayerAlive() const;
    bool haveAllPlayersWon() const;
};