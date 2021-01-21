#pragma once
#include "Weapon.h"

namespace CR::Weapons {
	class Soti : public Weapon {
	protected:
		float startDamage, minDamage;

	public:
		Soti(float startDamage, float minDamage)
			: Weapon("The \"Soti\"", startDamage, 0.75f, 0.44f, 800, 3, 10, 5), startDamage(startDamage), minDamage(minDamage) {}

		virtual GameObject* getNewBullet(const Vector2<float>& startPos, Direction direction);
	};
}
