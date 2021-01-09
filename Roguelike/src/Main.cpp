#include <time.h>
#include "Engine/Game.h"
#include "Object/Walls/HollowRectWall.h"
#include "Entity/Player.h"
#include "Weapon/Weapon.h"

#include "Weapon/BoomBoomPistol.h"

using namespace CR;

int main() {
	Engine::addGameObject(new Objects::HollowRectWall(' ', BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED, 0, 0, Engine::getMapWidth() - 1, Engine::getMapHeight() - 2));

	auto player = new Entities::Player({30, 30});
	auto weapon = new Weapons::Weapon(1, 0.85f, 0.4f, 100, 45, 320);
	//auto weapon = new Weapons::BoomBoomPistol();
	
	player->changeWeapon(weapon);
	Engine::addGameObject(player);
	Engine::setPlayer(player);

	Engine::setRandomSeed((int) time(NULL));

	Engine::generateRandomRoom();

	Engine::start();
}