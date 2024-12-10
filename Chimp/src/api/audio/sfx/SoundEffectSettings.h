#pragma once

#include "api/audio/ISound.h"
#include "api/files/yaml/YAMLSerialisable.h"
#include "stdafx.h"
#include "Loggers.h"

namespace Chimp {
	class SoundEffectSettings : public YAMLSerialisable {
	public:
		SoundEffectSettings(const std::vector<std::string>& soundFiles, int maxSimultaneousSounds, float minPitch, float maxPitch);

		void Serialise(YAMLBlock& block, const SerialiseChildFunc& serialiseChild) const override;

		static std::unique_ptr<SoundEffectSettings> Deserialise(const YAMLBlock& block, const DeserialiseChildFunc& deserialiseChild);

		[[nodiscard]] const std::vector<std::string>& GetSoundFiles() const;

		[[nodiscard]] int GetMaxSimultaneousSounds() const;

		[[nodiscard]] float GetMinPitch() const;

		[[nodiscard]] float GetMaxPitch() const;

	private:
		std::vector<std::string> m_SoundFiles;
		int m_MaxSimultaneousSounds;
		float m_MinPitch;
		float m_MaxPitch;
	};
}