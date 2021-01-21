#include <cmath>
#include "Enemy.h"

namespace CR::Entities {
	void Enemy::tick() {
		return;

		const Vector2<float>& playerLocation = Engine::getPlayer()->getPos();
		Weapons::Weapon* currentWeapon = inventory.getCurrentItem();

		int xDiff = (int)round(playerLocation.x) - (int)round(position.x);
		int yDiff = (int)round(playerLocation.y) - (int)round(position.y);
		
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

	void Enemy::die() {
		Engine::removeGameObject(healthbar, true);

		Entity::die();
	}

	void Enemy::createHealthbar() {
		if (healthbar != nullptr)
			deleteHealthbar();

		healthbar = new Particles::Healthbar(this, 0, FOREGROUND_RED | FOREGROUND_INTENSITY);
		Engine::addGameObject(healthbar);
	}

	void Enemy::deleteHealthbar() {
		if (healthbar == nullptr)
			return;

		Engine::removeGameObject(healthbar, true);
		healthbar = nullptr;
	}

	void Enemy::setFreeze(bool freeze) {
		if (freeze)
			deleteHealthbar();
		else
			createHealthbar();
		
		this->freeze = freeze;
	}
}