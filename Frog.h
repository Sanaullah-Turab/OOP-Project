#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include "Log.h"

using namespace sf;
class Frog
{
private:
	Sprite sprite;
	Texture texture;
	RenderWindow &window;

public:
	bool hasWon;
	bool isDead; // to track if frog is dead
	IntRect texRect;
	Frog(RenderWindow &window);
	Sprite getSprite() { return sprite; }
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void Move(int x, int y);
	void Draw();
	void Reset();
	void Reset(float xOffset); // overloaded Reset method with custom x position
	void MoveWithLog(Log log);
};
