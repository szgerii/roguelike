#include <cmath>
#include "Engine/GameObject.h"
#include "Engine/Game.h"

namespace CR {
	char GameObject::getSkin() const {
		return skin;
	}
	
	unsigned short GameObject::getSkinColor() const {
		return skinColor;
	}
	
	float GameObject::getX() const {
		return position.x;
	}
	
	float GameObject::getY() const {
		return position.y;
	}

	Vector2<float> GameObject::getPos() const {
		return position;
	}

	void GameObject::die() {
		Engine::removeGameObject(this, true);
	}

	void GameObject::updateCollision() {
		if (!customObject)
			Engine::addCollision((int) round(position.x), (int) round(position.y), this);
	}
}