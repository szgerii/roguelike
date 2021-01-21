#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "Entity/Bullets/ExplodingBullet.h"
#include "Engine/Game.h"
#include "Utils.h"

namespace CR::Entities {
	class ExplodingLineBullet : public ExplodingBullet {
	protected:
		float xStep, yStep;
	
	public:
		ExplodingLineBullet(char skin, unsigned short skinColor, const Vector2<int>& start, const Vector2<int>& target, float speed, float damage, GameObject* sender, bool initialCollisionCheck = false)
			: ExplodingBullet(skin, skinColor, { (float)start.x, (float)start.y }, 0, damage, Direction::LEFT, sender, initialCollisionCheck) {
			// Shout-out to Bácskai Kristóf for this
			float deltaX = target.x - position.x,
				  deltaY = target.y - position.y;

			float angle = (float)atan2(-deltaY, deltaX) + (float)(90 * (M_PI / 180));

			xStep = sin(angle) * speed;
			yStep = cos(angle) * speed;
		}

		virtual void tick();
	};
}
