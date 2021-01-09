#include "Weapon.h"
#include "Entity/Bullets/Bullet.h"

#ifndef WHITE
#define WHITE FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN
#endif

namespace CR::Weapons {
	void Weapon::changeOwner(GameObject* owner) {
		this->owner = owner;
	}

	void Weapon::setStatIndex(int index) {
		statIndex = index;
		updateStat();
	}

	void Weapon::fire(Direction direction) {
		namespace chrono = std::chrono;

		if (magazine <= 0 || chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - lastFire).count() < cooldown || owner == nullptr)
			return;

		Entities::Bullet* bullet = nullptr;
		Vector2<float> startPos = owner->getPos();
		if (direction == Direction::LEFT || direction == Direction::RIGHT) {
			startPos.x += direction == Direction::LEFT ? -1 : 1;
			bullet = new Entities::Bullet('-', WHITE, startPos, hBulletSpeed, damage, direction);
		} else {
			startPos.y += direction == Direction::UP ? -1 : 1;
			bullet = new Entities::Bullet('-', WHITE, startPos, vBulletSpeed, damage, direction);
		}
		
		bullet->addToIgnoreList(owner);
		Engine::addGameObject(bullet);
		magazine--;

		lastFire = chrono::steady_clock::now();
		updateStat();
	}

	void Weapon::reload() {
		int magazineDiff = magazineSize - magazine;
		int amount = ammo >= magazineDiff ? magazineDiff : ammo;

		magazine += amount;
		ammo -= amount;

		updateStat();
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
			updateStat();
			return diff;
		}

		updateStat();
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

	void Weapon::updateStat() {
		if (statIndex != -1)
			Engine::modifyGUIText(statIndex, "Weapon: " + std::to_string(magazine) + "/" + std::to_string(magazineSize) + " (" + std::to_string(ammo) + ")");
	}
}