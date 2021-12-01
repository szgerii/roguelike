#include <time.h>
#include "Engine/Game.h"

using namespace CR;

int main() {
	Engine::setRandomSeed((int)time(NULL));

	Engine::setMapGenerationSetting(MapGenerationSetting::NORMAL);
	Engine::setDifficulty(Difficulty::NORMAL);

	Engine::start();
}