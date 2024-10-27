#pragma once

namespace Chimp {
	typedef int NetworkPacketType;
	namespace Packets {
		// Register these in packet type registry
		static constexpr NetworkPacketType INVALID = 0;
		static constexpr NetworkPacketType CLIENT_SET_ID = 1;
		static constexpr NetworkPacketType FORWARD = 2;
		static constexpr NetworkPacketType TEST = 3;
		static constexpr NetworkPacketType CLIENT_CONNECTED = 4;
		static constexpr NetworkPacketType CLIENT_DISCONNECTED = 5;
		static constexpr NetworkPacketType CLIENT_REQUEST_RESPONSE = 6;
		static constexpr NetworkPacketType SERVER_RESPONDING_TO_CLIENT = 7;
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

	struct ClientConnectedPacket : public NetworkPacket {
		int ClientId;
	};

	struct ClientDisconnectedPacket : public NetworkPacket {
		int ClientId;
	};

	struct ToServerRequestResponsePacket : public NetworkPacket {
		int RequestId;
	};

	struct ToClientServerRespondingPacket : public NetworkPacket {
		int RequestId;
	};
}