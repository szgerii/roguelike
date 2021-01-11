#pragma once
#include "Engine/GameObject.h"
#include "Entity/Inventory.h"
#include "Weapon/Weapon.h"

namespace CR::Entities {
	class Entity : public GameObject
	{
	protected:
		float health, meleeDamage, hWalk, hRun, vWalk, vRun;
		int meleeCooldown;
		Inventory<3> inventory;
		long long lastMeleeTime;

	public:
		float maxHealth;

		Entity(char skin, unsigned short skinColor, Vector2<float> pos, float maxHealth, int meleeCooldownMs = 400)
			: GameObject(skin, skinColor, pos), health(maxHealth), maxHealth(maxHealth), meleeCooldown(meleeCooldownMs), inventory(this) {}
		
		virtual float getHealth() const;
		virtual void move(const Vector2<float>& delta, bool force=false);
		virtual void moveTo(const Vector2<float>& position);
		virtual void hurt(float amount, GameObject* from);
		virtual void heal(float amount);
		virtual void die();
		virtual bool pickUpItem(Weapons::Weapon* item);
		virtual Weapons::Weapon* getCurrentWeapon();

	protected:
		virtual void meleeAttack(Direction direction);
	};
}

