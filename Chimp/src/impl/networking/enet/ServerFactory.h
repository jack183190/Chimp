#pragma once

#include "api/networking/IServerConnection.h"

namespace Chimp {
	class ENetServerFactory {
	public:
		static std::unique_ptr<IServerConnection> CreateServer(const ConnectionInfo& serverInfo);
	};
}