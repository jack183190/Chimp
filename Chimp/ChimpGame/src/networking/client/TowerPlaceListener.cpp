#include "TowerPlaceListener.h"
#include "networking/Networking.h"

TowerPlaceListener::TowerPlaceListener(Chimp::IClient& client)
	: m_Client(client)
{
	m_Listener = m_Client.GetEventHandler().Subscribe(
		Networking::CLIENT_TOWER_PLACE,
		[this](const Chimp::NetworkPacket* packet) {
			const auto placePacket = static_cast<const ClientTowerPlacePacket*>(packet);
			assert(placePacket);
			m_PlacePackets.push(*placePacket);
		}
	);
}

TowerPlaceListener::~TowerPlaceListener()
{
	m_Client.GetEventHandler().Unsubscribe(m_Listener);
}

void TowerPlaceListener::Update(TowerManager& opponentTowerManager)
{
	while (!m_PlacePackets.empty()) {
		const auto packet = m_PlacePackets.front();
		opponentTowerManager.PlaceTower(packet.Type, packet.Position);
		m_PlacePackets.pop();
	}
}
