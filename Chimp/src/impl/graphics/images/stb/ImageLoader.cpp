#include "ImageLoader.h"

namespace Chimp::STB {
	ImageLoader::LoadedImage::~LoadedImage()
	{
		stbi_image_free(Data);
		Data = nullptr;
	}

	std::unique_ptr<IImageLoader::LoadedImage> ImageLoader::LoadImage(const std::string& path, const Options& options)
	{
		std::unique_ptr<LoadedImage> image = std::make_unique<LoadedImage>();

		stbi_set_flip_vertically_on_load(options.FlipVertically);

		image->Data = stbi_load(path.c_str(), &image->Width, &image->Height, &image->NumberChannels, 0);

#ifndef NDEBUG
		if (!image->IsValid())
		{
			std::cerr << "Failed to load image: " << path << std::endl;
		}
		else {
			std::cout << "--------------------------" << std::endl;
			std::cout << "Loaded image: " << path << std::endl;
			std::cout << "Width: " << image->Width << std::endl;
			std::cout << "Height: " << image->Height << std::endl;
			std::cout << "Number of channels: " << image->NumberChannels << std::endl;
			std::cout << "--------------------------" << std::endl;
		}
#endif

		return image;
	}
}