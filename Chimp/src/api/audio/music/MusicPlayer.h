#pragma once

#include "stdafx.h"
#include "api/audio/music/MusicTracksContainer.h"

namespace Chimp {
	class Engine;
	class ISound;
	class MainLoop;
	class IPlayingAudio;

	class MusicPlayer {
		friend class MainLoop;
		struct Music {
			DISABLE_COPY_AND_MOVE(Music);
			Music() : Sound(nullptr), PlayingAudio(nullptr) {}

			~Music() {
				Reset();
			}

			void Reset() {
				if (PlayingAudio) {
					PlayingAudio = nullptr;
				}
				if (Sound) {
					Sound = nullptr;
				}
			}

			std::unique_ptr<ISound> Sound;
			std::shared_ptr<IPlayingAudio> PlayingAudio;
		};
	public:
		MusicPlayer(Engine& engine);

		// Returns true if there is any music tracks
		[[nodiscard]] bool HasTracks() const;
		
		void SwitchMusic(std::shared_ptr<MusicTracksContainer> tracksContainer);
		void SwitchMusic(const MusicTracksContainer& tracksContainer);

		// Get amount of milliseconds it will take to fade from min to max volume
		[[nodiscard]] float GetMillisToFade() const;

		// Set min and max volume
		[[nodiscard]] void SetVolumeRange(float minVolume, float maxVolume);

		// Call this once per frame if you aren't using the music player in the engine instance
		void Update();

	public:
		float FadeSpeed = 0.25f; // volume change per second

	private:
		bool IsCurrentTrackStopped() const;

		void PlayNewTrack();

		void EnsureTracksLoaded();

		float GetStartingVolume() const;

	private:
		Engine& m_Engine;
		std::shared_ptr<MusicTracksContainer> m_TracksContainer;
		Music m_CurrentTrack;
		Music m_NextTrack;
		float MinVolume = 0;
		float MaxVolume = 1;
		std::chrono::system_clock::time_point m_MusicFadeOutStartTime; // Time stamp which fading out has or should start
		Vector3f m_Position = { 0, 0, 0 };
		Vector3f m_Velocity = { 0, 0, 0 };
	};
}