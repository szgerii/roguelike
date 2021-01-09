#pragma once
#include "Object/Walls/RectWall.h"

namespace CR::Objects {
	class HollowRectWall : public RectWall {
	public:
		HollowRectWall(char skin, unsigned short skinColor, Vector2<int>& start, Vector2<int>& end)
			: RectWall(skin, skinColor, start, end) {}

		HollowRectWall(char skin, unsigned short skinColor, int startX, int startY, int endX, int endY)
			: RectWall(skin, skinColor, startX, startY, endX, endY) {}

		void updateCollision();
		void render(CHAR_INFO* screen, int screenWidth, int screenHeight) const;
	};
}