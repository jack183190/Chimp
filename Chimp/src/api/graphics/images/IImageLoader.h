#pragma once

#include "stdafx.h"

namespace Chimp {
	class IImageLoader {
	public:
		// Represents a loaded image
		struct LoadedImage {
			friend class IImageLoader;
		protected:
			LoadedImage() = default;

		public:
			virtual ~LoadedImage(); // Deallocate the image data

			unsigned char* Data = nullptr; // The image data, owned by this struct
			int Width = 0; // The width of the image
			int Height = 0; // The height of the image
			int NumberChannels = 0; // The number of channels in the image (3 for RGB, 4 for RGBA)

			[[nodiscard]] bool IsValid() const;
		};

		// Represents options for loading an image
		struct Options {
			bool FlipVertically = false;
		};

	public:
		// Load an image from a file
		[[nodiscard]] virtual  std::unique_ptr<LoadedImage> LoadImage(
			const std::string& path,
			const Options& options = {}
		) = 0;
	};
}