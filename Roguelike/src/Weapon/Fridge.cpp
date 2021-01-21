#include "Fridge.h"
#include "Entity/Bullets/StunBullet.h"

namespace CR::Weapons {
	GameObject* Fridge::getNewBullet(const Vector2<float>& startPos, Direction direction) {
		if (direction == Direction::LEFT || direction == Direction::RIGHT)
			return new Entities::StunBullet(startPos, hBulletSpeed, stunDuration, direction, owner, true);
		else
			return new Entities::StunBullet(startPos, vBulletSpeed, stunDuration, direction, owner, true);
	}
}