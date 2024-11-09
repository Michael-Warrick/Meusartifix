#include "File.hpp"

std::vector<char> IO::File::Read(const std::string &fileName) {
    std::ifstream file(fileName, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());

    if (fileSize > static_cast<size_t>(std::numeric_limits<std::streamsize>::max())) {
        throw std::runtime_error("File size exceeds maximum supported size!");
    }

    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), static_cast<std::streamsize>(fileSize));
    file.close();

    return buffer;
}
