#include "Engine/Game.h"
#include "Object/Walls/HollowRectWall.h"

namespace CR::Objects {
	void HollowRectWall::updateCollision() {
		for (int x = start.x; x <= end.x; x++)
			Engine::addCollision(x, start.y, this);
		
		for (int y = start.y + 1; y < end.y; y++) {
			Engine::addCollision(start.x, y, this);
			Engine::addCollision(end.x, y, this);
		}

		for (int x = start.x; x <= end.x; x++)
			Engine::addCollision(x, end.y, this);
	}

	void HollowRectWall::render(CHAR_INFO* screen, int screenWidth, int screenHeight) const {
		for (int x = start.x; x <= end.x; x++)
			*(screen + (screenWidth * start.y + x)) = CHAR_INFO{ (wchar_t)skin, skinColor };

		for (int y = start.y + 1; y < end.y; y++) {
			*(screen + (screenWidth * y + start.x)) = CHAR_INFO{ (wchar_t)skin, skinColor };
			*(screen + (screenWidth * y + end.x)) = CHAR_INFO{ (wchar_t)skin, skinColor };
		}

		for (int x = start.x; x <= end.x; x++)
			*(screen + (screenWidth * end.y + x)) = CHAR_INFO{ (wchar_t)skin, skinColor };
	}
}