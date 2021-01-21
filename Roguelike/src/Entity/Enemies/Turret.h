#pragma once
#include "Entity/Enemies/Enemy.h"
#include "Weapon/Enemy/BoomBoomLine.h"
#include "Utils.h"

namespace CR::Entities {
	class Turret : public Enemy {
	public:
		Turret(const Vector2<float> pos)
			: Enemy('U', FOREGROUND_GREEN, pos, 20) {
			float weaponDamage = 0, tempMaxHealth;

			switch (Engine::getDifficulty()) {
			case Difficulty::EASY:
				health = 16;
				tempMaxHealth = 16;
				weaponDamage = 5;
				break;

			case Difficulty::NORMAL:
				health = 18;
				tempMaxHealth = 18;
				weaponDamage = 6;
				break;

			case Difficulty::HARD:
				health = 20;
				tempMaxHealth = 20;
				weaponDamage = 8;
				break;

			case Difficulty::VERY_HARD:
				health = 22;
				tempMaxHealth = 22;
				weaponDamage = 9;
				break;
			}
			meleeDamage = Property<float>(0.0f, 0.0f, 0.0f, 0.0f);
			meleeCooldown = 0;

			meleeDamage = Property<float>(tempMaxHealth, 0.0f, tempMaxHealth, tempMaxHealth);

			inventory.pickUpItem(new Weapons::BoomBoomLine(weaponDamage));
		}

		virtual void tick();
	};
}
