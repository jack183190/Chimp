#pragma once

#include "api/scenes/Scene.h"

int main(int argc, char** argv);

extern std::unique_ptr<Chimp::Scene> CreateEntryScene();