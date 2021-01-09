#pragma once
#include <vector>
#include "Engine/GameObject.h"
#include "Engine/Game.h"
#include "Entity/Entity.h"
#include "Entity/Player.h"

namespace CR::Entities {
	class Bullet : public Entity {
	public:
		Bullet(char skin, unsigned short skinColor, Vector2<float> start, float speed, float damage, Direction direction)
			: Entity(skin, skinColor, start, 1), speed(speed), damage(damage), direction(direction) {}

		Bullet(char skin, unsigned short skinColor, float startX, float startY, float speed, float damage, Direction direction)
			: Entity(skin, skinColor, startX, startY, 1), speed(speed), damage(damage), direction(direction) {}

		virtual void tick();
		virtual void hurt() {} // can't take damage
		virtual void heal() {} // can't heal
		virtual void die();
		virtual void addToIgnoreList(GameObject* obj);
		
	protected:
		virtual void handleHit(GameObject* obj);

	protected:
		float speed, damage;
		Direction direction;
		std::vector<GameObject*> ignoreList;
	};
}

