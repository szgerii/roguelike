#include "DestroyableTile.h"

namespace CR::Objects {
	void DestroyableTile::hurt(float amount, GameObject* from) {
		health--;

		if (health == 0)
			die();
		else if (initialHealth / 2 >= health)
			skinColor &= ~FOREGROUND_INTENSITY;
	}
}