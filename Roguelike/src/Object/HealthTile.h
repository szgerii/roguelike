#pragma once
#include "Tile.h"

namespace CR::Objects {
	class HealthTile : public Tile {
	protected:
		int healthAmount;

	public:
		HealthTile(const Vector2<int>& pos, int healthAmount)
			: Tile(' ', BACKGROUND_GREEN, pos), healthAmount(healthAmount) { }

		virtual void hurt(float amount);
	};
}
