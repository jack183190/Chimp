#pragma once

namespace Chimp {
	enum class NetworkEventType {
		CONNECTED, DISCONNECTED, MESSAGE, CLIENT_SET_ID
	};

	struct NetworkEvent {
	};

	struct ConnectionEvent : public NetworkEvent {

	};

	struct IdPacket {
		NetworkEventType Type = NetworkEventType::CLIENT_SET_ID;
		int Id;
	};
}