#include <stdexcept>
#include <cmath>
#include "Entity.h"
#include "Engine/Game.h"

namespace CR::Entities {
	float Entity::getHealth() const {
		return health;
	}

	void Entity::move(const Vector2<float>& delta, bool force) {
		int mapWidth = Engine::getMapWidth();
		int mapHeight = Engine::getMapHeight();

		if (delta.x > 1 || delta.y > 1 || delta.x < -1 || delta.y < -1)
			throw std::invalid_argument("Delta X or Y did not fall into the following range: -1 <= x <= 1 (use the moveTo method instead to move without collision checking or override this method in a derived class).");

		if (Engine::collisionAtCoord((int) round(position.x + delta.x), (int) round(position.y + delta.y), this) && !force)
			return;
		
		position.x += delta.x;
		position.y += delta.y;

		if (position.x < 0)
			position.x = 0;
		if (position.y < 0)
			position.x = 0;
		if (position.x >= mapWidth)
			position.x = mapWidth;
		if (position.y >= mapHeight)
			position.y = mapHeight;
	}

	void Entity::moveTo(const Vector2<float>& position) {
		this->position = position;
	}

	void Entity::hurt(float amount) {
		health -= amount;
		
		if (health <= 0)
			die();
	}

	void Entity::heal(float amount) {
		health += amount;

		if (health > maxHealth)
			health = maxHealth;
	}

	Weapons::Weapon* Entity::getWeapon() const {
		return currentWeapon;
	}

	void Entity::changeWeapon(Weapons::Weapon* newWeapon, bool noDelete) {		
		if (currentWeapon != nullptr) {
			currentWeapon->changeOwner(nullptr);

			if (!noDelete)
				delete currentWeapon;
		}

		currentWeapon = newWeapon;
		currentWeapon->changeOwner(this);
	}
}