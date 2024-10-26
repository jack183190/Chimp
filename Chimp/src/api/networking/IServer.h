#pragma once

#include "stdafx.h"
#include "ConnectionInfo.h"
#include "EventHandler.h"
#include "events/NetworkEventType.h"
#include "api/utils/ThreadQueue.h"

namespace Chimp {
	class IServer {
	protected:
		// Represents either a hosted server
		IServer(const ConnectionInfo& serverInfo);
	public:
		~IServer();
	public:
		// If this is false, failed to host the server
		[[nodiscard]] virtual bool IsValid() const = 0;

		// Returns the event handler for this server
		[[nodiscard]] EventHandler<NetworkPacketType, NetworkPacket>& GetEventHandler() {
			return *m_EventHandlerAndBroadcaster.Handler;
		}

		// Send a packet to a client
		// clientId - The client to send the packet to, make sure this is a valid client
		// packet - The packet to send
		// channel - The channel to send the packet on, defaults to 0, make sure this is a valid channel
		virtual void SendPacketToClient(unsigned int clientId, const NetworkPacket& packet, int channel = 0) = 0;
		virtual void SendPacketToAllClients(const NetworkPacket& packet, int channel = 0) = 0;

		// Broadcast all polled events
		void Update();

	protected:
		// Push events into a queue, this is called as fast as possible in its own thread
		// it is up to the impl to handle if the server is invalid or if the function is called too early
		virtual void PollEvents() = 0;

	public:
		constexpr static int HOST_ID = -1;
		constexpr static unsigned int INVALID_ID = -2;

	protected:
		const ConnectionInfo m_ServerInfo;
		ThreadQueue<std::tuple<NetworkPacketType, std::shared_ptr<NetworkPacket>>> m_EventQueue;
		unsigned int m_ConnectionId = INVALID_ID;

	private:
		std::thread m_EventPollingThread;
		bool m_IsBeingDestroyed = false;

	protected:
		EventHandlerAndBroadcaster<NetworkPacketType, NetworkPacket> m_EventHandlerAndBroadcaster;
	};
}