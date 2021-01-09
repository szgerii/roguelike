#pragma once
#include <chrono>
#include "Engine/GameObject.h"
#include "Bullet.h"

namespace CR::Entities {
	class ExplodingBullet : public Bullet {
	public:
		ExplodingBullet(char skin, unsigned short skinColor, Vector2<float> start, float speed, float damage, Direction direction)
			: Bullet(skin, skinColor, start, speed, damage, direction) { }

		ExplodingBullet(char skin, unsigned short skinColor, float startX, float startY, float speed, float damage, Direction direction)
			: Bullet(skin, skinColor, startX, startY, speed, damage, direction) { }

		virtual void tick();

	protected:
		bool dead = false;
		std::chrono::steady_clock::time_point deathTime;
		virtual void handleHit(GameObject* obj);
	};
}
