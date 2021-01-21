#include "SotiBullet.h"

namespace CR::Entities {
	void SotiBullet::tick() {
		int currentPos;

		if (direction == Direction::LEFT || direction == Direction::RIGHT)
			currentPos = (int)round(position.x);
		else
			currentPos = (int)round(position.y);

		if (currentPos != lastPos) {
			damage -= 1;
			lastPos = currentPos;
		
			if (damage < minDamage)
				damage = minDamage;
		}

		Bullet::tick();
	}
}