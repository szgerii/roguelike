#include "RectWall.h"
#include "Engine/Game.h"

namespace CR::Objects {
	RectWall::RectWall(char skin, unsigned short skinColor, Vector2<int>& start, Vector2<int>& end)
		: CustomGameObject(skin, skinColor, 0, 0), start(0, 0), end(0, 0) {
		start.x = start.x < end.x ? start.x : end.x;
		start.y = start.y < end.y ? start.y : end.y;
		end.x = start.x >= end.x ? start.x : end.x;
		end.y = start.y >= end.y ? start.y : end.y;
	}
	
	RectWall::RectWall(char skin, unsigned short skinColor, int startX, int startY, int endX, int endY)
		: CustomGameObject(skin, skinColor, 0, 0), start(0, 0), end(0, 0)
	{
		start.x = startX < endX ? startX : endX;
		start.y = startY < endY ? startY : endY;
		end.x = startX >= endX ? startX : endX;
		end.y = startY >= endY ? startY : endY;
	}

	void RectWall::updateCollision() {
		for (int x = start.x; x <= end.x; x++)
			for (int y = start.y; y <= end.y; y++)
				Engine::addCollision(x, y, this);
	}

	void RectWall::render(CHAR_INFO* screen, int screenWidth, int screenHeight) const {
		for (int x = start.x; x <= end.x; x++)
			for (int y = start.y; y <= end.y; y++)
				*(screen + (screenWidth * y + x)) = CHAR_INFO{ (wchar_t)skin, skinColor };
	}
}