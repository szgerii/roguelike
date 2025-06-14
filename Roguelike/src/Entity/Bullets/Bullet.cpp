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

		collisionCheck();
	}

	void Bullet::handleHit(GameObject* obj) {
		obj->hurt(damage, sender);
	}

	void Bullet::collisionCheck() {
		GameObject* collObj = Engine::objectFromCoord({ (int)round(position.x), (int)round(position.y) });

		if (collObj != nullptr && collObj->getType() != Type::BULLET && collObj->getType() != sender->getType()) {
			for (auto& obj : ignoreList)
				if (obj == collObj)
					return;

			handleHit(collObj);
			die();
		}
	}

	void Bullet::addToIgnoreList(GameObject* obj) {
		ignoreList.push_back(obj);
	}
}