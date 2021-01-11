#include <cmath>
#include "TrackingParticle.h"

namespace CR::Particles {
	void TrackingParticle::tick() {
		position = { round(trackedObj->getX()) + offset.x, round(trackedObj->getY()) + offset.y };

		Particle::tick();
	}
}