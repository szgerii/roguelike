#pragma once
#include "Entity/Enemies/Enemy.h"

namespace CR::Entities {
	class ShootyBoi : public Enemy {
	public:
		ShootyBoi(const Vector2<float>& start, Weapons::Weapon* weapon = nullptr, float hSpeed = 0.18f, float vSpeed = 0.07f)
			: Enemy('<', FOREGROUND_GREEN, start, 12) {
			float weaponDamage = 0, tempMaxHealth, tempMeleeDamage;
			int reloadTime = 0;

			switch (Engine::getDifficulty()) {
			case Difficulty::EASY:
				health = 12;
				tempMaxHealth = 12;
				weaponDamage = 1;
				tempMeleeDamage = 1;
				meleeCooldown = 500;
				reloadTime = 1350;
				break;

			case Difficulty::NORMAL:
				health = 17;
				tempMaxHealth = 17;
				weaponDamage = 2;
				tempMeleeDamage = 1;
				meleeCooldown = 400;
				reloadTime = 1000;
				break;

			case Difficulty::HARD:
				health = 22;
				tempMaxHealth = 22;
				weaponDamage = 3;
				tempMeleeDamage = 1;
				meleeCooldown = 400;
				reloadTime = 900;
				break;

			case Difficulty::VERY_HARD:
				health = 28;
				tempMaxHealth = 28;
				weaponDamage = 4;
				tempMeleeDamage = 1;
				meleeCooldown = 300;
				reloadTime = 750;
				break;
			}

			meleeDamage = Property<float>(tempMeleeDamage, 0.0f, tempMeleeDamage, tempMeleeDamage);
			maxHealth = Property<float>(tempMaxHealth, 0.0f, tempMaxHealth, tempMaxHealth);
			
			if (weapon == nullptr)
				weapon = new CR::Weapons::Weapon("SB Gun", weaponDamage, 0.58f, 0.37f, 400, 7, 7, 1, false, reloadTime);

			inventory.pickUpItem(weapon);
			hWalk = hSpeed;
			vWalk = vSpeed;
		}

		virtual void tick();
	};
}
