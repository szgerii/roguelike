#include "AmmoTile.h"
#include "Entity/Player.h"
#include "Engine/Game.h"

namespace CR::Objects {
	void AmmoTile::hurt(float amount, GameObject* from) {
		ammoAmount = ((Entities::Player*)Engine::getPlayer())->getWeapon()->addAmmo(ammoAmount);

		if (ammoAmount <= 0)
			die();
	}
}