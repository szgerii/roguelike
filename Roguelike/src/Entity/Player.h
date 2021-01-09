#pragma once
#include <Windows.h>
#include "Entity/Entity.h"
#include "Events/KeyEvent.h"

namespace CR::Entities {
	class Player : public Entity
	{
	private:
		int healthStatIndex, weaponStatIndex;

	public:
		Player()
			: Entity('@', FOREGROUND_BLUE | FOREGROUND_INTENSITY, 1, 1, 20)
		{
			vWalk = 0.4f;
			hWalk = 0.2f;
			vRun = 0.7f;
			hRun = 0.5f;
			healthStatIndex = Engine::addGUIText("Health: " + std::to_string((int) health) + "/" + std::to_string((int) maxHealth));
			weaponStatIndex = Engine::addGUIText("Weapon: ?");
		}

		Player(float x, float y)
			: Entity('@', FOREGROUND_BLUE | FOREGROUND_INTENSITY, x, y, 20)
		{
			vWalk = 0.4f;
			hWalk = 0.2f;
			vRun = 0.7f;
			hRun = 0.5f;
			healthStatIndex = Engine::addGUIText("Health: " + std::to_string((int) health) + "/" + std::to_string((int) maxHealth));
			weaponStatIndex = Engine::addGUIText("Weapon: ?");
		}

		void tick();
		void hurt(float amount);
		void heal(float amount);
		void changeWeapon(Weapons::Weapon* newWeapon, bool noDelete = false);
	};
}