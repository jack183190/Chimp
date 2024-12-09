#include "api/audio/music/MusicTracksContainer.h"
#include "api/utils/Random.h"

namespace Chimp {
	MusicTracksContainer::MusicTracksContainer(const std::vector<std::string>& trackList)
		: m_TrackList(trackList)
	{
	}

	const std::string& MusicTracksContainer::GetRandomTrack(Chimp::Random& random) 
	{
		assert(!IsEmpty());
		int pickedIndex = 0;
		
		if (m_TrackList.size() > 1) {
			// Don't pick the same track twice in a row
			pickedIndex = random.GetRandomInt(0, m_TrackList.size() - 1);
			if (pickedIndex == m_LastPickedIndex) {
				pickedIndex = (pickedIndex + 1) % m_TrackList.size();
			}
			m_LastPickedIndex = pickedIndex;
		}

		return m_TrackList[pickedIndex];
	}

	const std::vector<std::string>& MusicTracksContainer::GetTrackList() const
	{
		return m_TrackList;
	}

	size_t MusicTracksContainer::GetTrackCount() const
	{
		return m_TrackList.size();
	}

	bool MusicTracksContainer::IsEmpty() const
	{
		return m_TrackList.empty();
	}
}