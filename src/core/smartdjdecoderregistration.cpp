#include "smartdjdecoderregistration.h"
#include <dcmtk/dcmjpeg/djdecode.h>

void asclepios::core::SmartDJDecoderRegistration::registerCodecs()
{
	if (!m_referenceCount)
	{
		DJDecoderRegistration::registerCodecs();
	}
	++m_referenceCount;
}

//-----------------------------------------------------------------------------
void asclepios::core::SmartDJDecoderRegistration::cleanup()
{
	if (m_referenceCount > 0)
	{
		--m_referenceCount;
	}
	if (!m_referenceCount)
	{
		DJDecoderRegistration::cleanup();
	}
}
