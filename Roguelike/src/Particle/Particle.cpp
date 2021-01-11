#include "Particle.h"
#include "Engine/Game.h"

namespace CR::Particles {
	void Particle::tick() {
		if (getCurrentTime() - start > duration)
			die();
	}
	
	void Particle::render(CHAR_INFO* screen, int screenWidth, int screenHeight) const {
		Engine::addToOverlay({ (int)position.x, (int)position.y }, skin, skinColor);
	}
}