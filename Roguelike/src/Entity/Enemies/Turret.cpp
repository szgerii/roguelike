#include "Turret.h"

namespace CR::Entities {
	void Turret::tick() {
		inventory.getCurrentItem()->fire(Direction::LEFT);
	}
}