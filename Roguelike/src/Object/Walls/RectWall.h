#pragma once
#include "Engine/CustomGameObject.h"

namespace CR::Objects {
	class RectWall : public CustomGameObject {
	public:
		void updateCollision();
		virtual void render(CHAR_INFO* screen, int screenWidth, int screenHeight) const;

		RectWall(char skin, unsigned short skinColor, Vector2<int>& start, Vector2<int>& end);
		RectWall(char skin, unsigned short skinColor, int startX, int startY, int endX, int endY);

	protected:
		Vector2<int> start, end;
	};
}