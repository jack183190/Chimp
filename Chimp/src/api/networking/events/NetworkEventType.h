#pragma once

namespace Chimp {
	enum class NetworkEventType {
		CONNECTED, DISCONNECTED, MESSAGE, CLIENT_SET_ID
	};

	struct NetworkEvent {
	};

	struct ConnectionEvent : public NetworkEvent {

	};
}