#include "api/graphics/buffers/IElementArrayLayout.h"

namespace Chimp {
	IElementArrayLayout::IElementArrayLayout(const std::vector<ElementComponentLayout>& layouts) :
		Layouts(layouts), Stride(CalculateStride(layouts))
	{
	}

	unsigned int Chimp::IElementArrayLayout::CalculateStride(const std::vector<ElementComponentLayout>& layouts)
	{
		unsigned int stride = 0;
		for (const auto& layout : layouts) {
			stride += layout.Count * layout.Type.Size;
		}
		return stride;
	}
}