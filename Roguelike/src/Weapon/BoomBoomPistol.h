#pragma once
#include "Weapon/Weapon.h"

namespace CR::Weapons {
	class BoomBoomPistol : public Weapon {
	public:
		BoomBoomPistol()
			: Weapon(10, 0.6f, 0.2f, 500, 5, 20) {}

		virtual void fire(Direction direction);
	};
}
