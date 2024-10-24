#pragma once

#include "stdafx.h"
#include "ServerInfo.h"
#include "EventHandler.h"
#include "events/NetworkEventType.h"

namespace Chimp {
	class IServer {
	protected:
		// Represents either a hosted server or a connection to a server
		IServer(const ServerInfo& serverInfo) :
			m_ServerInfo(serverInfo),
			m_EventHandler(m_Broadcaster),
			m_ServerLocalId(ServerIdCounter++) {
		}
	public:
		// If this is false, failed to connect to server
		[[nodiscard]] virtual bool IsValid() const = 0;

		virtual void Update() = 0;

		// Get a locally unique id for this instance
		[[nodiscard]] unsigned int GetServerLocalId() const {
			return m_ServerLocalId;
		}

		// Returns true if we are hosting a server, false if we are connected to a server
		[[nodiscard]] bool IsHosting() const {
			return m_ServerInfo.IsHost;
		}

	protected:
		const ServerInfo m_ServerInfo;
		std::shared_ptr<EventHandler<NetworkEventType, NetworkEvent>::Broadcaster> m_Broadcaster = nullptr;
		EventHandler<NetworkEventType, NetworkEvent> m_EventHandler;
		const unsigned int m_ServerLocalId;

	private:
		static unsigned int ServerIdCounter;
	};
}