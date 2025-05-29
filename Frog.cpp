#include "Frog.h"
#include "Global.h"
#include <iostream>

Frog::Frog(RenderWindow &window) : window(window), texRect(0, CELL_SIZE, CELL_SIZE, CELL_SIZE), isDead(false)
{
	Reset();
}

void Frog::Draw()
{
	sprite.setTextureRect(texRect);
	window.draw(sprite);
}
void Frog::Reset()
{
	hasWon = false;
	isDead = false;
	texture.loadFromFile("Resources/Images/Frog.png");
	sprite.setTexture(texture);
	sprite.setTextureRect(texRect);
	sprite.setScale(Vector2f(2.f, 2.f));
	Vector2f startingPos = Vector2f(WINDOW_WIDTH / 2 - sprite.getGlobalBounds().width / 2, WINDOW_HEIGHT - sprite.getGlobalBounds().height * 1.5);
	sprite.setPosition(startingPos);
}

void Frog::Reset(float xOffset)
{
	hasWon = false;
	isDead = false;
	texture.loadFromFile("Resources/Images/Frog.png");
	sprite.setTexture(texture);
	sprite.setTextureRect(texRect);
	sprite.setScale(Vector2f(2.f, 2.f));
	Vector2f startingPos = Vector2f(xOffset, WINDOW_HEIGHT - sprite.getGlobalBounds().height * 1.5);
	sprite.setPosition(startingPos);
}

void Frog::Move(int x, int y)
{
	sprite.move(Vector2f(x * LANE_HEIGHT, y * LANE_HEIGHT));
	if (sprite.getPosition().y < 0)
		// sprite.setPosition(sprite.getPosition().x, TOP_BOUND + CELL_SIZE);
		hasWon = true;
	if (sprite.getPosition().y + sprite.getGlobalBounds().height > WINDOW_HEIGHT)
		sprite.setPosition(sprite.getPosition().x, WINDOW_HEIGHT - sprite.getGlobalBounds().height - CELL_SIZE);
	if (sprite.getPosition().x < LEFT_BOUND)
		sprite.setPosition(LEFT_BOUND, sprite.getPosition().y);
	if (sprite.getPosition().x > RIGHT_BOUND)
		sprite.setPosition(WINDOW_WIDTH - LEFT_BOUND - sprite.getGlobalBounds().width, sprite.getPosition().y);
}
void Frog::MoveWithLog(Log log)
{
	sprite.move(Vector2f(log.speed * log.direction, 0));
	if (sprite.getPosition().x < LEFT_BOUND)
		sprite.setPosition(LEFT_BOUND, sprite.getPosition().y);
	if (sprite.getPosition().x > WINDOW_WIDTH - LEFT_BOUND - sprite.getGlobalBounds().width)
		sprite.setPosition(WINDOW_WIDTH - LEFT_BOUND - sprite.getGlobalBounds().width, sprite.getPosition().y);
}