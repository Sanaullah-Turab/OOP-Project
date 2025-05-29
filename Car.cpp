#include "Car.h"
#include "Global.h"
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

Car::Car(int type, int laneNo)
{
	this->type = type;
	this->laneNo = laneNo;
	Initialize();
	SetLane();
}
void Car::SetTexRect()
{
	if (type == 0 || type == 1)
	{
		texRect.width = 2 * CELL_SIZE;
	}
	else
	{
		texRect.width = CELL_SIZE;
	}
	texRect.top = type * CELL_SIZE;
	texRect.left = 0;
	texRect.height = CELL_SIZE;
}
void Car::Initialize()
{
	texture = new Texture;
	texture->loadFromFile("Resources/Images/Cars.png");
	SetTexRect();
	sprite.setTexture(*texture);
	sprite.setTextureRect(texRect);
	sprite.setScale(Vector2f(2.8f, 2.8f));
}
void Car::SetLane()
{
	// 9.6 is added in y position to make the car sprite in the middle of the lane
	if (laneNo %2 == 0)
	{
		direction = 1;
		sprite.setPosition(Vector2f(sprite.getPosition().x - sprite.getGlobalBounds().width, laneNo * LANE_HEIGHT + TOP_BOUND + 9.6f));
	}
	else if (laneNo %2 == 1)
	{
		direction = -1;
		sprite.setScale(Vector2f(sprite.getScale().x * -1, sprite.getScale().y));
		sprite.setPosition(Vector2f(sprite.getPosition().x + WINDOW_WIDTH + sprite.getGlobalBounds().width, laneNo * LANE_HEIGHT + TOP_BOUND + 9.6f));
	}
}
void Car::Move()
{
	sprite.setPosition(Vector2f(sprite.getPosition().x + (speed * direction), sprite.getPosition().y));
}