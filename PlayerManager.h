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

    void drawPlayers();
    void drawScores();

    bool isAnyPlayerAlive() const;
    bool haveAllPlayersWon() const;
};