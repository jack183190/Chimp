#pragma once

namespace Chimp {
	typedef int NetworkPacketType;
	namespace Packets {
		static constexpr NetworkPacketType INVALID = 0;
		static constexpr NetworkPacketType CLIENT_SET_ID = 1;
		static constexpr NetworkPacketType FORWARD = 2;
		static constexpr NetworkPacketType TEST = 3;
	}

	struct NetworkPacket {
		NetworkPacketType PacketType = Packets::INVALID;
	};

	// Forward a packet to another client
	// sending this means the next packet will be forwarded
	struct ToServerForwardPacket : public NetworkPacket {
		int ClientId;
	};

	struct ToClientSetClientIdPacket : public NetworkPacket {
		int NewClientId;
	};

	struct TestPacket : public NetworkPacket {
		int TestInt;
	};
}