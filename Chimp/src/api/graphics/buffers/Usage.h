#pragma once

namespace Chimp {
	struct Usage {
		enum class UpdateFrequency {
			VERY_OCCASIONAL,
			OCCASIONAL,
			OFTEN
		};

		enum class Access {
			READ,
			WRITE,
			READ_WRITE
		};

		UpdateFrequency UpdateFrequency;
		Access Access;

		bool CanRead() const {
			return Access == Access::READ || Access == Access::READ_WRITE;
		}

		bool CanWrite() const {
			return Access == Access::WRITE || Access == Access::READ_WRITE;
		}
	};
}