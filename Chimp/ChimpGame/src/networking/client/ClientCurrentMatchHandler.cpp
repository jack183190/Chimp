#include "ClientCurrentMatchHandler.h"
#include "networking/packets/Packets.h"
#include "networking/Networking.h"

ClientCurrentMatchHandler::ClientCurrentMatchHandler(Chimp::Engine& engine,
	Chimp::IClient& client) :
	m_Engine(engine),
	m_Client(client)
{
	m_MatchStartListener = m_Client.GetEventHandler().Subscribe(Networking::CLIENT_MATCH_START,
		[this](const Chimp::NetworkPacket* event) { HandleMatchStart(event); });
}

ClientCurrentMatchHandler::~ClientCurrentMatchHandler()
{
	m_Client.GetEventHandler().Unsubscribe(m_MatchStartListener);
}

void ClientCurrentMatchHandler::Update()
{
}

void ClientCurrentMatchHandler::HandleMatchStart(const Chimp::NetworkPacket* event)
{
	auto matchStartPacket =
		static_cast<const ClientMatchStartPacket*>(event);
	assert(matchStartPacket != nullptr);

	std::cout << "Match started with opponent " << matchStartPacket->OpponentId << std::endl;
}
