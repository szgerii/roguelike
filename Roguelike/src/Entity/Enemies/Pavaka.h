#pragma once
#include <vector>
#include "Entity/Enemies/Enemy.h"

namespace CR::Entities {
	class Pavaka : public Enemy {
	protected:
		//std::vector<Vector2<int>> path;
		//unsigned int pathIndex = 0;
		Vector2<float> lastPlayerPos;
		Vector2<int> targetCell;
	
	public:
		Pavaka(const Vector2<float>& start)
			: Enemy('&', FOREGROUND_GREEN, start, 27), lastPlayerPos(-1, -1), targetCell(-1, -1) {
			float tempMaxHealth, tempMeleeDamage;

			switch (Engine::getDifficulty()) {
			case Difficulty::EASY:
				health = 23;
				tempMaxHealth = 23;
				tempMeleeDamage = 6;
				meleeCooldown = 1200;
				break;

			case Difficulty::NORMAL:
				health = 27;
				tempMaxHealth = 27;
				tempMeleeDamage = 7;
				meleeCooldown = 1000;
				break;

			case Difficulty::HARD:
				health = 33;
				tempMaxHealth = 33;
				tempMeleeDamage = 8;
				meleeCooldown = 900;
				break;

			case Difficulty::VERY_HARD:
				health = 37;
				tempMaxHealth = 37;
				tempMeleeDamage = 10;
				meleeCooldown = 900;
				break;
			}

			meleeDamage = Property<float>(tempMeleeDamage, 0.0f, tempMeleeDamage, tempMeleeDamage);
			maxHealth = Property<float>(tempMaxHealth, 0.0f, tempMaxHealth, tempMaxHealth);

			vWalk = 0.07f;
			hWalk = 0.13f;
		}

		virtual void tick();
	};
}