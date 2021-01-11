#include "RectWall.h"
#include "Engine/Game.h"

namespace CR::Objects {
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