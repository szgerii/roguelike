#include "BoomBoomPistol.h"
#include "Entity/Bullets/ExplodingBullet.h"

namespace CR::Weapons {
	GameObject* BoomBoomPistol::getNewBullet(const Vector2<float>& startPos, Direction direction) {
		if (direction == Direction::LEFT || direction == Direction::RIGHT)
			return new Entities::ExplodingBullet('o', owner->getSkinColor(), startPos, hBulletSpeed, damage * dmgMult, direction, owner, true);
		else
			return new Entities::ExplodingBullet('o', owner->getSkinColor(), startPos, vBulletSpeed, damage * dmgMult, direction, owner, true);
	}
}