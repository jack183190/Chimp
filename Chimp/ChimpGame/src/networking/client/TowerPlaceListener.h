#pragma once

#include "stdafx.h"
#include "networking/packets/Packets.h"
#include "game/towers/TowerManager.h"

class TowerPlaceListener {
public:
	TowerPlaceListener(Chimp::IClient& client);
	~TowerPlaceListener();

	void Update(TowerManager& opponentTowerManager);

private:
	std::queue<ClientTowerPlacePacket> m_PlacePackets;
	Chimp::EventListener m_Listener;
	Chimp::IClient& m_Client;
};