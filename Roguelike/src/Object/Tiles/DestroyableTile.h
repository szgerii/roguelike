#pragma once
#include "Object/Tiles/Tile.h"

namespace CR::Objects {
	class DestroyableTile : public Tile {
	public:
		// The number of hits this tile can take before breaking
		int health;
		int initialHealth;

		DestroyableTile(char skin, unsigned short skinColor, const Vector2<int>& pos, int initialHealth)
			: Tile(skin, skinColor | FOREGROUND_INTENSITY, pos), health(initialHealth), initialHealth(initialHealth) { }

		virtual void hurt(float amount, GameObject* from);
	};
}