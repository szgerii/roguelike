#include "PDestroyableTile.h"
#include "Engine/Game.h"

namespace CR::Objects {
	void PDestroyableTile::hurt(float amount, GameObject* from) {
		if (from == Engine::getPlayer())
			DestroyableTile::hurt(amount, from);
	}
}