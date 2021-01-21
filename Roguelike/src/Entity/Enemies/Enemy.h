#pragma once
#include "Entity/Entity.h"
#include "Particle/Healthbar.h"
#include "Utils.h"

namespace CR::Entities {
	class Enemy : public Entity {
	protected:
		Particles::Healthbar* healthbar;
	
	public:
		Enemy(char skin, unsigned short skinColor, const Vector2<float>& pos, float maxHealth)
			: Entity(skin, skinColor, pos, maxHealth) {
			type = Type::ENEMY;
			healthbar = new Particles::Healthbar(this);
			Engine::addGameObject(healthbar);
		}

		~Enemy() {
			Engine::removeGameObject(healthbar, true);
		}

		virtual void die();
		virtual void tick();
		virtual void createHealthbar();
		virtual void deleteHealthbar();
		virtual void setFreeze(bool freeze);
	};
}