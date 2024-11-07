#pragma once

#include "GameClient.h"
#include "GameServer.h"
#include "packets/Packets.h"

class Networking {
private:
	Networking() = delete;
public:
	static constexpr Chimp::NetworkPacketType CLIENT_MATCH_START = 0;
	static constexpr Chimp::NetworkPacketType CLIENT_MATCH_WIN = 1;
	static constexpr Chimp::NetworkPacketType SERVER_MATCH_END = 2;

	static void Init(Chimp::Engine& engine) {
		assert(!client && !server);

		Chimp::PacketTypeRegistry::RegisterPacketType<ClientMatchStartPacket>(CLIENT_MATCH_START);
		Chimp::PacketTypeRegistry::RegisterPacketType<ClientMatchWinPacket>(CLIENT_MATCH_WIN);
		Chimp::PacketTypeRegistry::RegisterPacketType<ClientMatchWinPacket>(SERVER_MATCH_END);

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
	inline static std::shared_ptr<GameServer> server = nullptr;
};