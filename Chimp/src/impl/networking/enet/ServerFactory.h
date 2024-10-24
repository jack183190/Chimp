#pragma once

#include "api/networking/IServer.h"

namespace Chimp {
	class ENetServerFactory {
	public:
		static std::unique_ptr<IServer> CreateServer(const ServerInfo& serverInfo);
	};
}