#pragma once
#include "Object/Tiles/Tile.h"

namespace CR::Objects {
	class HealthTile : public Tile {
	protected:
		int healthAmount;

	public:
		HealthTile(const Vector2<int>& pos, int healthAmount)
			: Tile(' ', BACKGROUND_GREEN | BACKGROUND_RED, pos), healthAmount(healthAmount) { }

		virtual void hurt(float amount, GameObject* from);
	};
}
