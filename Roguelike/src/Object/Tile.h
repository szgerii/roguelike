#pragma once
#include "Engine/GameObject.h"

namespace CR::Objects {
	class Tile : public GameObject {
	public:
		Tile(char skin, unsigned short skinColor, int x, int y)
			: GameObject(skin, skinColor, x, y) {}
		
		Tile(char skin, unsigned short skinColor, const Vector2<int>& pos)
			: GameObject(skin, skinColor, pos.x, pos.y) {}
	};
}