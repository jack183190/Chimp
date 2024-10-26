#pragma once

namespace Chimp {
	typedef int NetworkPacketType;
	namespace Packets {
		static constexpr NetworkPacketType INVALID = 0;
		static constexpr NetworkPacketType CLIENT_SET_ID = 1;
		static constexpr NetworkPacketType TEST = 2;
	}

	struct NetworkPacket {
		NetworkPacketType PacketType = Packets::INVALID;
	};

	struct ToClientSetClientIdPacket : public NetworkPacket {
		int NewClientId;
	};

	struct TestPacket : public NetworkPacket {
		int TestInt;
	};
}