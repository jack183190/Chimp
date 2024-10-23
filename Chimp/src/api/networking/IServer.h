#pragma once

#include "stdafx.h"
#include "ServerInfo.h"

namespace Chimp {
	class IServer {
	protected:
		// Represents either a hosted server or a connection to a server
		IServer(const ServerInfo& serverInfo) {}
	public:
		// If this is false, failed to connect to server
		[[nodiscard]] virtual bool IsValid() const;

	private:
	};
}