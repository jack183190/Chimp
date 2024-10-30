#pragma once

#include "GameClient.h"
#include "GameServer.h"

class Networking {
private:
	Networking() = delete;
public:
	static void Init(Chimp::Engine& engine) {
		assert(!client && !server);
		client = std::make_shared<GameClient>(engine);
		server = std::make_shared<GameServer>(engine);
	}

	static std::shared_ptr<GameClient> GetClient() {
		return client;
	}

	static std::shared_ptr<GameServer> GetServer() {
		return server;
	}

private:
	inline static std::shared_ptr<GameClient> client = nullptr;
	inline	static std::shared_ptr<GameServer> server = nullptr;
};