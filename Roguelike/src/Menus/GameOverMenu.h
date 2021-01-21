#pragma once
#include "Menus/Menu.h"

namespace CR::Menus {
	template <int screenWidth, int screenHeight>
	class GameOverMenu : public Menu<screenWidth, screenHeight> {
	public:
		GameOverMenu(HANDLE screenBuffer, HANDLE inputBuffer)
			: Menu<screenWidth, screenHeight>(screenBuffer, inputBuffer) {
			this->title = "G A M E   O V E R";

			this->options.push_back("Restart");
			this->options.push_back("Quit");
			this->calculateLongestOptionLength();
		}

		enum Results : char {
			RESTART, QUIT
		};

		virtual int selectOption(std::string option) {
			if (option == "Restart")
				return Results::RESTART;
			else if (option == "Quit")
				return Results::QUIT;

			return -1;
		}
	};
}