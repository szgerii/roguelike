#include "AmmoTile.h"
#include "Entity/Player.h"
#include "Engine/Game.h"

namespace CR::Objects {
	void AmmoTile::hurt(float amount, GameObject* from) {
		if (from == Engine::getPlayer()) {
			Weapons::Weapon* playerWeapon = ((Entities::Player*)Engine::getPlayer())->getCurrentWeapon();

			if (playerWeapon == nullptr)
				return;

			playerWeapon->addAmmo(playerWeapon->getPickupSize());
		}

		die();
	}
}