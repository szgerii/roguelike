#pragma once
#include "Entity/Entity.h"

namespace CR::Entities {
	class Enemy : public Entity {
	public:
		Enemy(char skin, unsigned short skinColor, Vector2<float> pos, int maxHealth)
			: Entity(skin, skinColor, pos, maxHealth)
		{
			vWalk = 0.2f;
			hWalk = 0.1f;
		}

		virtual void tick();
	};
}