#pragma once
#include <chrono>
#include "Engine/GameObject.h"
#include "Bullet.h"

namespace CR::Entities {
	class ExplodingBullet : public Bullet {
	public:
		ExplodingBullet(char skin, unsigned short skinColor, Vector2<float> start, float speed, float damage, Direction direction, GameObject* sender, bool initialCollisionCheck=true)
			: Bullet(skin, skinColor, start, speed, damage, direction, sender, false) {
			if (initialCollisionCheck)
				collisionCheck();
		}

		virtual void tick();

	protected:
		virtual void collisionCheck();
		virtual void handleHit(GameObject* obj);
	};
}
