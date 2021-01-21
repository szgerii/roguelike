#pragma once
#include "Bullet.h"

namespace CR::Entities {
	class StunBullet : public Bullet {
	protected:
		int stunDuration;
	
	public:
		StunBullet(const Vector2<float> start, float speed, int stunDurationMs, Direction direction, GameObject* sender, bool initialCollisionCheck = true)
			: Bullet('+', FOREGROUND_BLUE | FOREGROUND_INTENSITY, start, speed, 0, direction, sender, initialCollisionCheck), stunDuration(stunDurationMs) { }

		virtual void handleHit(GameObject* obj);
	};
}
