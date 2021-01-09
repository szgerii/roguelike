#pragma once
#include "Engine/GameObject.h"
#include "Weapon/Weapon.h"

namespace CR::Entities {
	class Entity : public GameObject
	{
	protected:
		float health;
		Weapons::Weapon* currentWeapon;
		float hWalk, hRun, vWalk, vRun;

	public:
		float maxHealth;

		Entity(char skin, unsigned short skinColor, Vector2<float> pos, float maxHealth)
			: GameObject(skin, skinColor, pos), health(maxHealth), maxHealth(maxHealth) {}
		
		Entity(char skin, unsigned short skinColor, float x, float y, float maxHealth)
			: GameObject(skin, skinColor, x, y), health(maxHealth), maxHealth(maxHealth) {}
		
		Entity(char skin, unsigned short skinColor, Vector2<float> pos, float maxHealth, Weapons::Weapon* weapon)
			: GameObject(skin, skinColor, pos), health(maxHealth), maxHealth(maxHealth), currentWeapon(weapon) {}

		Entity(char skin, unsigned short skinColor, float x, float y, float maxHealth, Weapons::Weapon* weapon)
			: GameObject(skin, skinColor, x, y), health(maxHealth), maxHealth(maxHealth), currentWeapon(weapon) {}

		virtual float getHealth() const;
		virtual void move(const Vector2<float>& delta, bool force=false);
		virtual void moveTo(const Vector2<float>& position);
		virtual void hurt(float amount);
		virtual void heal(float amount);
		virtual Weapons::Weapon* getWeapon() const;
		virtual void changeWeapon(Weapons::Weapon* newWeapon, bool noDelete=false);
	};
}

