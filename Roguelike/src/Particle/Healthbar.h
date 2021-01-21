#pragma once
#include "Engine/GameObject.h"
#include "Entity/Entity.h"
#include "Particle/TrackingParticle.h"

namespace CR::Particles {
	class Healthbar : public TrackingParticle {
	public:
		unsigned short activeColor, inactiveColor;

		Healthbar(Entities::Entity* target, unsigned short activeColor = 0, unsigned short inactiveColor = FOREGROUND_RED)
			: TrackingParticle('_', 0, 0, target, { 0, -1 }), activeColor(activeColor), inactiveColor(inactiveColor) {}

		virtual void tick() {}
		virtual void render(CHAR_INFO* screen, int mapWidth, int mapHeight) const;
	};
}
