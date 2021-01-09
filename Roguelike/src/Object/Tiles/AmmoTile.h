#pragma once
#include "Object/Tiles/Tile.h"

namespace CR::Objects {
	class AmmoTile : public Tile {
	protected:
		int ammoAmount;

	public:
		AmmoTile(const Vector2<int>& pos, int ammoAmount)
			: Tile(' ', BACKGROUND_BLUE | BACKGROUND_INTENSITY, pos), ammoAmount(ammoAmount) { }

		virtual void hurt(float amount, GameObject* from);
	};
}
