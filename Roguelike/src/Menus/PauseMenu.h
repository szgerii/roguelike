#pragma once
#include "Menus/Menu.h"

namespace CR::Menus {
	template <int screenWidth, int screenHeight>
	class PauseMenu : public Menu<screenWidth, screenHeight> {
	private:
		long long createTime;
	
	public:
		PauseMenu(HANDLE screenBuffer, HANDLE inputBuffer)
			: Menu<screenWidth, screenHeight>(screenBuffer, inputBuffer) {
			this->title = "P A U S E D";

			this->options.push_back("Continue");
			this->options.push_back("Main Menu");
			this->options.push_back("Quit");
			this->calculateLongestOptionLength();

			createTime = getCurrentTime();
		}

		enum Results : char {
			CONTINUE, BACK_TO_MAIN_MENU, QUIT
		};

		virtual int selectOption(std::string option) {
			if (option == "Continue")
				return Results::CONTINUE;
			else if (option == "Main Menu")
				return Results::BACK_TO_MAIN_MENU;
			else if (option == "Quit")
				return Results::QUIT;

			return -1;
		}

		virtual int keyEventHandle(KeyEvent& ke) {
			if (ke.keyCode == 27 && getCurrentTime() - createTime >= 300)
				return Results::CONTINUE;
			
			return Menu<screenWidth, screenHeight>::keyEventHandle(ke);
		}
	};
}