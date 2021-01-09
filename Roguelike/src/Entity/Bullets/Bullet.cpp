#include <cmath>
#include "Bullet.h"
#include "Engine/Game.h"

namespace CR::Entities {
	void Bullet::tick() {
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

		GameObject* collObj = Engine::objectFromCoord({ (int)round(position.x), (int)round(position.y) });

		if (collObj != nullptr && collObj != this) {
			for (auto& obj : ignoreList)
				if (obj == collObj)
					return;

			handleHit(collObj);
			die();
		}
	}

	void Bullet::handleHit(GameObject* obj) {
		obj->hurt(damage);
	}

	void Bullet::die() {
		Engine::removeGameObject(this, true);
	}

	void Bullet::addToIgnoreList(GameObject* obj) {
		ignoreList.push_back(obj);
	}
}