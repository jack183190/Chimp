#include "api/audio/music/MusicPlayer.h"
#include "api/Engine.h"
#include "api/audio/IPlayingAudio.h"
#include "api/audio/ISound.h"
#include "Loggers.h"

namespace Chimp {
	MusicPlayer::MusicPlayer(Engine& engine) :
		m_Engine(engine),
		m_NextTrackWriteMutex()
	{
	}

	bool MusicPlayer::HasTracks() const
	{
		return m_TracksContainer != nullptr && !m_TracksContainer->IsEmpty();
	}

	void MusicPlayer::SwitchMusic(std::shared_ptr<MusicTracksContainer> tracksContainer)
	{
		if (tracksContainer.get() == m_TracksContainer.get()) [[unlikely]] return;

		std::lock_guard lock(m_NextTrackWriteMutex);
		Loggers::Audio().Info(std::format("Switching music tracks container"));

		m_TracksContainer = tracksContainer;
		m_MusicFadeOutStartTime = std::chrono::system_clock::now();
		m_NextTrackLoadId++;
		m_NextTrack.Reset();

		if (HasTracks()) {
			if (IsCurrentTrackStopped()) {
				PlayNewTrack();
			}
			else {
				AsyncLoadNextTrack();
			}
		}
	}

	void MusicPlayer::SwitchMusic(const MusicTracksContainer& tracksContainer)
	{
		SwitchMusic(std::make_shared<MusicTracksContainer>(tracksContainer));
	}

	float MusicPlayer::GetMillisToFade() const
	{
		assert(FadeSpeed > 0.0f);
		return (MaxVolume - MinVolume) / FadeSpeed * 1000;
	}

	void MusicPlayer::SetVolumeRange(float minVolume, float maxVolume)
	{
		MinVolume = minVolume;
		MaxVolume = Max(maxVolume, minVolume);
	}

	bool MusicPlayer::IsCurrentTrackStopped() const
	{
		return
			!m_CurrentTrack.PlayingAudio ||
			!m_CurrentTrack.PlayingAudio->IsPlaying() ||
			!m_CurrentTrack.PlayingAudio->GetVolume() > MinVolume;
	}

	void MusicPlayer::PlayNewTrack()
	{
		std::lock_guard lock(m_NextTrackWriteMutex);

		if (m_NextTrack.Sound) {
			m_CurrentTrack.Sound = std::move(m_NextTrack.Sound);
			m_NextTrack.PlayingAudio.reset();
		}
		else {
			m_CurrentTrack.Sound = m_Engine.GetAudioManager().LoadSound(m_TracksContainer->GetRandomTrack(m_Engine.GetRandom()));
			m_CurrentTrack.PlayingAudio.reset();
		}
		AsyncLoadNextTrack();

		m_CurrentTrack.PlayingAudio = m_CurrentTrack.Sound->Play(m_Position, m_Velocity, 1.0f, GetStartingVolume());

		const auto trackDuration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::duration<float>(m_CurrentTrack.Sound->GetDurationSeconds()));
		m_MusicFadeOutStartTime = std::chrono::system_clock::now() + trackDuration - std::chrono::milliseconds((int)GetMillisToFade());
	}

	void MusicPlayer::AsyncLoadNextTrack()
	{
		if (m_NextTrack.Sound) return;

		std::lock_guard lock(m_NextTrackWriteMutex);
		const size_t loadId = ++m_NextTrackLoadId;

		m_Engine.GetThreadPool().RunTask([this, loadId]() {
			if (m_NextTrackLoadId != loadId) return; // something happened like we switched music container and this was useless

			auto timer = m_Engine.GetTimeManager().CreateTimer().Start();
			auto sound = m_Engine.GetAudioManager().LoadSound(m_TracksContainer->GetRandomTrack(m_Engine.GetRandom()));
			timer.Stop();
			Loggers::Audio().Info(std::format("Loaded next music track in {}s", timer.GetSecondsElapsed()));
			
			std::lock_guard lock(m_NextTrackWriteMutex);
			if (m_NextTrackLoadId == loadId) { // double check we still need it
				m_NextTrack.Sound = std::move(sound);
				m_NextTrackLoadId++;
			}
			});
	}

	float MusicPlayer::GetStartingVolume() const
	{
		return MinVolume + 0.001; // if it is min volume, we would pick a new track next frame
	}

	void MusicPlayer::Update()
	{
		if (!HasTracks()) return;

		// Update sounds
		if (m_CurrentTrack.Sound) m_CurrentTrack.Sound->Update();
		if (m_NextTrack.Sound) m_NextTrack.Sound->Update();

		if (IsCurrentTrackStopped()) {
			PlayNewTrack();
		}

		const float dt = m_Engine.GetTimeManager().GetDeltaTime();

		// Handle fading out
		float fadeDirection = std::chrono::system_clock::now() >= m_MusicFadeOutStartTime ? -1.0f : 1.0f; // Negative if fading out, positive if fading in
		m_CurrentTrack.PlayingAudio->AddVolumeThenClamp(fadeDirection * FadeSpeed * dt, MinVolume, MaxVolume);
	}
}