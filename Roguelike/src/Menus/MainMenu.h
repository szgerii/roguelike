#pragma once
#include "Menus/Menu.h"
#include "Menus/Options.h"

namespace CR::Menus {
	template <int screenWidth, int screenHeight>
	class MainMenu : public Menu<screenWidth, screenHeight> {
	public:
		MainMenu(HANDLE screenBuffer, HANDLE inputBuffer)
			: Menu<screenWidth, screenHeight>(screenBuffer, inputBuffer) {
			this->title = "R O G U E L I K E";
			this->options.push_back("Start");
			//this->options.push_back("Tutorial");
			this->options.push_back("Options");
			this->options.push_back("Quit");

			this->calculateLongestOptionLength();
		}

		enum Results : char {
			START, QUIT, TUTORIAL
		};

		virtual int selectOption(std::string option) {
			if (option == "Start")
				return Results::START;
			else if (option == "Tutorial")
				return Results::TUTORIAL;
			else if (option == "Options") {
				OptionsMenu optionsMenu = OptionsMenu<screenWidth, screenHeight>(this->screenBuffer, this->inputBuffer);
				optionsMenu.show();
			} else if (option == "Quit")
				return Results::QUIT;

			return -1;
		}
	};
}
