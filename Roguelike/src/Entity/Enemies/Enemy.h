#pragma once
#include "Entity/Entity.h"
#include "Particle/Healthbar.h"

namespace CR::Entities {
	class Enemy : public Entity {
	protected:
		Particles::Healthbar* healthbar;
	
	public:
		Enemy(char skin, unsigned short skinColor, Vector2<float> pos, float maxHealth)
			: Entity(skin, skinColor, pos, maxHealth)
		{
			vWalk = 0.2f;
			hWalk = 0.1f;
			healthbar = new Particles::Healthbar(this);
			Engine::addGameObject(healthbar);
		}

		virtual void die();
		virtual void tick();
	};
}