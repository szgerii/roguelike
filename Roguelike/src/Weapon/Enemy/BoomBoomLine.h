#pragma once
#include "Weapon/Weapon.h"

namespace CR::Weapons {
	class BoomBoomLine : public Weapon {
	public:
		BoomBoomLine(float damage)
			: Weapon("Boom Line", damage, 0.6f, 0, 1000, 5, 20, 5, true) {}
	
	protected:
		virtual GameObject* getNewBullet(const Vector2<float>& startPos, Direction direction);
	};
}
