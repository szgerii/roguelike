#include "Weapon.h"
#include "Utils.h"
#include "Entity/Bullets/Bullet.h"

namespace CR::Weapons {
	void Weapon::changeOwner(GameObject* owner) {
		this->owner = owner;
	}

	void Weapon::setStatIndex(int index) {
		statIndex = index;
		drawStat();
	}

	void Weapon::fire(Direction direction) {
		if (reloading) {
			if (getCurrentTime() - reloadStart >= reloadTime) {
				instantReload();
				reloading = false;
			} else {
				return;
			}
		}

		if (getCurrentTime() - lastFireTime < cooldown || owner == nullptr)
			return;

		if (!infiniteAmmo && magazine <= 0)
			return;

		Vector2<float> startPos = owner->getPos();
		if (direction == Direction::LEFT || direction == Direction::RIGHT)
			startPos.x += direction == Direction::LEFT ? -1 : 1;
		else
			startPos.y += direction == Direction::UP ? -1 : 1;
		
		Entities::Bullet* bullet = (Entities::Bullet*)getNewBullet(startPos, direction);
		bullet->addToIgnoreList(owner);
		Engine::addGameObject(bullet);
		
		if (!infiniteAmmo)
			magazine--;

		lastFireTime = getCurrentTime();
		drawStat();
	}

	void Weapon::reload() {
		if (reloading)
			return;

		if (reloadTime == 0) {
			instantReload();
			return;
		}

		reloading = true;
		reloadStart = getCurrentTime();
	}

	void Weapon::instantReload() {
		int magazineDiff = magazineSize - magazine;
		int amount = ammo >= magazineDiff ? magazineDiff : ammo;

		magazine += amount;
		ammo -= amount;

		drawStat();
	}

	/// <summary>
	/// Adds ammo to the weapon's ammo storage
	/// </summary>
	/// <returns>The amount of ammo remaining (the weapon can't have more ammo than maxAmmo)</returns>
	int Weapon::addAmmo(int amount) {
		ammo += (int)round(amount * pickupMult);

		if (ammo > maxAmmo) {
			int diff = ammo - maxAmmo;
			ammo -= diff;
			drawStat();
			return diff;
		}

		drawStat();
		return 0;
	}

	int Weapon::getMagazine() {
		return magazine;
	}

	int Weapon::getMagazineSize() {
		return magazineSize;
	}

	int Weapon::getAmmo() {
		return ammo;
	}

	int Weapon::getMaxAmmo() {
		return maxAmmo;
	}

	int Weapon::getPickupSize() {
		return pickupSize;
	}

	void Weapon::drawStat() {
		if (statIndex != -1)
			Engine::modifyGUIText(statIndex, std::string(name) + ": " + std::to_string(magazine) + "/" + std::to_string(magazineSize) + " (" + std::to_string(ammo) + ") [" + std::to_string(dps) + " - " + std::to_string(maxAmmo) + " - " + std::to_string((int)round(pickupSize * pickupMult)) + "]");
	}

	GameObject* Weapon::getNewBullet(const Vector2<float>& startPos, Direction direction) {
		if (direction == Direction::LEFT || direction == Direction::RIGHT)
			return new Entities::Bullet('-', owner->getSkinColor(), startPos, hBulletSpeed, damage * dmgMult, direction, owner, true);
		else
			return new Entities::Bullet('-', owner->getSkinColor(), startPos, vBulletSpeed, damage * dmgMult, direction, owner, true);
	}

	void Weapon::setDamageMultiplier(float dmgMult) {
		this->dmgMult = dmgMult;
		calculateDPS();
	}

	float Weapon::getDamageMultiplier() {
		return dmgMult;
	}

	void Weapon::setPickupMultiplier(float pickupMult) {
		this->pickupMult = pickupMult;
	}

	float Weapon::getPickupMultiplier() {
		return pickupMult;
	}

	void Weapon::calculateDPS() {
		dps = (int)round(1000 * (damage * dmgMult / cooldown));
	}
}