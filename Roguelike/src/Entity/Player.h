#pragma once
#include <Windows.h>
#include "Entity/Entity.h"
#include "Events/KeyEvent.h"
#include "Particle/Healthbar.h"
#include "Property.h"

namespace CR::Entities {
	class Player : public Entity
	{
	private:
		float hSlow, vSlow;

		Property<float> weaponDamageMultiplier, ammoPickupMultiplier, damageReduction;

		int	healthStatIndex, inventoryStatIndex, weaponStatIndex, roomStatIndex;
		Particles::Healthbar* healthbar;

	public:
		Player(const Vector2<float> pos)
			: Entity('@', FOREGROUND_BLUE | FOREGROUND_INTENSITY, pos, 20), weaponDamageMultiplier(0.0f, 0.0f, 0.0f, 0.0f), ammoPickupMultiplier(0.0f, 0.0f, 0.0f, 0.0f), damageReduction(0.0f, 0.0f, 0.0f, 0.0f) {
			meleeCooldown = 400;
			
			switch (Engine::getDifficulty()) {
			case Difficulty::EASY:
				health = 25;
				maxHealth = Property<float>(25.0f, 10.0f, 55.0f, 60.0f);
				meleeDamage = Property<float>(10.0f, 5.0f, 25.0f, 30.0f);
				weaponDamageMultiplier = Property<float>(1.0f, 0.5f, 2.5f, 2.75f);
				ammoPickupMultiplier = Property<float>(1.0f, 0.25f, 2.25f, 2.5f);
				damageReduction = Property<float>(1.0f, 0.1f, 0.6f, 0.5f, false);
				break;

			case Difficulty::NORMAL:
				health = 20;
				maxHealth = Property<float>(20.0f, 10.0f, 40.0f, 50.0f);
				meleeDamage = Property<float>(8.0f, 4.0f, 20.0f, 27.0f);
				weaponDamageMultiplier = Property<float>(1.0f, 0.25f, 2.25f, 2.6f);
				ammoPickupMultiplier = Property<float>(1.0f, 0.25f, 2.25f, 2.5f);
				damageReduction = Property<float>(1.0f, 0.1f, 0.65f, 0.57f, false);
				break;

			case Difficulty::HARD:
				health = 20;
				maxHealth = Property<float>(20.0f, 10.0f, 40.0f, 45.0f);
				meleeDamage = Property<float>(8.0f, 3.0f, 20.0f, 25.0f);
				weaponDamageMultiplier = Property<float>(1.0f, 0.25f, 2.0f, 2.5f);
				ammoPickupMultiplier = Property<float>(1.0f, 0.25f, 2.0f, 2.35f);
				damageReduction = Property<float>(1.0f, 0.07f, 0.7f, 0.63f, false);
				break;

			case Difficulty::VERY_HARD:
				health = 20;
				maxHealth = Property<float>(20.0f, 7.0f, 45.0f, 50.0f);
				meleeDamage = Property<float>(7.0f, 4.0f, 15.0f, 21.0f);
				weaponDamageMultiplier = Property<float>(1.0f, 0.25f, 2.0f, 2.35f);
				ammoPickupMultiplier = Property<float>(1.0f, 0.2f, 2.0f, 2.25f);
				damageReduction = Property<float>(1.0f, 0.05f, 0.75f, 0.67f, false);
				break;
			}

			vSlow = 0.12f;
			hSlow = 0.21f;
			vWalk = 0.22f;
			hWalk = 0.35f;
			vRun = 0.42f;
			hRun = 0.55f;

			type = Type::PLAYER;
			healthStatIndex = Engine::addGUIText("Health: " + std::to_string((int) health) + "/" + std::to_string((int) maxHealth.getValue()));
			inventoryStatIndex = Engine::addGUIText("Inventory: ?");
			weaponStatIndex = Engine::addGUIText("Weapon: ?");
			roomStatIndex = Engine::addGUIText("Room: ? (?/?)");
			inventory = Inventory<3>(this, inventoryStatIndex, weaponStatIndex);
			createHealthbar();
		}

		~Player() {
			deleteHealthbar();
		}

		void tick();
		void hurt(float amount, GameObject* from);
		void heal(float amount);
		void createHealthbar();
		void deleteHealthbar();
		void setFreeze(bool freeze);
		void die();
		bool pickUpItem(Weapons::Weapon* item);
		float getAmmoPickupMultiplier();
		void upgradeWeaponDamage();
		void downgradeWeaponDamage();
		void upgradePickupModifier();
		void downgradePickupModifier();
		Property<float>& getWeaponDamageMultiplierProp();
		Property<float>& getAmmoPickupMultiplierProp();
		Property<float>& getDamageReductionProp();
	};
}