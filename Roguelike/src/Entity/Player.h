#pragma once
#include <Windows.h>
#include "Entity/Entity.h"
#include "Events/KeyEvent.h"
#include "Particle/Healthbar.h"

namespace CR::Entities {
	class Player : public Entity
	{
	private:
		int healthStatIndex, inventoryStatIndex, weaponStatIndex;
		Particles::Healthbar* healthbar;

	public:
		Player(const Vector2<float> pos)
			: Entity('@', FOREGROUND_BLUE | FOREGROUND_INTENSITY, pos, 20)
		{
			vWalk = 0.22f;
			hWalk = 0.35f;
			vRun = 0.55f;
			hRun = 0.55f;
			meleeDamage = 7;
			healthStatIndex = Engine::addGUIText("Health: " + std::to_string((int) health) + "/" + std::to_string((int) maxHealth));
			inventoryStatIndex = Engine::addGUIText("Inventory: ?");
			weaponStatIndex = Engine::addGUIText("Weapon: ?");
			inventory = Inventory<3>(this, inventoryStatIndex, weaponStatIndex);
			healthbar = new Particles::Healthbar(this);
			Engine::addGameObject(healthbar);
		}

		void tick();
		void hurt(float amount, GameObject* from);
		void heal(float amount);
		void die();
	};
}