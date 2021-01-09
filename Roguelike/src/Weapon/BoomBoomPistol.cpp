#include "BoomBoomPistol.h"
#include "Entity/Bullets/ExplodingBullet.h"

namespace CR::Weapons {
	void BoomBoomPistol::fire(Direction direction) {
		namespace chrono = std::chrono;

		if (magazine <= 0 || chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - lastFire).count() < cooldown || owner == nullptr)
			return;

		Entities::ExplodingBullet* bullet = nullptr;
		Vector2<float> startPos = owner->getPos();
		if (direction == Direction::LEFT || direction == Direction::RIGHT) {
			startPos.x += direction == Direction::LEFT ? -1 : 1;
			bullet = new Entities::ExplodingBullet('-', WHITE, startPos, hBulletSpeed, damage, direction);
		} else {
			startPos.y += direction == Direction::UP ? -1 : 1;
			bullet = new Entities::ExplodingBullet('-', WHITE, startPos, vBulletSpeed, damage, direction);
		}

		bullet->addToIgnoreList(owner);
		Engine::addGameObject(bullet);
		magazine--;

		lastFire = chrono::steady_clock::now();
		updateStat();
	}
}