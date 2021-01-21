#pragma once
#include <cmath>
#include "Menu.h"
#include "Engine/Game.h"
#include "Entity/Player.h"
#include "Utils.h"

namespace CR::Menus {
	template <int screenWidth, int screenHeight>
	class UltimateUpgradeMenu : public Menu<screenWidth, screenHeight> {
	protected:
		char price;

	public:
		UltimateUpgradeMenu(HANDLE screenBuffer, HANDLE inputBuffer, char target, char price)
			: Menu<screenWidth, screenHeight>(screenBuffer, inputBuffer), price(price) {
			Entities::Player* player = (Entities::Player*)Engine::getPlayer();

			std::string priceText;

			switch (price) {
			case 0:
				priceText = "-" + std::to_string((int)round(player->getMaxHealthProp().getStep())) + " Max Health";
				break;

			case 1:
				priceText = "-" + std::to_string((int)round(player->getMeleeDamageProp().getStep())) + " Melee Damage";
				break;

			case 2:
				priceText = "-" + std::to_string(player->getWeaponDamageMultiplierProp().getStep()).substr(0, 4) + " Weapon Damage Multiplier";
				break;

			case 3:
				priceText = "+" + std::to_string(player->getDamageReductionProp().getStep()).substr(0, 4) + " Damage Reduction";
				break;

			case 4:
				priceText = "-" + std::to_string(player->getAmmoPickupMultiplierProp().getStep()).substr(0, 4) + " Ammo Pickup Multiplier";
				break;

			default:
				priceText = "ERROR";
				break;
			}
			
			this->title = "Upgrade for " + priceText + "?";

			this->options.push_back("Yes");
			this->options.push_back("No");
		}

		enum Results : char {
			YES, NO
		};

		virtual int selectOption(std::string option) {
			switch (this->selectedIndex) {
			case 0:
				return Results::YES;

			case 1:
				return Results::NO;

			default:
				break;
			}

			return -1;
		}
	};
}