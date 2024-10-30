#include "MatchHandler.h"
#include "networking/packets/Packets.h"
#include "networking/Networking.h"

MatchHandler::MatchHandler(Chimp::Engine& engine,
	Chimp::IServer& server) :
	m_Engine(engine),
	m_Server(server)
{
	m_NewConnectionListener = m_Server.GetEventHandler().Subscribe(
		Chimp::Packets::SERVER_CLIENT_CONNECTED,
		[this](const Chimp::NetworkPacket* event) { HandleNewConnections(event); }
	);
}

MatchHandler::~MatchHandler()
{
	m_Server.GetEventHandler().Unsubscribe(m_NewConnectionListener);
}

void MatchHandler::Update()
{
	if (m_PlayersNotInMatch.size() >= 2)
	{
		auto player1 = m_PlayersNotInMatch.front();
		m_PlayersNotInMatch.pop();
		auto player2 = m_PlayersNotInMatch.front();
		m_PlayersNotInMatch.pop();

		StartMatch(player1, player2);
	}
}

void MatchHandler::HandleNewConnections(const Chimp::NetworkPacket* event)
{
	auto clientConnected =
		static_cast<const Chimp::ClientConnectedPacket*>(event);
	assert(clientConnected != nullptr);

	m_PlayersNotInMatch.push(clientConnected->ClientId);
}

void MatchHandler::StartMatch(int player1, int player2)
{
	int matchId;
	{
		ServerMatch match(player1, player2);
		matchId = match.GetMatchId();
		m_MatchSet.AddMatch(match);
	}

	// Send match start packet to both players
	ClientMatchStartPacket matchStartPacket;
	matchStartPacket.PacketType = Networking::CLIENT_MATCH_START;
	matchStartPacket.MatchId = matchId;
	matchStartPacket.OpponentId = player2;
	m_Server.SendPacketToClient(player1, matchStartPacket);
	matchStartPacket.OpponentId = player1;
	m_Server.SendPacketToClient(player2, matchStartPacket);
}
