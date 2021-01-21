#pragma once
#include "Utils.h"
#include "Events/KeyEvent.h"

namespace CR {
	class GameObject {
	public:
		// who needs instanceof
		enum class Type : char {
			NORMAL,
			TILE,
			ENEMY,
			PLAYER,
			BULLET,
			PARTICLE
		};

		bool customObject;
	
	protected:
		char skin;
		unsigned short skinColor;
		Vector2<float> position;
		Type type;
		bool freeze = false;

	public:
		GameObject(char skin, unsigned short skinColor, const Vector2<float>& position, bool customObject = false)
			: skin(skin), skinColor(skinColor), position(position), customObject(customObject), type(Type::NORMAL) {}

		virtual char getSkin() const;
		virtual unsigned short getSkinColor() const;
		Vector2<float> getPos() const;
		float getX() const;
		float getY() const;
		Type getType() const;
		bool getFreeze() const;
		virtual void setFreeze(bool freeze);
		virtual void hurt(float amount, GameObject* from) {}
		virtual void die();
		virtual void updateCollision();
		virtual void tick() {}
	};
}