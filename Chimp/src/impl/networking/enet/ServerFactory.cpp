#include "ServerFactory.h"
#include "Server.h"

namespace Chimp {
	std::unique_ptr<IServerConnection> ENetServerFactory::CreateServer(const ConnectionInfo& serverInfo) {
		return std::make_unique<Server>(serverInfo);
	}
}