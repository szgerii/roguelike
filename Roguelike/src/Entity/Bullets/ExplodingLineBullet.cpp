#include "ExplodingLineBullet.h"

namespace CR::Entities {
	void ExplodingLineBullet::tick() {
		move({ xStep, yStep }, true);

		collisionCheck();
	}
}