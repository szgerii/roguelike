#pragma once
#include "Engine/CustomGameObject.h"
#include "Utils.h"
#include <vector>

namespace CR::Objects {
	class LineWall : public CustomGameObject
	{
	public:
		void updateCollision();
		void render(CHAR_INFO* screen, int screenWidth, int screenHeight) const;

		LineWall(char skin, Vector2<int>& start, Vector2<int>& end);
		LineWall(char skin, int startX, int startY, int endX, int endY);

	protected:
		int width, height, startX, startY;

	private:
		std::vector<Vector2<int>> coords;
		void init(Vector2<int>& start, Vector2<int>& end);
	};
}