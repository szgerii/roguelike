#include <cmath>
#include "Enemy.h"

namespace CR::Entities {
	void Enemy::tick() {
		const Vector2<float>& playerLocation = Engine::getPlayer()->getPos();

		int xDiff = round(playerLocation.x) - round(position.x);
		int yDiff = round(playerLocation.y) - round(position.y);
		
		if (xDiff == 0) {
			if (yDiff < 0)
				currentWeapon->fire(Direction::UP);
			else
				currentWeapon->fire(Direction::DOWN);
		} else if (yDiff == 0) {
			if (xDiff < 0)
				currentWeapon->fire(Direction::LEFT);
			else
				currentWeapon->fire(Direction::RIGHT);
		} else {
			if (abs(xDiff) < abs(yDiff))
				move({ xDiff < 0 ? -hWalk : hWalk, 0 });
			else
				move({ 0, yDiff < 0 ? -vWalk : vWalk });
		}

		currentWeapon->reload();
	}
}