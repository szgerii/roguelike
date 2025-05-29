#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <ctype.h>
#include "Events/KeyEvent.h"
#include "Utils.h"

namespace CR::Menus {
	template <int screenWidth, int screenHeight>
	class Menu {
	protected:
		HANDLE screenBuffer, inputBuffer;
		CHAR_INFO screen[screenHeight][screenWidth];
		std::string title;
		std::vector<std::string> options;
		int selectedIndex = 0, titleHighlightIndex = 0, longestOptionLength;
		long long lastSwitch = getCurrentTime(), lastStep = 0;

	public:
		Menu(HANDLE screenBuffer, HANDLE inputBuffer)
			: screenBuffer(screenBuffer), inputBuffer(inputBuffer) {}

		enum Results : char {};

		int show() {
			return menuLoop();
		}

	protected:
		void calculateLongestOptionLength() {
			int max = -1;

			for (size_t i = 0; i < options.size(); i++) {
				if ((int)options[i].length() > max)
					max = (int)options[i].length();
			}
			
			longestOptionLength = max;
		}

		virtual int menuLoop() {
			bool running = true;

			int result;
			while (running) {
				result = tick();

				if (result != -1)
					running = false;

				render();
			}

			return result;
		}

		virtual int tick() {
			DWORD numOfInputsAvailable, numOfInputsRead;

			GetNumberOfConsoleInputEvents(inputBuffer, &numOfInputsAvailable);

			INPUT_RECORD* inputs = new INPUT_RECORD[numOfInputsAvailable];

			ReadConsoleInputA(inputBuffer, inputs, numOfInputsAvailable, &numOfInputsRead);
			for (size_t i = 0; i < numOfInputsRead; i++) {
				if (inputs[i].EventType != KEY_EVENT)// || inputs[i].Event.KeyEvent.uChar.AsciiChar == '\0')
					continue;

				CR::KeyEvent ke(inputs[i].Event.KeyEvent);

				int result = keyEventHandle(ke);
				if (result != -1)
					return result;
			}

			delete[] inputs;

			if (getCurrentTime() - lastSwitch > 500) {
				do {
					titleHighlightIndex++;
					
					if (titleHighlightIndex >= (int)title.length())
						titleHighlightIndex = 0;
				} while (title[titleHighlightIndex] == ' ');

				lastSwitch = getCurrentTime();
			}

			return -1;
		}

		virtual int keyEventHandle(KeyEvent& ke) {
			if (ke.keyDown && getCurrentTime() - lastStep > 100) {
				if (ke.keyCode == 13)
					return selectOption(options[selectedIndex]);
				else if (tolower(ke.keyValue) == 's' || ke.keyCode == VK_DOWN)
					nextOption();
				else if (tolower(ke.keyValue) == 'w' || ke.keyCode == VK_UP)
					previousOption();

				lastStep = getCurrentTime();
			}

			return -1;
		}

		virtual void render() {
			for (int i = 0; i < screenHeight; i++) {
				for (int j = 0; j < screenWidth; j++) {
					screen[i][j] = CHAR_INFO();
				}
			}

			for (int i = 0; i < screenHeight; i++) {
				if (i == 0 || i == screenHeight - 1) {
					for (int j = 0; j < screenWidth; j++)
						screen[i][j] = { (wchar_t)'#', FG_WHITE };

					continue;
				}

				screen[i][0] = { (wchar_t)'#', FG_WHITE };
				screen[i][screenWidth - 1] = { (wchar_t)'#', FG_WHITE };
			}

			int verticalSpace = (options.size() + 1) * 4 - 1,
				verticalMargin = (screenHeight - verticalSpace) / 2;

			for (size_t i = 0; i <= options.size(); i++) {
				std::string text = title;

				if (i != 0) {
					text = options[i - 1];

					//int padding = ((longestOptionLength - text.length()) / 2 + 2);
					//text = std::string(padding, ' ') + text + std::string(padding, ' ');
				}

				int horizontalMargin = (screenWidth - text.length()) / 2;
				horizontalMargin = 5;
				unsigned short color = (i - 1 == selectedIndex ? FOREGROUND_RED : FG_WHITE);

				/*for (int j = -1; j <= (int)text.length(); j++) {
					if (j == -1 || j == (int)text.length()) {
						if (i != 0) {
							screen[verticalMargin + i * 4][horizontalMargin + j] = { (wchar_t)'#', color };
							screen[verticalMargin + i * 4 + 1][horizontalMargin + j] = { (wchar_t)'#', color };
							screen[verticalMargin + i * 4 + 2][horizontalMargin + j] = { (wchar_t)'#', color };
						}
						
						continue;
					}

					if (i != 0) {
						screen[verticalMargin + i * 4][horizontalMargin + j] = { (wchar_t)'#', color };
						screen[verticalMargin + i * 4 + 2][horizontalMargin + j] = { (wchar_t)'#', color };
					}
					
					screen[verticalMargin + i * 4 + 1][horizontalMargin + j] = { (wchar_t)text[j], color };
				}*/

				for (int j = 0; j < (int)text.length(); j++)
					screen[verticalMargin + i * 4 + 1][horizontalMargin + j] = { (wchar_t)text[j], color };

				if (i == 0) {
					screen[verticalMargin + 1][horizontalMargin + titleHighlightIndex].Attributes = FOREGROUND_RED;
				}
			}

			SMALL_RECT sr = SMALL_RECT{ 0, 0, screenWidth, screenHeight };
			WriteConsoleOutputA(screenBuffer, screen[0], { screenWidth, screenHeight }, { 0, 0 }, &sr);
		}

		virtual void nextOption() {
			selectedIndex++;

			if (selectedIndex >= (int)options.size())
				selectedIndex = 0;
		}

		virtual void previousOption() {
			selectedIndex--;

			if (selectedIndex < 0)
				selectedIndex = options.size() - 1;
		}

		virtual int selectOption(std::string option) = 0;
	};
}