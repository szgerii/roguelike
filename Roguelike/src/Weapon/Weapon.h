#pragma once
#include <chrono>
#include "Engine/Game.h"
#include "Utils.h"

namespace CR::Weapons {
	class Weapon {
	public:
		// PICKUP: proper weapon stat display
		std::chrono::steady_clock::time_point lastFire;
		Weapon(float damage, float hBulletSpeed, float vBulletSpeed, int cooldownMs, int magSize, int maxAmmo)
			: damage(damage), hBulletSpeed(hBulletSpeed), vBulletSpeed(vBulletSpeed), cooldown(cooldownMs), magazineSize(magSize), magazine(magSize), maxAmmo(maxAmmo), ammo(magSize), statIndex(-1) {}

		virtual void changeOwner(GameObject* owner);
		virtual void setStatIndex(int index);
		virtual void fire(Direction direction);
		virtual void reload();
		virtual int addAmmo(int amount);
		
		virtual int getMagazine();
		virtual int getMagazineSize();
		virtual int getAmmo();
		virtual int getMaxAmmo();

	protected:
		float damage, hBulletSpeed, vBulletSpeed;
		int cooldown, magazineSize, magazine, maxAmmo, ammo, statIndex;
		GameObject* owner;

		virtual void updateStat();
	};
}