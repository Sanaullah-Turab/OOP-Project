#include "Log.h"
#include "Global.h"

Log::Log(int type, int laneNo)
{
	this->type = type;
	this->laneNo = laneNo;
	Initialize();
	SetLane();
}
void Log::SetTexRect()
{
	texRect.left = 0;
	texRect.height = CELL_SIZE;
	texRect.top = type * CELL_SIZE;
	texRect.width = (type + 1) * 2 * CELL_SIZE;
}
void Log::Initialize()
{
	texture = new Texture;
	texture->loadFromFile("Resources/Images/Logs.png");
	SetTexRect();
	sprite.setTexture(*texture);
	sprite.setTextureRect(texRect);
	sprite.setScale(Vector2f(2.8f, 2.8f));
}
void Log::SetLane()
{
	// 9.6 is added in y position to make the car sprite in the middle of the lane
	if (laneNo % 2 == 0)
	{
		direction = 1;
		sprite.setPosition(Vector2f(sprite.getPosition().x - sprite.getGlobalBounds().width, laneNo * LANE_HEIGHT + TOP_BOUND + 9.6f));
	}
	else if (laneNo % 2 == 1)
	{
		direction = -1;
		sprite.setPosition(Vector2f(sprite.getPosition().x + WINDOW_WIDTH + sprite.getGlobalBounds().width, laneNo * LANE_HEIGHT + TOP_BOUND + 9.6f));
	}
}
void Log::Move()
{
	sprite.setPosition(Vector2f(sprite.getPosition().x + (speed * direction), sprite.getPosition().y));
}