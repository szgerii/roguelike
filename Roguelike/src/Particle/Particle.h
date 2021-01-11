#pragma once
#include "Engine/CustomGameObject.h"
#include "Utils.h"

namespace CR::Particles {
	class Particle : public CustomGameObject {
	protected:
		int duration;
		long long start;

	public:
		Particle(char skin, unsigned short skinColor, const Vector2<int>& pos, int durationMs)
			: CustomGameObject(skin, skinColor, { (float)pos.x, (float)pos.y }), duration(durationMs) {
			start = getCurrentTime();
		}

		virtual void tick();
		virtual void render(CHAR_INFO* screen, int screenWidth, int screenHeight) const;
	};
}
