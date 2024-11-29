#pragma once

#include "Chimp.h"

class GameRenderer : public Chimp::GameShader
{
public:
	GameRenderer(Chimp::Engine& engine) :
		GameShader(engine, {
		GAME_SRC + std::string("/assets/shaders/default.vert"),
		GAME_SRC + std::string("/assets/shaders/default.frag")
			}) {

	}
};