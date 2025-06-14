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
		if (stunActive) {
			if (getCurrentTime() - stunStart >= stunDuration) {
				stunActive = false;
				stunStart = 0;
				stunDuration = 0;
			} else {
				return;
			}
		}

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

		if (health > maxHealth.getValue())
			health = maxHealth.getValue();
	}

	void Entity::stun(int stunDurationMs) {
		stunDuration = stunDuration < stunDurationMs ? stunDurationMs : stunDuration;
		stunStart = getCurrentTime();
		stunActive = true;
	}

	void Entity::meleeAttack(Direction direction) {
		if (getCurrentTime() - lastMeleeTime < meleeCooldown)
			return;

		int x, y, offset;
		switch (direction) {
		case Direction::LEFT:
		case Direction::RIGHT:
			offset = direction == Direction::LEFT ? -1 : 1;
			x = (int)round(position.x) + offset;

			for (int i = -1; i <= 1; i++) {
				y = (int)round(position.y) + i;
				Engine::addGameObject(new Particles::TrackingParticle('#', skinColor, 100, this, { offset, i }));

				GameObject* collObj = Engine::objectFromCoord({ x, y });
				if (collObj == nullptr || collObj == this || collObj->getType() == type)
					continue;

				collObj->hurt(meleeDamage.getValue(), this);
			}

			if (x != 1) {
				Engine::addGameObject(new Particles::TrackingParticle('#', skinColor, 100, this, { offset * 2, 0 }));
				GameObject* collObj = Engine::objectFromCoord({ (int)round(position.x) + offset * 2, y - 1 });
				
				if (collObj == nullptr || collObj == this || collObj->getType() == type)
					break;

				collObj->hurt(meleeDamage.getValue(), this);
			}

			break;

		case Direction::UP:
		case Direction::DOWN:
			offset = direction == Direction::UP ? -1 : 1;
			y = (int)round(position.y) + offset;

			for (int i = -1; i <= 1; i++) {
				x = (int)round(position.x) + i;

				Engine::addGameObject(new Particles::TrackingParticle('#', skinColor, 100, this, { i, offset }));

				GameObject* collObj = Engine::objectFromCoord({ x, y });
				if (collObj == nullptr || collObj == this || collObj->getType() == type)
					continue;

				collObj->hurt(meleeDamage.getValue(), this);
			}

			if (y != 1) {
				Engine::addGameObject(new Particles::TrackingParticle('#', skinColor, 100, this, { 0, offset * 2 }));
				GameObject* collObj = Engine::objectFromCoord({ x - 1, (int)round(position.y) + offset * 2 });
				if (collObj == nullptr || collObj == this || collObj->getType() == type)
					break;

				collObj->hurt(meleeDamage.getValue(), this);
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

	float Entity::getMaxHealth() {
		return maxHealth.getValue();
	}

	Property<float>& Entity::getMaxHealthProp() {
		return maxHealth;
	}

	Property<float>& Entity::getMeleeDamageProp() {
		return meleeDamage;
	}
}