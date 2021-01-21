#pragma once
#include <cmath>
#include <chrono>
#include "Engine/Game.h"
#include "Engine/GameObject.h"
#include "Utils.h"

namespace CR::Weapons {
	class Weapon {
	protected:
		const char* name;
		float damage, hBulletSpeed, vBulletSpeed, dmgMult, pickupMult;
		int cooldown, magazineSize, magazine, maxAmmo, ammo, pickupSize, reloadTime, dps, statIndex;
		GameObject* owner;
		long long lastFireTime, reloadStart;
		bool infiniteAmmo, reloading = false;

	public:
		Weapon(const char* name, float damage, float hBulletSpeed, float vBulletSpeed, int cooldownMs, int magSize, int maxAmmo, int pickupSize, bool infiniteAmmo=false, int reloadTime = 0)
			: name(name), damage(damage), hBulletSpeed(hBulletSpeed), vBulletSpeed(vBulletSpeed), dmgMult(1), pickupMult(1), cooldown(cooldownMs), magazineSize(magSize), magazine(magSize), maxAmmo(maxAmmo), ammo(magSize), pickupSize(pickupSize), reloadTime(reloadTime), statIndex(-1), dps((int)round(1000 * (damage * dmgMult / cooldownMs))), infiniteAmmo(infiniteAmmo) {}

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
		virtual void setDamageMultiplier(float dmgMult);
		virtual float getDamageMultiplier();
		virtual void setPickupMultiplier(float pickupMult);
		virtual float getPickupMultiplier();
		virtual void calculateDPS();

	protected:
		virtual void drawStat();
		virtual void instantReload();
		virtual GameObject* getNewBullet(const Vector2<float>& startPos, Direction direction);
	};
}