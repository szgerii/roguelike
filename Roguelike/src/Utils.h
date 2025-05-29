#pragma once
#include <Windows.h>
#include <chrono>
#include <cmath>
#include <vector>

#ifndef FG_WHITE
#define FG_WHITE (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN)
#endif

#ifndef BG_WHITE
#define BG_WHITE (BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN)
#endif

namespace CR {
	template <typename T>
	struct Vector2 {
	public:
		T x, y;

		Vector2(T x, T y)
			: x(x), y(y) {}

		bool operator ==(const Vector2<T>& other) {
			return x == other.x && y == other.y;
		}
		
		bool operator !=(const Vector2<T>& other) {
			return x != other.x || y != other.y;
		}

		Vector2<int> getClosestCellTo(const Vector2<T>& pos) {
			Vector2<int> roundedPos = { (int)round(pos.x), (int)round(pos.y) };

			int startX = roundedPos.x == 0 ? roundedPos.x : roundedPos.x - 1,
				endX = roundedPos.x == 120 - 1 ? roundedPos.x : roundedPos.x + 1,
				//endX = roundedPos.x == Engine::getMapWidth() - 1 ? roundedPos.x : roundedPos.x + 1,
				startY = roundedPos.y == 0 ? roundedPos.y : roundedPos.y - 1,
				//endY = roundedPos.y == Engine::getMapHeight() - 1 ? roundedPos.y : roundedPos.y + 1;
				endY = roundedPos.y == 40 - 1 ? roundedPos.y : roundedPos.y + 1;

			Vector2<int> closestCell = { startX, startY };
			int minDistance = closestCell.getManhattanDistanceTo({ (int)round(x), (int)round(y) });

			for (int x = startX; x <= endX; x++) {
				for (int y = startY; y <= endY; y++) {
					if (x == startX && y == startY)
						continue;

					Vector2<int> currentPos = { x, y };
					int currDistance = currentPos.getManhattanDistanceTo({ (int)round(this->x), (int)round(this->y) });
					if (currDistance < minDistance) {
						closestCell = currentPos;
						minDistance = currDistance;
					}
				}
			}

			return closestCell;
		}

		int getManhattanDistanceTo(const Vector2<T>& pos) {
			return abs(x - pos.x) + abs(y - pos.y);
		}
	};

	template <typename precision = std::chrono::milliseconds>
	long long getCurrentTime() {
		return std::chrono::time_point_cast<precision>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
	}

	// Bresenham's line drawing algorithm
	std::vector<Vector2<int>> getLine(const Vector2<int>& start, const Vector2<int>& end);
	std::vector<Vector2<int>> getLineUntil(const Vector2<int>& start, const Vector2<int>& through, const Vector2<int>& min, const Vector2<int>& max);

	template <typename T>
	Vector2<T> getDistance(const Vector2<T>& pos1, const Vector2<T>& pos2) {
		return { abs(pos1.x - pos2.x), abs(pos1.y - pos2.y) };
	}

	// min, max inclusive
	int getRandomNumberBetween(int min, int max);
}