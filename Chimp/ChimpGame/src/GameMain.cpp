#include "TestScene.h"

std::unique_ptr<Chimp::Scene> CreateEntryScene() {
	return std::make_unique<TestScene>();
}