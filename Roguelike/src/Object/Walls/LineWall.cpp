#include <Windows.h>
#include <vector>
#include "Object/Walls/LineWall.h"
#include "Engine/Game.h"

namespace CR::Objects {
	LineWall::LineWall(char skin, Vector2<int>& start, Vector2<int>& end)
		: CustomGameObject(skin, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED, 0, 0)
	{
		init(start, end);
	}

	LineWall::LineWall(char skin, int startX, int startY, int endX, int endY)
		: CustomGameObject(skin, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED, 0, 0)
	{
		Vector2 start(startX, startY);
		Vector2 end(endX, endY);
		init(start, end);
	}
	
	void LineWall::updateCollision() {
		for (auto& coord : coords)
			Engine::addCollision(coord, this);
	}

	void LineWall::render(CHAR_INFO* screen, int screenWidth, int screenHeight) const {
		for (auto& coord : coords)
			*(screen + (screenWidth * coord.y + coord.x)) = CHAR_INFO{ (wchar_t)skin, skinColor };
	}

	void LineWall::init(Vector2<int>& start, Vector2<int>& end) {
		// Bresenham's line drawing algorithm
		int deltaX = end.x - start.x, deltaY;
		float slope, correction = 0;

		if (start.y < end.y) {
			// Downwards
			deltaY = end.y - start.y;
			if (deltaX > deltaY) {
				slope = (float)deltaY / deltaX;
				int y = start.y;

				for (int x = start.x; x <= end.x; x++) {
					correction += slope;

					if (correction > 1) {
						y++;
						correction -= 1;
					}

					coords.emplace_back(x, y);
				}
			} else {
				slope = (float)deltaX / deltaY;
				int x = start.x;

				for (int y = start.y; y <= end.y; y++) {
					correction += slope;

					if (correction > 1) {
						x++;
						correction -= 1;
					}

					coords.emplace_back(x, y);
				}
			}
		} else {
			// Upwards
			deltaY = start.y - end.y;
			if (deltaX > deltaY) {
				slope = (float)deltaY / deltaX;
				int y = start.y;

				for (int x = start.x; x <= end.x; x++) {
					correction += slope;

					if (correction > 1) {
						y--;
						correction -= 1;
					}

					coords.emplace_back(x, y);
				}
			} else {
				slope = (float)deltaX / deltaY;
				int x = start.x;

				for (int y = start.y; y >= end.y; y--) {
					correction += slope;

					if (correction > 1) {
						x++;
						correction -= 1;
					}

					coords.emplace_back(x, y);
				}
			}
		}
	}
}