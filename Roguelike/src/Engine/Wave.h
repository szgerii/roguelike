#pragma once
#include <vector>
#include "Engine/GameObject.h"
#include "Weapon/Weapon.h"

namespace CR {
	class Wave {
	protected:
		std::vector<GameObject*> gameObjects;

	public:
		Wave(int enemyCount, int ammoTileCount, int healthTileCount, unsigned char* room, Weapons::Weapon* weapon = nullptr);

		void activate();
	};
}