#pragma once
#include "Engine/GameObject.h"
#include "Entity/Inventory.h"
#include "Weapon/Weapon.h"
#include "Property.h"

namespace CR::Entities {
	class Entity : public GameObject
	{
	protected:
		Property<float> maxHealth, meleeDamage;
		float health, hWalk, hRun, vWalk, vRun;
		int meleeCooldown, stunDuration;
		Inventory<3> inventory;
		long long lastMeleeTime, stunStart;
		bool stunActive;

	public:

		Entity(char skin, unsigned short skinColor, const Vector2<float>& pos, float maxHealth, int meleeCooldownMs = 400)
			: GameObject(skin, skinColor, pos),
			maxHealth(maxHealth, 0.0f, maxHealth, maxHealth), meleeDamage(0.0f, 0.0f, 0.0f, 0.0f), health(maxHealth),
			meleeCooldown(meleeCooldownMs), stunDuration(0), inventory(this), stunActive(false) {}

		virtual float getHealth() const;
		virtual void move(const Vector2<float>& delta, bool force=false);
		virtual void moveTo(const Vector2<float>& position);
		virtual void hurt(float amount, GameObject* from);
		virtual void heal(float amount);
		virtual void stun(int stunDurationMs);
		virtual bool pickUpItem(Weapons::Weapon* item);
		virtual Weapons::Weapon* getCurrentWeapon();
		virtual float getMaxHealth();
		virtual Property<float>& getMaxHealthProp();
		virtual Property<float>& getMeleeDamageProp();

	protected:
		virtual void meleeAttack(Direction direction);
	};
}

