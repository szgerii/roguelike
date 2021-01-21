#include "Utils.h"

namespace CR {
	std::vector<Vector2<int>> getLine(const Vector2<int>& start, const Vector2<int>& end) {
		std::vector<Vector2<int>> coords;
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

		return coords;
	}

	std::vector<Vector2<int>> getLineUntil(const Vector2<int>& start, const Vector2<int>& through, const Vector2<int>& min, const Vector2<int>& max) {
		std::vector<Vector2<int>> coords;
		int deltaX = through.x - start.x, deltaY;
		float slope, correction = 0;

		if (start.y < through.y) {
			// Downwards
			deltaY = through.y - start.y;
			if (deltaX > deltaY) {
				slope = (float)deltaY / deltaX;
				int y = start.y;

				for (int x = start.x; x <= max.x && y <= max.y; x++) {
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

				for (int y = start.y; x <= max.x && y <= max.y; y++) {
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
			deltaY = start.y - through.y;
			if (deltaX > deltaY) {
				slope = (float)deltaY / deltaX;
				int y = start.y;

				for (int x = start.x; x <= max.x && y <= max.y; x++) {
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

				for (int y = start.y; y >= min.y && x >= min.x; y--) {
					correction += slope;

					if (correction > 1) {
						x++;
						correction -= 1;
					}

					coords.emplace_back(x, y);
				}
			}
		}

		return coords;
	}

	/*Vector2<float> getDistance(const Vector2<int>& pos1, const Vector2<int>& pos2) {
		return { abs(pos1.x - pos2.x), abs(pos1.y - pos2.y) };
	}*/

	int getRandomNumberBetween(int min, int max) {
		return (rand() % (max - min + 1)) + min;
	}
}