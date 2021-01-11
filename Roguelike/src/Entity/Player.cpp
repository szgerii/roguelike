#include <string>
#include "Player.h"
#include "Engine/Game.h"

int debugTimeout = 0;

namespace CR::Entities {
	void Player::tick() {
		float vDelta = 0, hDelta = 0;
		bool sprintActive = Engine::keyPressed('o');
		Weapons::Weapon* currentWeapon = inventory.getCurrentItem();

		// Movement
		if (Engine::keyPressed('w'))
			vDelta -= sprintActive ? vRun : vWalk;
		if (Engine::keyPressed('s'))
			vDelta += sprintActive ? vRun : vWalk;
		if (Engine::keyPressed('a'))
			hDelta -= sprintActive ? hRun : hWalk;
		if (Engine::keyPressed('d'))
			hDelta += sprintActive ? hRun : hWalk;

		// Reload weapon
		if (Engine::keyPressed('r'))
			currentWeapon->reload();

		// DEBUG (TODO: remove)
		if (Engine::keyPressed('z') && debugTimeout == 0) {
			// DEBUG: add ammo
			currentWeapon->addAmmo(30);
			debugTimeout = 15;
		}

		if (Engine::keyPressed('\t') && debugTimeout == 0) {
			// next weapon
			inventory.nextItem();
			debugTimeout = 15;
		} else if (Engine::keyPressed('q')) {
			// drop item
			inventory.dropItem();
		} else if (Engine::keyPressed(' ') || currentWeapon == nullptr) {
			// check melee
			if (Engine::keyPressed('j'))
				meleeAttack(LEFT);
			else if (Engine::keyPressed('l'))
				meleeAttack(RIGHT);
			else if (Engine::keyPressed('i'))
				meleeAttack(UP);
			else if (Engine::keyPressed('k'))
				meleeAttack(DOWN);
		} else {
			// check shooting
			if (Engine::keyPressed('j'))
				currentWeapon->fire(LEFT);
			else if (Engine::keyPressed('l'))
				currentWeapon->fire(RIGHT);
			else if (Engine::keyPressed('i'))
				currentWeapon->fire(UP);
			else if (Engine::keyPressed('k'))
				currentWeapon->fire(DOWN);
		}

		/*if (vDelta != 0 && hDelta != 0) {
			vDelta /= 2;
			hDelta /= 2;
		}*/

		move({ hDelta, vDelta });

		if (debugTimeout > 0)
			debugTimeout--;
	}

	void Player::hurt(float amount, GameObject* from) {
		Entity::hurt(amount, from);
		Engine::modifyGUIText(healthStatIndex, "Health: " + std::to_string((int) health) + "/" + std::to_string((int) maxHealth));
	}

	void Player::heal(float amount) {
		Entity::heal(amount);
		Engine::modifyGUIText(healthStatIndex, "Health: " + std::to_string((int) health) + "/" + std::to_string((int) maxHealth));
	}

	void Player::die() {
		Engine::removeGameObject(healthbar, true);

		Entity::die();
	}
}