#pragma once

#include "stdafx.h"
#include "ConnectionInfo.h"
#include "EventHandler.h"
#include "events/NetworkEventType.h"
#include "api/utils/ThreadQueue.h"
#include "PacketResponseFunc.h"

namespace Chimp {
	// Client and server do share a lot of similarities, but it is easier to have them separate and just duplicate some code.
	// because it makes sense to have the same code with different function names
	// also yes you could have a base class, but it just makes the code a lot harder to reason about
	class IClient {
	protected:
		// Represents a connection to a server
		IClient(const ConnectionInfo& serverInfo);
	public:
		~IClient();
	public:
		// If this is false, failed to connect to server
		[[nodiscard]] virtual bool IsValid() const = 0;

		// Returns the event handler for this server
		[[nodiscard]] EventHandler<NetworkPacketType, NetworkPacket>& GetEventHandler() {
			return *m_EventHandlerAndBroadcaster.Handler;
		}

		// Broadcast all polled events
		void Update();

		// Send a packet to the server
		// packet - The packet to send
		// channel - The channel to send the packet on, defaults to 0, make sure this is a valid channel
		// callback - The callback to call when a response is received
		virtual void SendPacketToServer(const NetworkPacket& packet, int channel = 0) = 0;
		virtual void SendPacketToServerWithResponse(const NetworkPacket& packet, std::function<void(const NetworkPacket*)> callback, int channel = 0) = 0;

		// Send a packet to a client
		// clientId - The client to send the packet to, make sure this is a valid client
		// packet - The packet to send
		// channel - The channel to send the packet on, defaults to 0, make sure this is a valid channel
		void SendPacketToClient(unsigned int clientId, const NetworkPacket& packet, int channel = 0);

		// Handle responding to a packet
		void SetPacketResponseHandler(NetworkPacketType type, PacketResponseFunc func) {
			m_PacketResponseHandlers[type] = func;
		}

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
		std::unordered_map<NetworkPacketType, PacketResponseFunc> m_PacketResponseHandlers;

	private:
		std::thread m_EventPollingThread;
		bool m_IsBeingDestroyed = false;

	protected:
		EventHandlerAndBroadcaster<NetworkPacketType, NetworkPacket> m_EventHandlerAndBroadcaster;
	};
}