#pragma once

namespace Chimp {
	enum class NetworkEventType {
		CONNECTED, DISCONNECTED, MESSAGE
	};

	struct NetworkEvent {};

	struct ConnectionEvent : public NetworkEvent {

	};
}