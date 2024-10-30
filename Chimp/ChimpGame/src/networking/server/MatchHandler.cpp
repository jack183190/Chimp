#include "MatchHandler.h"

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
	std::cout << "Connections: " << m_PlayersNotInMatch.size() << std::endl;
}

void MatchHandler::HandleNewConnections(const Chimp::NetworkPacket* event)
{
	auto clientConnected =
		static_cast<const Chimp::ClientConnectedPacket*>(event);
	assert(clientConnected != nullptr);

	m_PlayersNotInMatch.push(clientConnected->ClientId);
}