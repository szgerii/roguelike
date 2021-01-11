#include <cmath>
#include "Entity.h"
#include "Engine/Game.h"
#include "Utils.h"
#include "Particle/TrackingParticle.h"

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
			position.x = (float)mapWidth;
		if (position.y >= mapHeight)
			position.y = (float)mapHeight;
	}

	void Entity::moveTo(const Vector2<float>& position) {
		this->position = position;
	}

	void Entity::hurt(float amount, GameObject* from) {
		health -= amount;
		
		if (health <= 0)
			die();
	}

	void Entity::heal(float amount) {
		health += amount;

		if (health > maxHealth)
			health = maxHealth;
	}

	void Entity::meleeAttack(Direction direction) {
		if (getCurrentTime() - lastMeleeTime < meleeCooldown)
			return;

		int x, y, offset;
		switch (direction) {
		case LEFT:
		case RIGHT:
			offset = direction == LEFT ? -1 : 1;
			x = (int)round(position.x) + offset;

			for (int i = -1; i <= 1; i++) {
				y = (int)round(position.y) + i;
				Engine::addGameObject(new Particles::TrackingParticle('#', FOREGROUND_BLUE | FOREGROUND_GREEN, 100, this, { offset, i }));

				GameObject* collObj = Engine::objectFromCoord({ x, y });
				if (collObj == nullptr)
					continue;

				collObj->hurt(meleeDamage, this);
			}

			if (x != 1) {
				Engine::addGameObject(new Particles::TrackingParticle('#', FOREGROUND_BLUE | FOREGROUND_GREEN, 100, this, { offset * 2, 0 }));
				GameObject* collObj = Engine::objectFromCoord({ (int)round(position.x) + offset * 2, y - 1 });
				
				if (collObj == nullptr)
					break;

				collObj->hurt(meleeDamage, this);
			}

			break;

		case UP:
		case DOWN:
			offset = direction == UP ? -1 : 1;
			y = (int)round(position.y) + offset;

			for (int i = -1; i <= 1; i++) {
				x = (int)round(position.x) + i;

				Engine::addGameObject(new Particles::TrackingParticle('#', FOREGROUND_BLUE | FOREGROUND_GREEN, 100, this, { i, offset }));

				GameObject* collObj = Engine::objectFromCoord({ x, y });
				if (collObj == nullptr)
					continue;

				collObj->hurt(meleeDamage, this);
			}

			if (y != 1) {
				Engine::addGameObject(new Particles::TrackingParticle('#', FOREGROUND_BLUE | FOREGROUND_GREEN, 100, this, { 0, offset * 2 }));
				GameObject* collObj = Engine::objectFromCoord({ x - 1, (int)round(position.y) + offset * 2 });
				if (collObj == nullptr)
					break;

				collObj->hurt(meleeDamage, this);
			}

			break;
		}

		lastMeleeTime = getCurrentTime();
	}

	bool Entity::pickUpItem(Weapons::Weapon* item) {
		return inventory.pickUpItem(item);
	}

	Weapons::Weapon* Entity::getCurrentWeapon() {
		return inventory.getCurrentItem();
	}
}