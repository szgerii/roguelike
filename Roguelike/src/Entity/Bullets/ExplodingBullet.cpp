#include "ExplodingBullet.h"
#include "Particle/Particle.h"

namespace CR::Entities {
	void ExplodingBullet::tick() {
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
		
		collisionCheck();
	}

	void ExplodingBullet::collisionCheck() {
		int xRounded = (int)round(position.x);
		int yRounded = (int)round(position.y);
		
		GameObject* collObj = Engine::objectFromCoord({ xRounded, yRounded});

		if (collObj != nullptr && collObj != this && collObj->getType() != Type::BULLET && collObj->getType() != sender->getType()) {
			for (auto& obj : ignoreList)
				if (obj == collObj)
					return;

			int startX = xRounded == 0 ? xRounded : xRounded - 1,
				endX = xRounded == Engine::getMapWidth() - 1 ? xRounded : xRounded + 1,
				startY = yRounded == 0 ? yRounded : yRounded - 1,
				endY = yRounded == Engine::getMapHeight() - 1 ? yRounded : yRounded + 1;

			for (int x = startX; x <= endX; x++) {
				for (int y = startY; y <= endY; y++) {
					GameObject* target = Engine::objectFromCoord({ x, y });

					if (target != nullptr && target != this && target->getType() != Type::BULLET && target->getType() != sender->getType()) {
						bool ignored = false;

						for (auto& obj : ignoreList) {
							if (obj == target) {
								ignored = true;
								break;
							}
						}

						if (ignored)
							continue;

						handleHit(target);
					}
				}
			}

			die();
		}
	}

	void ExplodingBullet::handleHit(GameObject* obj) {
		int xRounded = (int)round(position.x);
		int yRounded = (int)round(position.y);

		int startX = xRounded == 0 ? xRounded : xRounded - 1,
			endX = xRounded == Engine::getMapWidth() - 1 ? xRounded : xRounded + 1,
			startY = yRounded == 0 ? yRounded : yRounded - 1,
			endY = yRounded == Engine::getMapWidth() - 1 ? yRounded : yRounded + 1;

		for (int x = startX; x <= endX; x++) {
			for (int y = startY; y <= endY; y++) {
				Particles::Particle* p1 = new Particles::Particle('#', FOREGROUND_RED, { x, y }, 120);
				Particles::Particle* p2 = new Particles::Particle('#', FOREGROUND_RED | FOREGROUND_GREEN, { x, y }, 200);
				Engine::addGameObject(p2);
				Engine::addGameObject(p1);
			}
		}

		Bullet::handleHit(obj);
	}
}