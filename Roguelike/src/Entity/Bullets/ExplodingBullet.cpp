#include "ExplodingBullet.h"

namespace CR::Entities {
	void ExplodingBullet::tick() {
		if (dead) {
			int mapWidth = Engine::getMapWidth();
			int mapHeight = Engine::getMapHeight();

			int xRounded = (int)round(position.x);
			int yRounded = (int)round(position.y);

			int startX = xRounded == 0 ? xRounded : xRounded - 1,
				endX = xRounded == Engine::getMapWidth() - 1 ? xRounded : xRounded + 1,
				startY = yRounded == 0 ? yRounded : yRounded - 1,
				endY = yRounded == Engine::getMapWidth() - 1 ? yRounded : yRounded + 1;
			
			long long elapsed;
			{
				using namespace std::chrono;
				elapsed = duration_cast<milliseconds>(high_resolution_clock::now() - deathTime).count();
			}

			for (int x = startX; x <= endX; x++)
				for (int y = startY; y <= endY; y++)
					Engine::addToOverlay(x, y, '#', elapsed < 100 ? FOREGROUND_RED : FOREGROUND_RED | FOREGROUND_INTENSITY);

			if (elapsed > 200)
				die();

			return;
		}

		switch (direction) {
		case Direction::LEFT:
			move({ -speed, 0 }, true);
			break;

		case Direction::UP:
			move({ 0, -speed }, true);
			break;

		case Direction::RIGHT:
			move({ speed, 0 }, true);
			break;

		case Direction::DOWN:
			move({ 0, speed }, true);
			break;
		}

		/*for (int x = xRounded - 1; x <= xRounded + 1; x++) {
			for (int y = yRounded - 1; y <= yRounded + 1; y++) {
				GameObject* collObj = Engine::objectFromCoord({ x, y });

				if (collObj != nullptr && collObj != this) {
					for (auto& obj : ignoreList)
						if (obj == collObj)
							return;

					hit = true;
					handleHit(collObj);
				}
			}
		}*/
		
		collisionCheck();
	}

	void ExplodingBullet::collisionCheck() {
		bool hit = false;
		int xRounded = (int)round(position.x);
		int yRounded = (int)round(position.y);

		int startX = xRounded == 0 ? xRounded : xRounded - 1,
			endX = xRounded == Engine::getMapWidth() - 1 ? xRounded : xRounded + 1,
			startY = yRounded == 0 ? yRounded : yRounded - 1,
			endY = yRounded == Engine::getMapWidth() - 1 ? yRounded : yRounded + 1;

		GameObject* collObj = Engine::objectFromCoord({ (int)round(position.x), (int)round(position.y) });

		if (collObj != nullptr && collObj != this) {
			for (auto& obj : ignoreList)
				if (obj == collObj)
					return;

			hit = true;
		}

		if (hit) {
			for (int x = startX; x <= endX; x++) {
				for (int y = startY; y <= endY; y++) {
					GameObject* collObj = Engine::objectFromCoord({ x, y });

					if (collObj != nullptr && collObj != this) {
						bool ignored = false;

						for (auto& obj : ignoreList) {
							if (obj == collObj) {
								ignored = true;
								break;
							}
						}

						if (ignored)
							continue;

						handleHit(collObj);
					}
				}
			}

			dead = true;
			deathTime = std::chrono::high_resolution_clock::now();
		}
	}
}