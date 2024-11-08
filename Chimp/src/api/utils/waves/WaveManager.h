#pragma once

#include "Wave.h"

namespace Chimp {
	class WaveManager {
	public:
		class Builder {
		public:
			Builder(Chimp::Engine& engine);
		public:
			Builder& AddWave(std::unique_ptr<Wave> wave);
			std::unique_ptr<WaveManager> Build();
		private:
			std::unique_ptr<WaveManager> m_WaveManager;
		};
		friend class Builder;
	private:
		WaveManager(Chimp::Engine& engine);
	public:
		void Update();

		// Allow the wave manager to start the next wave once the current wave has finished
		void AllowStartNextWave();

		size_t GetWave() const;
		size_t NumWaves() const;
		bool IsCurrentWaveFinished() const;
		bool AreAllWavesFinished() const;
		bool HasStartedFirstWave() const;
		// If true, don't need to call AllowStartNextWave to start the next wave
		void SetWaveAutoStart(bool autoStart);
	private:
		size_t m_CurrentWaveIndex = -1;
		std::vector<std::unique_ptr<Wave>> m_Waves;
		Chimp::Engine& m_Engine;
		bool m_CanStartNextWave = false;
		bool m_WaveAutoStart = true;
		bool m_WaveFinished = true;
	};
}