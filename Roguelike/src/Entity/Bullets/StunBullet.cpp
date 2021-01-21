#include "StunBullet.h"

namespace CR::Entities {
	void StunBullet::handleHit(GameObject* obj) {
		if (obj->getType() == Type::ENEMY || obj->getType() == Type::PLAYER)
			((Entities::Entity*)obj)->stun(stunDuration);

		Bullet::handleHit(obj);
	}
}