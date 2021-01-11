#pragma once
#include <chrono>
#include "Engine/Game.h"
#include "Utils.h"

namespace CR::Weapons {
	class Weapon {
	protected:
		const char* name;
		float damage, hBulletSpeed, vBulletSpeed;
		int cooldown, magazineSize, magazine, maxAmmo, ammo, pickupSize, dps, statIndex;
		GameObject* owner;
		long long lastFireTime;

	public:
		Weapon(const char* name, float damage, float hBulletSpeed, float vBulletSpeed, int cooldownMs, int magSize, int maxAmmo, int pickupSize)
			: name(name), damage(damage), hBulletSpeed(hBulletSpeed), vBulletSpeed(vBulletSpeed), cooldown(cooldownMs), magazineSize(magSize), magazine(magSize), maxAmmo(maxAmmo), ammo(magSize), pickupSize(pickupSize), statIndex(-1), dps(1000 * damage / cooldownMs) {}

		virtual void changeOwner(GameObject* owner);
		virtual void setStatIndex(int index);
		virtual void fire(Direction direction);
		virtual void reload();
		virtual int addAmmo(int amount);
		
		virtual int getMagazine();
		virtual int getMagazineSize();
		virtual int getAmmo();
		virtual int getMaxAmmo();
		virtual int getPickupSize();

	protected:
		virtual void drawStat();
	};
}