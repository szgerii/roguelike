#pragma once
#include "Engine/GameObject.h"
#include "Entity/Entity.h"
#include "Particle/TrackingParticle.h"

namespace CR::Particles {
	class Healthbar : public TrackingParticle {
	public:
		unsigned short activeColor = BACKGROUND_RED | BACKGROUND_INTENSITY, inactiveColor = BACKGROUND_RED;

		Healthbar(Entities::Entity* target)
			: TrackingParticle(' ', 0, 100000, target, { 0, -1 }) {}

		virtual void render(CHAR_INFO* screen, int mapWidth, int mapHeight) const;
	};
}
