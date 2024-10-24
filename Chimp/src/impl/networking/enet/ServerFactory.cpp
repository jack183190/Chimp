#include "ServerFactory.h"
#include "Server.h"

namespace Chimp {
	std::unique_ptr<IServer> ENetServerFactory::CreateServer(const ServerInfo& serverInfo) {
		return std::make_unique<Server>(serverInfo);
	}
}