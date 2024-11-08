#pragma once

#include "stdafx.h"
#include "api/Engine.h"

namespace Chimp {
	class WaveManager;
	class WaveCommand {
		friend class WaveManager;
	protected:
		// Each wave command should only be associated with one wave
		WaveCommand(float durationSeconds);
	public:
		virtual ~WaveCommand() = default;

		virtual void Update(Chimp::Engine& engine) = 0;
		virtual void OnFinish(Chimp::Engine& engine) {}

		float GetDuration() const;

	private:
		float m_Duration;
	};

	// Delay for an amount of seconds
	class DelayWaveCommand : public WaveCommand {
	public:
		DelayWaveCommand(float durationSeconds) : WaveCommand(durationSeconds) {}

		void Update(Chimp::Engine& engine) override {}
	};

	// Run one function
	class LambdaWaveCommand : public WaveCommand {
	public:
		LambdaWaveCommand(std::function<void(Chimp::Engine&)> lambda);

		void Update(Chimp::Engine& engine) override {}

		void OnFinish(Chimp::Engine& engine) override;

	private:
		std::function<void(Chimp::Engine&)> m_Lambda;
	};

	// Repeat a function for a number of times
	class RepeatLambdaWaveCommand : public WaveCommand {
	public:
		RepeatLambdaWaveCommand(
			std::function<void(Chimp::Engine&)> lambda,
			float interval,
			unsigned int numRepeats);

		void Update(Chimp::Engine& engine) override;

		void OnFinish(Chimp::Engine& engine) override;

	private:
		std::function<void(Chimp::Engine&)> m_Lambda;
		float m_Interval;
		unsigned int m_NumRepeats;
		float m_SecondsSinceLastExecution;
		unsigned int m_NumRepeatsRemaining;
	};
}