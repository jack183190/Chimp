#include "ElementArrayLayout.h"
#include "impl/graphics/opengl/Translations.h"

namespace Chimp::GL {
	ElementArrayLayout::ElementArrayLayout(const std::vector<ElementComponentLayout>& layouts) :
		IElementArrayLayout(layouts)
	{
	}

	void ElementArrayLayout::Bind() const
	{
		unsigned int offset = 0;
		for (unsigned int index = 0; index < Layouts.size(); index++) {
			const auto& layout = Layouts[index];
			glEnableVertexAttribArray(index); // todo: what happens if we bind one with 3 layouts and another with 2? won't the 3rd layout still be enabled?
			glVertexAttribPointer(index,
				layout.Count,
				TypeTranslation(layout.Type),
				layout.Normalized,
				Stride,
				(const void*)offset);
			offset += layout.Count * layout.Type.Size;
		}
	}
}