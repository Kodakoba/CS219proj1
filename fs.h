#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

namespace fs {

template<typename T>
std::vector<T> read_file(const std::filesystem::path &filename, bool binary) {
	// onelining was a mistake
	std::ifstream file(filename, binary ? std::ios::ate | std::ios::binary : std::ios::ate);

	if(!file.is_open()) {
		return {};
	}

	size_t file_size = (size_t)file.tellg();
	std::vector<T> buffer(file_size);

	file.seekg(0);
	file.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(file_size));
	file.close();

	return buffer;
}

template<typename T>
std::vector<T> read_local(const std::filesystem::path &filename, bool binary) {
	return fs::read_file<T>(std::filesystem::current_path().append(filename.string()), binary);
}

std::string read_string_file(const std::filesystem::path &filename);
std::string read_string_local(const std::filesystem::path &filename);


}