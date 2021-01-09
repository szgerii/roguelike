#include "Object/ObstacleTile.h"

namespace CR::Objects {
	void ObstacleTile::hurt(float amount) {
		health--;

		if (health == 0)
			die();
		else if (initialHealth / 2 >= health)
			skinColor &= ~FOREGROUND_INTENSITY;
	}
}