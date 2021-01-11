#pragma once
#include "Object/Tiles/Tile.h"

namespace CR::Objects {
	class AmmoTile : public Tile {
	public:
		AmmoTile(const Vector2<int>& pos)
			: Tile(' ', BACKGROUND_BLUE | BACKGROUND_INTENSITY, pos) { }

		virtual void hurt(float amount, GameObject* from);
	};
}
