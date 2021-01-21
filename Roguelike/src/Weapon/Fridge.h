#pragma once
#include "Weapon.h"

namespace CR::Weapons {
	class Fridge : public Weapon {
	protected:
		int stunDuration;
	
	public:
		Fridge(int stunDurationMs, float hBulletSpeed, float vBulletSpeed)
			: Weapon("The Fridge", 0, hBulletSpeed, vBulletSpeed, 1000, 6, 18, 8), stunDuration(stunDurationMs) { }

		virtual GameObject* getNewBullet(const Vector2<float>& startPos, Direction direction);
	};
}
