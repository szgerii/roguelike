#pragma once
#include <cmath>
#include "Bullet.h"

namespace CR::Entities {
	class LongShotBullet : public Bullet {
	protected:
		float maxDamage;
		int lastPos;
	
	public:
		LongShotBullet(Vector2<float> start, float speed, float startDamage, float maxDamage, Direction direction, GameObject* sender, bool initialCollisionCheck = true)
			: Bullet(']', sender->getSkinColor(), start, speed, startDamage, direction, sender, false), maxDamage(maxDamage) {
			if (direction == Direction::LEFT)
				skin = '[';

			if (direction == Direction::LEFT || direction == Direction::RIGHT)
				lastPos = (int)round(start.x);
			else
				lastPos = (int)round(start.y);

			if (initialCollisionCheck)
				collisionCheck();
		}

		virtual void tick();
	};
}
