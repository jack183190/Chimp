#pragma once

#include "stdafx.h"
#include "ServerInfo.h"
#include "EventHandler.h"
#include "events/NetworkEventType.h"
#include "api/utils/ThreadQueue.h"

namespace Chimp {
	class IServer {
	protected:
		// Represents either a hosted server or a connection to a server
		IServer(const ServerInfo& serverInfo);
	public:
		~IServer();
	public:
		// If this is false, failed to connect to server
		[[nodiscard]] virtual bool IsValid() const = 0;

		// Get a locally unique id for this instance
		[[nodiscard]] unsigned int GetServerLocalId() const {
			return m_ServerLocalId;
		}

		// Returns true if we are hosting a server, false if we are connected to a server
		[[nodiscard]] bool IsHosting() const {
			return m_ServerInfo.IsHost;
		}

		// Returns the event handler for this server
		[[nodiscard]] EventHandler<NetworkEventType, NetworkEvent>& GetEventHandler() {
			return *m_EventHandlerAndBroadcaster.Handler;
		}

		// Broadcast all polled events
		void Update();

	protected:
		// Push events into a queue, this is called as fast as possible in its own thread
		// it is up to the impl to handle if the server is invalid or if the function is called too early
		virtual void PollEvents() = 0;

	protected:
		const ServerInfo m_ServerInfo;
		ThreadQueue<std::tuple<NetworkEventType, NetworkEvent>> m_EventQueue;
		const unsigned int m_ServerLocalId;

	private:
		std::thread m_EventPollingThread;
		bool m_IsBeingDestroyed = false;
		static unsigned int ServerIdCounter;

	protected:
		EventHandlerAndBroadcaster<NetworkEventType, NetworkEvent> m_EventHandlerAndBroadcaster;
	};
}