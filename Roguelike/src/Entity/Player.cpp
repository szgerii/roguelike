#include <string>
#include "Player.h"
#include "Engine/Game.h"

int debugTimeout = 0;

namespace CR::Entities {
	void Player::tick() {
		float vDelta = 0, hDelta = 0;
		bool sprintActive = Engine::keyPressed('o');

		if (Engine::keyPressed('w'))
			vDelta -= sprintActive ? vRun : vWalk;
		if (Engine::keyPressed('s'))
			vDelta += sprintActive ? vRun : vWalk;
		if (Engine::keyPressed('a'))
			hDelta -= sprintActive ? hRun : hWalk;
		if (Engine::keyPressed('d'))
			hDelta += sprintActive ? hRun : hWalk;

		if (Engine::keyPressed('r'))
			currentWeapon->reload();

		// DEBUG (TODO: remove)
		if (Engine::keyPressed('z') && debugTimeout == 0) {
			currentWeapon->addAmmo(30);
			debugTimeout = 15;
		}

		if (Engine::keyPressed('j'))
			currentWeapon->fire(Direction::LEFT);
		else if (Engine::keyPressed('l'))
			currentWeapon->fire(Direction::RIGHT);
		else if (Engine::keyPressed('i'))
			currentWeapon->fire(Direction::UP);
		else if (Engine::keyPressed('k'))
			currentWeapon->fire(Direction::DOWN);

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

	void Player::changeWeapon(Weapons::Weapon* newWeapon, bool noDelete) {
		if (currentWeapon != nullptr)
			currentWeapon->setStatIndex(-1);

		newWeapon->setStatIndex(weaponStatIndex);

		Entity::changeWeapon(newWeapon, noDelete);
	}
}