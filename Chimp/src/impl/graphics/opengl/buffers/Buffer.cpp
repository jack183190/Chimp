#include "Buffer.h"
#include "impl/graphics/opengl/Translations.h"

namespace Chimp::GL {
	Buffer::Buffer(const Usage& usage,
		const BindTarget target) :
		m_Usage(usage),
		m_GLUsageHint(UsageTranslation(usage)),
	m_BindTarget(BindTargetTranslation(target)) {
		glGenBuffers(1, &m_Id);
	}

	Buffer::~Buffer() {
		glDeleteBuffers(1, &m_Id);
	}

	void Buffer::Bind() {
		glBindBuffer(m_BindTarget, m_Id);
	}

	void Buffer::SetBindTarget(const BindTarget target)
	{
		m_BindTarget = BindTargetTranslation(target);
	}

	void Buffer::SetDataBindless(const void* data, const size_t size)
	{
		glBufferData(m_BindTarget, size, data, m_GLUsageHint);
	}

	void Buffer::SetSubDataBindless(const void* data, const size_t size, const size_t offset)
	{
		glBufferSubData(m_BindTarget, offset, size, data);
	}

	
}