#pragma once
#include "Engine/GameObject.h"

namespace CR::Objects {
	class Tile : public GameObject {
	public:
		Tile(char skin, unsigned short skinColor, const Vector2<int>& pos)
			: GameObject(skin, skinColor, {(float) pos.x, (float) pos.y}) {
			type = Type::TILE;
		}
	};
}