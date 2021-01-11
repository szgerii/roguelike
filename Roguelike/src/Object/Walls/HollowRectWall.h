#pragma once
#include "Object/Walls/RectWall.h"

namespace CR::Objects {
	class HollowRectWall : public RectWall {
	public:
		HollowRectWall(char skin, unsigned short skinColor, const Vector2<int>& start, const Vector2<int>& end)
			: RectWall(skin, skinColor, start, end) {}

		void updateCollision();
		void render(CHAR_INFO* screen, int screenWidth, int screenHeight) const;
	};
}