#include <cmath>
#include "BoomBoomLine.h"
#include "Entity/Bullets/ExplodingLineBullet.h"

namespace CR::Weapons {
	GameObject* BoomBoomLine::getNewBullet(const Vector2<float>& startPos, Direction direction) {
		const Vector2<float>& playerPos = Engine::getPlayer()->getPos();

		return new Entities::ExplodingLineBullet('o', owner->getSkinColor(), { (int)round(startPos.x), (int)round(startPos.y) }, { (int)round(playerPos.x), (int)round(playerPos.y) }, hBulletSpeed, damage * dmgMult, owner, true);
	}
}