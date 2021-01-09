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
		Player(const Vector2<float> pos)
			: Entity('@', FOREGROUND_BLUE | FOREGROUND_INTENSITY, pos, 20)
		{
			vWalk = 0.22f;
			hWalk = 0.35f;
			vRun = 0.55f;
			hRun = 0.55f;
			healthStatIndex = Engine::addGUIText("Health: " + std::to_string((int) health) + "/" + std::to_string((int) maxHealth));
			weaponStatIndex = Engine::addGUIText("Weapon: ?");
		}

		void tick();
		void hurt(float amount, GameObject* from);
		void heal(float amount);
		void changeWeapon(Weapons::Weapon* newWeapon, bool noDelete = false);
	};
}