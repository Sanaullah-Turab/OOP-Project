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
    std::vector<Clock> deathClocks;  // Track time since death
    std::vector<bool> playerRemoved; // Track if player has been removed
    std::vector<Text> deathMessages; // Death messages to display
    Font messageFont;                // Font for messages
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

    void checkForDeaths();    // Check for new deaths
    void updateDeathTimers(); // Update death timers and remove players
    void drawPlayers();
    void drawScores();
    void drawDeathMessages(); // Draw death messages

    bool isAnyPlayerAlive() const;
    bool haveAllPlayersWon() const;
};