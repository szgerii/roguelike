#include "Weapon.h"
#include "Entity/Bullets/Bullet.h"
#include "Utils.h"

namespace CR::Weapons {
	void Weapon::changeOwner(GameObject* owner) {
		this->owner = owner;
	}

	void Weapon::setStatIndex(int index) {
		statIndex = index;
		drawStat();
	}

	void Weapon::fire(Direction direction) {
		namespace chr = std::chrono;

		if (magazine <= 0 || getCurrentTime() - lastFireTime < cooldown || owner == nullptr)
			return;

		Entities::Bullet* bullet = nullptr;
		Vector2<float> startPos = owner->getPos();
		if (direction == Direction::LEFT || direction == Direction::RIGHT) {
			startPos.x += direction == Direction::LEFT ? -1 : 1;
			bullet = new Entities::Bullet('-', FG_WHITE, startPos, hBulletSpeed, damage, direction, owner);
		} else {
			startPos.y += direction == Direction::UP ? -1 : 1;
			bullet = new Entities::Bullet('-', FG_WHITE, startPos, vBulletSpeed, damage, direction, owner);
		}
		
		bullet->addToIgnoreList(owner);
		Engine::addGameObject(bullet);
		magazine--;

		lastFireTime = getCurrentTime();
		drawStat();
	}

	void Weapon::reload() {
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
		ammo += amount;

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
			Engine::modifyGUIText(statIndex, std::string(name) + ": " + std::to_string(magazine) + "/" + std::to_string(magazineSize) + " (" + std::to_string(ammo) + ") [DPS: " + std::to_string(dps) + " - Max Ammo: " + std::to_string(maxAmmo) + " - Ammo Pickup: " + std::to_string(pickupSize) + "]");
	}
}