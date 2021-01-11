#pragma once
#include "Particle/Particle.h"

namespace CR::Particles {
	class TrackingParticle : public Particle {
	protected:
		GameObject* trackedObj;
		const Vector2<int> offset;

	public:
		TrackingParticle(char skin, unsigned short skinColor, int durationMs, GameObject* trackedObj, const Vector2<int>& offset)
			: Particle(skin, skinColor, { (int)trackedObj->getX(), (int)trackedObj->getY() }, durationMs), trackedObj(trackedObj), offset(offset) {}
		
		virtual void tick();
	};
}
