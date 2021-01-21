#pragma once
#include "Weapon/Weapon.h"

namespace CR::Weapons {
	class BoomBoomPistol : public Weapon {
	public:
		BoomBoomPistol()
			: Weapon("The Boom Boom", 15, 0.6f, 0.34f, 500, 5, 20, 5) {}

		virtual GameObject* getNewBullet(const Vector2<float>& startPos, Direction direction);
	};
}
