#pragma once

namespace Chimp {
	enum class NetworkPacketType {
		INVALID, CLIENT_SET_ID
	};

	struct NetworkPacket {
		NetworkPacketType PacketType = NetworkPacketType::INVALID;
	};

	struct ToClientSetClientIdPacket : public NetworkPacket {
		int NewClientId;
	};
}