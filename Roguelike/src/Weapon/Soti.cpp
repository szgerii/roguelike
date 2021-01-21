#include "Soti.h"
#include "Entity/Bullets/SotiBullet.h"

namespace CR::Weapons {
	GameObject* Soti::getNewBullet(const Vector2<float>& startPos, Direction direction) {
		if (direction == Direction::LEFT || direction == Direction::RIGHT)
			return new Entities::SotiBullet(startPos, hBulletSpeed, startDamage * dmgMult, minDamage * dmgMult, direction, owner, true);
		else
			return new Entities::SotiBullet(startPos, vBulletSpeed, startDamage * dmgMult, minDamage * dmgMult, direction, owner, true);
	}
}