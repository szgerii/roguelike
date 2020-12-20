#include "GameObject.h"

namespace CR {
	char GameObject::getSkin() const {
		return skin;
	}
	
	unsigned short GameObject::getSkinColor() const {
		return skinColor;
	}

	void GameObject::tick() {

	}
	
	int GameObject::getX() const {
		return x;
	}
	
	int GameObject::getY() const {
		return y;
	}
	
	void GameObject::setX(int value) {
		x = value;
	}

	void GameObject::setY(int value) {
		y = value;
	}
}