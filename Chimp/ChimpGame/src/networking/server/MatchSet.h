#pragma once

#include "ServerMatch.h"
#include "stdafx.h"

// Represents a set of matches, wraps multiple unordered maps for quick lookups
class MatchSet {
public:
	MatchSet() = default;
	virtual ~MatchSet() = default;

	void AddMatch(const ServerMatch &match);

	void RemoveMatch(const ServerMatch& match);

	Chimp::OptionalReference<ServerMatch> GetMatchByPlayerId(int playerId);

	Chimp::OptionalReference<ServerMatch> GetMatchByMatchId(int matchId);

private:
	std::unordered_map<int, ServerMatch> m_Matches; // matchId -> match
	std::unordered_map<int, int> m_PlayerOneIdMatchId; // playerId -> matchId
	std::unordered_map<int, int> m_PlayerTwoIdMatchId; // playerId -> matchId
};