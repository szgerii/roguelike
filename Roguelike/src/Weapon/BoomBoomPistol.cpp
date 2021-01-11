#include "BoomBoomPistol.h"
#include "Entity/Bullets/ExplodingBullet.h"

namespace CR::Weapons {
	void BoomBoomPistol::fire(Direction direction) {
		namespace chr = std::chrono;

		if (magazine <= 0 || getCurrentTime() - lastFireTime < cooldown || owner == nullptr)
			return;

		Entities::ExplodingBullet* bullet = nullptr;
		Vector2<float> startPos = owner->getPos();
		if (direction == Direction::LEFT || direction == Direction::RIGHT) {
			startPos.x += direction == Direction::LEFT ? -1 : 1;
			bullet = new Entities::ExplodingBullet('-', FG_WHITE, startPos, hBulletSpeed, damage, direction, owner);
		} else {
			startPos.y += direction == Direction::UP ? -1 : 1;
			bullet = new Entities::ExplodingBullet('-', FG_WHITE, startPos, vBulletSpeed, damage, direction, owner);
		}

		bullet->addToIgnoreList(owner);
		Engine::addGameObject(bullet);
		magazine--;

		lastFireTime = getCurrentTime();
		drawStat();
	}
}