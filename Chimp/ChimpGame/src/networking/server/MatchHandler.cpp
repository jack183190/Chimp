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

	m_MatchEndListener = m_Server.GetEventHandler().Subscribe(
		Networking::SERVER_MATCH_END,
		[this](const Chimp::NetworkPacket* event) { HandleMatchEnd(event); }
	);
}

MatchHandler::~MatchHandler()
{
	m_Server.GetEventHandler().Unsubscribe(m_NewConnectionListener);
	m_Server.GetEventHandler().Unsubscribe(m_MatchEndListener);
}

void MatchHandler::Update()
{
	// HANDLE STARTING MATCHES
	if (m_PlayersNotInMatch.size() >= 2)
	{
		auto player1 = m_PlayersNotInMatch.front();
		m_PlayersNotInMatch.pop();
		auto player2 = m_PlayersNotInMatch.front();
		m_PlayersNotInMatch.pop();

		StartMatch(player1, player2);
	}

	// HANDLE ONE PLAYER DISCONNECTING
	auto connectedClientIds = m_Server.GetConnectedClientIds();
	for (auto& match : m_MatchSet)
	{
		if (std::find(connectedClientIds.begin(), connectedClientIds.end(), match.GetPlayerOneId()) == connectedClientIds.end())
		{
			// Player 2 won
			ClientMatchWinPacket winPacket;
			winPacket.PacketType = Networking::CLIENT_MATCH_WIN;
			winPacket.MatchId = match.GetMatchId();
			m_Server.SendPacketToClient(match.GetPlayerTwoId(), winPacket);
			SendMatchEndPacket(match.GetMatchId());
		}
		else if (std::find(connectedClientIds.begin(), connectedClientIds.end(), match.GetPlayerTwoId()) == connectedClientIds.end())
		{
			// Player 1 won
			ClientMatchWinPacket winPacket;
			winPacket.PacketType = Networking::CLIENT_MATCH_WIN;
			winPacket.MatchId = match.GetMatchId();
			m_Server.SendPacketToClient(match.GetPlayerOneId(), winPacket);
			SendMatchEndPacket(match.GetMatchId());
		}
	}
}

void MatchHandler::HandleNewConnections(const Chimp::NetworkPacket* event)
{
	auto clientConnected =
		static_cast<const Chimp::ClientConnectedPacket*>(event);
	assert(clientConnected != nullptr);

	m_PlayersNotInMatch.push(clientConnected->ClientId);
}

void MatchHandler::HandleMatchEnd(const Chimp::NetworkPacket* event)
{
	auto matchEndPacket = static_cast<const ServerMatchEndPacket*>(event);
	assert(matchEndPacket != nullptr);

	m_MatchSet.RemoveMatchById(matchEndPacket->MatchId);
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

void MatchHandler::SendMatchEndPacket(int matchId)
{
	ServerMatchEndPacket matchEndPacket;
	matchEndPacket.PacketType = Networking::SERVER_MATCH_END;
	matchEndPacket.MatchId = matchId;
	m_Server.SendPacketToSelf(matchEndPacket);
}
