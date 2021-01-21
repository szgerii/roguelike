#include "LongShot.h"
#include "Entity/Bullets/LongShotBullet.h"

namespace CR::Weapons {
	GameObject* LongShot::getNewBullet(const Vector2<float>& startPos, Direction direction) {
		if (direction == Direction::LEFT || direction == Direction::RIGHT)
			return new Entities::LongShotBullet(startPos, hBulletSpeed, startDamage * dmgMult, maxDamage * dmgMult, direction, owner, true);
		else
			return new Entities::LongShotBullet(startPos, vBulletSpeed, startDamage * dmgMult, maxDamage * dmgMult, direction, owner, true);
	}
}