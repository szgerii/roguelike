#pragma once
#include "Engine/GameObject.h"

namespace CR {
	class CustomGameObject : public GameObject {
	public:
		CustomGameObject(char skin, unsigned short skinColor, const Vector2<float>& position)
			: GameObject(skin, skinColor, position, true) { }

		virtual void render(CHAR_INFO* screen, int screenWidth, int screenHeight) const = 0;
	};
}