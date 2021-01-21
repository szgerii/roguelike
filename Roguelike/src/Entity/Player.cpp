#include <string>
#include "Player.h"
#include "Engine/Game.h"

int debugTimeout = 0;

namespace CR::Entities {
	void Player::tick() {
		float vDelta = 0, hDelta = 0;
		bool sprintActive = Engine::keyPressed('o');
		bool slowActive = Engine::keyPressed('q');
		Weapons::Weapon* currentWeapon = inventory.getCurrentItem();

		// Movement
		if (Engine::keyPressed('w'))
			vDelta -= sprintActive ? vRun : slowActive ? vSlow : vWalk;
		if (Engine::keyPressed('s'))
			vDelta += sprintActive ? vRun : slowActive ? vSlow : vWalk;
		if (Engine::keyPressed('a'))
			hDelta -= sprintActive ? hRun : slowActive ? hSlow : hWalk;
		if (Engine::keyPressed('d'))
			hDelta += sprintActive ? hRun : slowActive ? hSlow : hWalk;

		// Reload weapon
		if (Engine::keyPressed('r') && currentWeapon != nullptr)
			currentWeapon->reload();

		if (Engine::keyPressed('\t') && debugTimeout == 0) {
			// next weapon
			inventory.nextItem();
			debugTimeout = 15;
		} else if (Engine::keyPressed('v')) {
			// drop item
			inventory.dropItem();
		} else if (Engine::keyPressed('e') || currentWeapon == nullptr || currentWeapon->getMagazine() == 0) {
			// check melee
			if (Engine::keyPressed('j'))
				meleeAttack(Direction::LEFT);
			else if (Engine::keyPressed('l'))
				meleeAttack(Direction::RIGHT);
			else if (Engine::keyPressed('i'))
				meleeAttack(Direction::UP);
			else if (Engine::keyPressed('k'))
				meleeAttack(Direction::DOWN);
		} else {
			// check shooting
			if (Engine::keyPressed('j'))
				currentWeapon->fire(Direction::LEFT);
			else if (Engine::keyPressed('l'))
				currentWeapon->fire(Direction::RIGHT);
			else if (Engine::keyPressed('i'))
				currentWeapon->fire(Direction::UP);
			else if (Engine::keyPressed('k'))
				currentWeapon->fire(Direction::DOWN);
		}

		if (vDelta != 0 && hDelta != 0) {
			vDelta /= 1.2f;
			hDelta /= 1.2f;
		}

		Engine::modifyGUIText(roomStatIndex, "Room: " + std::to_string(Engine::getRoomCount()) + " (" + std::to_string(Engine::getCurrentWaveNum()) + "/" + std::to_string(Engine::getNumOfWaves()) + ")");

		move({ hDelta, vDelta });

		if (debugTimeout > 0)
			debugTimeout--;
	}

	void Player::hurt(float amount, GameObject* from) {
		Entity::hurt(amount * damageReduction.getValue(), from);
		Engine::modifyGUIText(healthStatIndex, "Health: " + std::to_string((int) health) + "/" + std::to_string((int) maxHealth.getValue()));
	}

	void Player::heal(float amount) {
		Entity::heal(amount);
		Engine::modifyGUIText(healthStatIndex, "Health: " + std::to_string((int) health) + "/" + std::to_string((int) maxHealth.getValue()));
	}

	void Player::createHealthbar() {
		if (healthbar != nullptr)
			deleteHealthbar();

		healthbar = new Particles::Healthbar(this, 0, FOREGROUND_RED | FOREGROUND_INTENSITY);
		Engine::addGameObject(healthbar);
	}

	void Player::deleteHealthbar() {
		if (healthbar == nullptr)
			return;

		Engine::removeGameObject(healthbar, true);
		healthbar = nullptr;
	}

	void Player::die() {
		Engine::removeGameObject(healthbar, true);
		Engine::gameOver();

		Entity::die();
	}

	void Player::setFreeze(bool freeze) {
		if (freeze)
			deleteHealthbar();
		else
			createHealthbar();

		this->freeze = freeze;
	}

	bool Player::pickUpItem(Weapons::Weapon* item) {
		item->setPickupMultiplier(ammoPickupMultiplier.getValue());
		item->setDamageMultiplier(weaponDamageMultiplier.getValue());
		
		return Entity::pickUpItem(item);
	}

	float Player::getAmmoPickupMultiplier() {
		return ammoPickupMultiplier.getValue();
	}

	Property<float>& Player::getWeaponDamageMultiplierProp() {
		return weaponDamageMultiplier;
	}

	Property<float>& Player::getAmmoPickupMultiplierProp() {
		return ammoPickupMultiplier;
	}

	Property<float>& Player::getDamageReductionProp() {
		return damageReduction;
	}

	void Player::upgradeWeaponDamage() {
		if (weaponDamageMultiplier.canUltimateUpgrade())
			weaponDamageMultiplier.upgrade(true);
		else
			weaponDamageMultiplier.upgrade();

		for (int i = 0; i < 3; i++) {
			if (inventory.getCurrentItem() != nullptr)
				inventory.getCurrentItem()->setDamageMultiplier(weaponDamageMultiplier.getValue());

			inventory.nextItem();
		}
	}

	void Player::upgradePickupModifier() {
		if (ammoPickupMultiplier.canUltimateUpgrade())
			ammoPickupMultiplier.upgrade(true);
		else
			ammoPickupMultiplier.upgrade();

		for (int i = 0; i < 3; i++) {
			if (inventory.getCurrentItem() != nullptr)
				inventory.getCurrentItem()->setPickupMultiplier(ammoPickupMultiplier.getValue());

			inventory.nextItem();
		}
	}

	void Player::downgradeWeaponDamage() {
		weaponDamageMultiplier.downgrade(ammoPickupMultiplier.getStep());

		for (int i = 0; i < 3; i++) {
			if (inventory.getCurrentItem() != nullptr)
				inventory.getCurrentItem()->setDamageMultiplier(weaponDamageMultiplier.getValue());

			inventory.nextItem();
		}
	}

	void Player::downgradePickupModifier() {
		ammoPickupMultiplier.downgrade(ammoPickupMultiplier.getStep());

		for (int i = 0; i < 3; i++) {
			if (inventory.getCurrentItem() != nullptr)
				inventory.getCurrentItem()->setPickupMultiplier(ammoPickupMultiplier.getValue());

			inventory.nextItem();
		}
	}
}