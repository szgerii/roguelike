#include "GameObject.h"

namespace CR {
	char GameObject::getSkin() const {
		return skin;
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