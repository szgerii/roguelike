#include "WeaponTile.h"
#include "Entity/Player.h"
#include "Engine/Game.h"

namespace CR::Objects {
	void WeaponTile::hurt(float amount, GameObject* from) {
		if (((Entities::Player*)Engine::getPlayer())->pickUpItem(weapon))
			die();
	}
}