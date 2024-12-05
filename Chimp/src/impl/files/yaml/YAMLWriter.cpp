#include "api/files/yaml/YAMLWriter.h"
#include "api/files/yaml/YAMLBlockParser.h"

namespace Chimp {
	void YAMLWriter::Write(const YAMLBlock& root, std::vector<std::string>& out)
	{
		Write(root, [&out](std::string_view line) {
			out.push_back(std::string(line));
			});
	}

	void YAMLWriter::Write(const YAMLBlock& root, std::function<void(std::string_view)> writeFunc, int numIndents) {
		const std::string indents = std::string(numIndents, YAMLBlockParser::INDENT_CHAR);
		for (const auto& [key, value] : root.Bools) {
			writeFunc(std::format("{}{}: {}", indents, key, value));
		}
		for (const auto& [key, value] : root.Ints) {
			writeFunc(std::format("{}{}: {}", indents, key, value));
		}
		for (const auto& [key, value] : root.Floats) {
			writeFunc(std::format("{}{}: {}", indents, key, value));
		}
		for (const auto& [key, value] : root.Strings) {
			writeFunc(std::format("{}{}: {}", indents, key, value));
		}
		for (const auto& [key, value] : root.StringArrays) {
			writeFunc(std::format("{}{}:", indents, key));
			for (const auto& v : value) {
				writeFunc(std::format("{} - {}", indents, v));
			}
		}
		for (const auto& [key, value] : root.IntArrays) {
			writeFunc(std::format("{}{}:", indents, key));
			for (const auto& v : value) {
				writeFunc(std::format("{} - {}", indents, v));
			}
		}
		for (const auto& [key, value] : root.FloatArrays) {
			writeFunc(std::format("{}{}:", indents, key));
			for (const auto& v : value) {
				writeFunc(std::format("{} - {}", indents, v));
			}
		}
		for (const auto& [key, value] : root.BoolArrays) {
			writeFunc(std::format("{}{}:", indents, key));
			for (const auto& v : value) {
				writeFunc(std::format("{} - {}", indents, v));
			}
		}
		for (const auto& [key, value] : root.Blocks) {
			writeFunc(std::format("{}{}:", indents, key));
			Write(value, writeFunc, numIndents + 1);
		}
	}
}