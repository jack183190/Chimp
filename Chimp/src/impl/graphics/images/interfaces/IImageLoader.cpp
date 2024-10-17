#pragma once

#include "api/graphics/images/IImageLoader.h"

namespace Chimp {

	Chimp::IImageLoader::LoadedImage::~LoadedImage()
	{
		assert(Data == nullptr); // This should have been deallocated by the derived class
	}

	bool Chimp::IImageLoader::LoadedImage::IsValid() const
	{
		return Data != nullptr;
	}
}