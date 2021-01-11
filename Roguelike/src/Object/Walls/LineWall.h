#pragma once
#include "Engine/CustomGameObject.h"
#include "Utils.h"
#include <vector>

namespace CR::Objects {
	class LineWall : public CustomGameObject
	{
	protected:
		int width, height, startX, startY;
	
	private:
		std::vector<Vector2<int>> coords;

	public:
		LineWall(char skin, Vector2<int>& start, Vector2<int>& end)
			: CustomGameObject(skin, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED, { 0, 0 }) {
			init(start, end);
		}

		void updateCollision();
		void render(CHAR_INFO* screen, int screenWidth, int screenHeight) const;

	private:
		void init(Vector2<int>& start, Vector2<int>& end);
	};
}