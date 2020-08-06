#pragma once

#include <atomic>
#include "utils.h"

namespace asclepios::core
{
	class export SmartDJDecoderRegistration
	{
	public:
		SmartDJDecoderRegistration() = default;
		~SmartDJDecoderRegistration() = default;

		static void registerCodecs();
		static void cleanup();

	private:
		inline static std::atomic<int> m_referenceCount = 0;
	};
}
