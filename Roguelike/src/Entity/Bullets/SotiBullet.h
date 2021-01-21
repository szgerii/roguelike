#pragma once
#include <cmath>
#include "Bullet.h"

namespace CR::Entities {
	class SotiBullet : public Bullet {
	protected:
		float minDamage;
		int lastPos;
	
	public:
		SotiBullet(Vector2<float> start, float speed, float startDamage, float minDamage, Direction direction, GameObject* sender, bool initialCollisionCheck = true)
			: Bullet('}', sender->getSkinColor(), start, speed, startDamage, direction, sender, false), minDamage(minDamage) {
			if (direction == Direction::LEFT)
				skin = '{';

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
