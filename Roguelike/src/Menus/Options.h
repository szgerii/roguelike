#pragma once
#include <string>
#include "Engine/Game.h"
#include "Menus/Menu.h"

namespace CR::Menus {
	template <int screenWidth, int screenHeight>
	class OptionsMenu : public Menu<screenWidth, screenHeight> {
	protected:
		MapGenerationSetting mgs;
		Difficulty diff;

	public:
		OptionsMenu(HANDLE screenBuffer, HANDLE inputBuffer)
			: Menu<screenWidth, screenHeight>(screenBuffer, inputBuffer) {
			this->title = "O P T I O N S";

			this->options.push_back("Difficulty: LOADING...");
			this->options.push_back("Map Generation: LOADING...");
			this->options.push_back("Apply");
			this->options.push_back("Cancel");
			
			updateMgs(Engine::getMapGenerationSetting());
			updateDiff(Engine::getDifficulty());

			this->calculateLongestOptionLength();
		}

		enum Results : char {
			APPLY, CANCEL
		};

		virtual int keyEventHandle(KeyEvent& ke) {
			if (ke.keyDown && getCurrentTime() - this->lastStep > 100) {
				int kv = tolower(ke.keyValue);
				
				if (ke.keyCode == 13) {
					return this->selectOption(this->options[this->selectedIndex]);
				} else if (kv == 's') {
					this->nextOption();
				} else if (kv == 'w') {
					this->previousOption();
				} else if (this->selectedIndex == 0 && kv == 'd' && diff != Difficulty::VERY_HARD) {
					updateDiff(static_cast<Difficulty>((char)diff + 1));
				} else if (this->selectedIndex == 0 && kv == 'a' && diff != Difficulty::EASY) {
					updateDiff(static_cast<Difficulty>((char)diff - 1));
				} else if (this->selectedIndex == 1 && kv == 'd' && mgs != MapGenerationSetting::FULL) {
					updateMgs(static_cast<MapGenerationSetting>((char)mgs + 1));
				} else if (this->selectedIndex == 1 && kv == 'a' && mgs != MapGenerationSetting::EMPTY) {
					updateMgs(static_cast<MapGenerationSetting>((char)mgs - 1));
				}

				this->lastStep = getCurrentTime();
			}

			return -1;
		}

		virtual int selectOption(std::string option) {
			if (option == "Apply") {
				Engine::setMapGenerationSetting(mgs);
				Engine::setDifficulty(diff);
				return Results::APPLY;
			} else if (option == "Cancel") {
				return Results::CANCEL;
			}

			return -1;
		}

		virtual void updateMgs(MapGenerationSetting newMgs) {
			mgs = newMgs;
			std::string mgsStr;

			switch (mgs) {
			case MapGenerationSetting::EMPTY:
				mgsStr = "EMPTY";
				break;

			case MapGenerationSetting::OPEN:
				mgsStr = "OPEN";
				break;

			case MapGenerationSetting::NORMAL:
				mgsStr = "NORMAL";
				break;

			case MapGenerationSetting::TIGHT:
				mgsStr = "TIGHT";
				break;

			case MapGenerationSetting::FULL:
				mgsStr = "FULL";
				break;

			default:
				mgsStr = "ERROR";
				break;
			}

			if (mgs == MapGenerationSetting::EMPTY)
				this->options[1] = "Map Generation:     " + mgsStr + "  >";
			else if (mgs == MapGenerationSetting::FULL)
				this->options[1] = "Map Generation:  <  " + mgsStr;
			else
				this->options[1] = "Map Generation:  <  " + mgsStr + "  >";
		}

		virtual void updateDiff(Difficulty newDiff) {
			diff = newDiff;
			std::string diffStr;

			switch (diff) {
			case Difficulty::EASY:
				diffStr = "EASY";
				break;

			case Difficulty::NORMAL:
				diffStr = "NORMAL";
				break;

			case Difficulty::HARD:
				diffStr = "HARD";
				break;

			case Difficulty::VERY_HARD:
				diffStr = "ROGUE";
				break;

			default:
				diffStr = "ERROR";
				break;
			}

			if (diff == Difficulty::EASY)
				this->options[0] = "Difficulty:     " + diffStr + "  >";
			else if (diff == Difficulty::VERY_HARD)
				this->options[0] = "Difficulty:  <  " + diffStr;
			else
				this->options[0] = "Difficulty:  <  " + diffStr + "  >";
		}
	};
}
