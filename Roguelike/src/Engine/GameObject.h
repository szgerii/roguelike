#pragma once
#include "Utils.h"
#include "Events/KeyEvent.h"

/*
#define FOREGROUND_BLUE 0x0001
#define FOREGROUND_GREEN 0x0002
#define FOREGROUND_RED 0x0004
#define FOREGROUND_INTENSITY 0x0008
#define BACKGROUND_BLUE 0x0010
#define BACKGROUND_GREEN 0x0020
#define BACKGROUND_RED 0x0040
#define BACKGROUND_INTENSITY 0x0080
#define COMMON_LVB_REVERSE_VIDEO 0x4000
#define COMMON_LVB_UNDERSCORE 0x8000
*/

namespace CR {
	class GameObject {
	public:
		bool customObject;
	
	protected:
		char skin;
		unsigned short skinColor;
		Vector2<float> position;

	public:
		GameObject(char skin, unsigned short skinColor, Vector2<float> position, bool customObject = false)
			: skin(skin), skinColor(skinColor), position(position), customObject(customObject) {}

		GameObject(char skin, unsigned short skinColor, float x, float y, bool customObject = false)
			: skin(skin), skinColor(skinColor), position({ x, y }), customObject(customObject) {}

		virtual char getSkin() const;
		virtual unsigned short getSkinColor() const;
		Vector2<float> getPos() const;
		float getX() const;
		float getY() const;
		virtual void hurt(float amount, GameObject* from) {}
		virtual void die();
		virtual void updateCollision();
		virtual void tick() {}
		virtual void onKeyEvent(KeyEvent& e) {}
	};
}