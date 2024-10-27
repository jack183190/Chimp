#pragma once

#include "stdafx.h"
#include "ConnectionInfo.h"
#include "EventHandler.h"
#include "events/NetworkEventType.h"
#include "api/utils/ThreadQueue.h"
#include "PacketResponseFunc.h"

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

		// Send a packet to client(s)
		// clientId - The client to send the packet to, make sure this is a valid client
		// packet - The packet to send
		// channel - The channel to send the packet on, defaults to 0, make sure this is a valid channel
		// excludedClients - Vector of client ids to exclude, if empty, the packet will be sent to all clients
		// callback - The callback to call when a response is received
		virtual void SendPacketToClient(int clientId, const NetworkPacket& packet, int channel = 0) = 0;
		virtual void SendPacketToAllClients(const NetworkPacket& packet, int channel = 0) = 0;
		virtual void SendPacketToAllClientsExcept(const NetworkPacket& packet, const std::vector<int>& excludedClients, int channel = 0) = 0;
		virtual void SendPacketToClientWithResponse(int clientId, const NetworkPacket& packet, std::function<void(const NetworkPacket*)> callback, int channel = 0) = 0;

		// Handle responding to a packet
		void SetPacketResponseHandler(NetworkPacketType type, PacketResponseFunc func) {
			m_PacketResponseHandlers[type] = func;
		}

		// Broadcast all polled events
		void Update();

	protected:
		// Push events into a queue, this is called as fast as possible in its own thread
		// it is up to the impl to handle if the server is invalid or if the function is called too early
		virtual void PollEvents() = 0;

	public:
		constexpr static int HOST_ID = -1;
		constexpr static int INVALID_ID = -2;

	protected:
		const ConnectionInfo m_ServerInfo;
		ThreadQueue<std::tuple<NetworkPacketType, std::shared_ptr<NetworkPacket>>> m_EventQueue;
		int m_ConnectionId = INVALID_ID;
		std::unordered_map<NetworkPacketType, PacketResponseFunc> m_PacketResponseHandlers;

	private:
		std::thread m_EventPollingThread;
		bool m_IsBeingDestroyed = false;

	protected:
		EventHandlerAndBroadcaster<NetworkPacketType, NetworkPacket> m_EventHandlerAndBroadcaster;
	};
}