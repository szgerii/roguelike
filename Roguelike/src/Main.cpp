#include <time.h>
#include "Engine/Game.h"
#include "Object/Walls/HollowRectWall.h"
#include "Entity/Player.h"
#include "Weapon/Weapon.h"

#include "Entity/Enemies/Enemy.h"
#include "Weapon/BoomBoomPistol.h"
#include "Object/Tiles/WeaponTile.h"

using namespace CR;

int main() {
	Engine::addGameObject(new Objects::HollowRectWall(' ', BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED, { 0, 0 }, { Engine::getMapWidth() - 1, Engine::getMapHeight() - 2 }));

	auto player = new Entities::Player({ 30, 30 });
	auto weapon = new Weapons::Weapon("BRRR", 1, 0.85f, 0.4f, 100, 45, 320, 35);
	auto weapon2 = new Weapons::BoomBoomPistol();

	player->pickUpItem(weapon);
	//player->pickUpItem(weapon2);
	Engine::addGameObject(player);
	Engine::setPlayer(player);

	Engine::setRandomSeed((int)time(NULL));

	Engine::generateRandomRoom();

	auto weaponTile = new Objects::WeaponTile({2, 10}, weapon2);
	Engine::addGameObject(weaponTile);

	auto enemy = new Entities::Enemy('<', FOREGROUND_GREEN, { 10, 10}, 20);
	Engine::addGameObject(enemy);

	Engine::start();
}