#pragma once
#include "Weapon.h"

namespace CR::Weapons {
	class LongShot : public Weapon {
	protected:
		float startDamage, maxDamage;

	public:
		LongShot(float startDamage, float maxDamage)
			: Weapon("The Long Shot", startDamage, 0.74f, 0.45f, 800, 3, 10, 5), startDamage(startDamage), maxDamage(maxDamage) {}

		virtual GameObject* getNewBullet(const Vector2<float>& startPos, Direction direction);
	};
}
