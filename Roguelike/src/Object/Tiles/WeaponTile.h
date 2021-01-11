#pragma once
#include "Object/Tiles/Tile.h"
#include "Weapon/Weapon.h"

namespace CR::Objects {
	class WeaponTile : public Tile {
	protected:
		Weapons::Weapon* weapon;

	public:
		WeaponTile(const Vector2<int>& pos, Weapons::Weapon* weapon)
			: Tile(' ', BACKGROUND_BLUE | BACKGROUND_RED, pos), weapon(weapon) {}

		virtual void hurt(float amount, GameObject* from);
	};
}
