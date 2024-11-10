#include "api/utils/FilePath.h"

namespace Chimp {
	std::string GetPathRelativeToFile(const std::string& path, const std::string& pathToFile)
	{
		if (std::filesystem::path(path).is_absolute())
		{
			return path;
		}

		std::filesystem::path pathToFilePath(pathToFile);
		// remove file name from path
		pathToFilePath = pathToFilePath.remove_filename();
		// append path to file
		pathToFilePath /= path;
		return pathToFilePath.string();
	}
}