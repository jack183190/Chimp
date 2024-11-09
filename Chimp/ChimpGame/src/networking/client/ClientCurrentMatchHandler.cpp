#include "ClientCurrentMatchHandler.h"
#include "networking/packets/Packets.h"
#include "networking/Networking.h"
#include "Logger.h"

ClientCurrentMatchHandler::ClientCurrentMatchHandler(Chimp::Engine& engine,
	Chimp::IClient& client) :
	m_Engine(engine),
	m_Client(client)
{
	m_MatchStartListener = m_Client.GetEventHandler().Subscribe(Networking::CLIENT_MATCH_START,
		[this](const Chimp::NetworkPacket* event) { HandleMatchStart(event); });

	ClientMatchStartPacket matchStartPacket; // TODO: Remove this
	matchStartPacket.PacketType = Networking::CLIENT_MATCH_START;
	matchStartPacket.MatchId = 1;
	matchStartPacket.OpponentId = 2;
//	HandleMatchStart(&matchStartPacket); 
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
	if (IsInMatch()) {
		std::cout << "Already in a match, dropping match start packet" << std::endl;
		GetLogger().Error("Already in a match, dropping match start packet");
		return;
	}

	auto matchStartPacket =
		static_cast<const ClientMatchStartPacket*>(event);
	assert(matchStartPacket != nullptr);

	m_MatchId = matchStartPacket->MatchId;
	m_OpponentId = matchStartPacket->OpponentId;

	GetLogger().Info("Match started with opponent " + std::to_string(m_OpponentId));
}
