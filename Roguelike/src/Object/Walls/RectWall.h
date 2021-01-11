#pragma once
#include "Engine/CustomGameObject.h"

namespace CR::Objects {
	class RectWall : public CustomGameObject {
	protected:
		Vector2<int> start, end;
	
	public:
		RectWall(char skin, unsigned short skinColor, const Vector2<int>& startPos, const Vector2<int>& endPos)
			: CustomGameObject(skin, skinColor, { 0, 0 }), start(0, 0), end(0, 0) {
			start.x = startPos.x < endPos.x ? startPos.x : endPos.x;
			start.y = startPos.y < endPos.y ? startPos.y : endPos.y;
			end.x = startPos.x >= endPos.x ? startPos.x : endPos.x;
			end.y = startPos.y >= endPos.y ? startPos.y : endPos.y;
		}

		void updateCollision();
		virtual void render(CHAR_INFO* screen, int screenWidth, int screenHeight) const;
	};
}