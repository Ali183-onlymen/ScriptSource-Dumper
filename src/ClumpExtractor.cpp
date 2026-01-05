#include "ClumpExtractor.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <iomanip>

namespace fs = std::filesystem;

ClumpExtractor::ClumpExtractor() {}
ClumpExtractor::~ClumpExtractor() {}

void ClumpExtractor::Process(const std::string& clumpPath, const std::string& outputDir) {
    if (!fs::exists(clumpPath)) {
        std::cout << Utils::PASTEL_RED << "[!] Error: File " << clumpPath << " not found." << Utils::RESET << std::endl;
        return;
    }

    std::cout << Utils::PASTEL_CYAN << "[*] Processing " << Utils::BOLD << clumpPath << Utils::RESET << Utils::PASTEL_CYAN << "..." << Utils::RESET << std::endl;

    std::ifstream file(clumpPath, std::ios::binary);
    if (!file.is_open()) {
        std::cout << Utils::PASTEL_RED << "[!] Error: Could not open file." << Utils::RESET << std::endl;
        return;
    }

    // Read first chunk to check file size
    char buffer[16384];
    file.read(buffer, 16384);
    if (file.gcount() < 16) {
        std::cout << Utils::PASTEL_RED << "[!] Error: File too small." << Utils::RESET << std::endl;
        return;
    }

    // Reset file pointer
    file.seekg(0, std::ios::beg);

    std::vector<ClumpEntry> entries;
    uint32_t minDataOffset = 0xFFFFFFFF;

    if (!ReadTOC(file, entries, minDataOffset)) {
        return;
    }

    std::cout << Utils::PASTEL_GREEN << "[+] Found " << entries.size() << " files in TOC." << Utils::RESET << std::endl;

    std::cout << Utils::PASTEL_CYAN << "[*] Scanning for filenames..." << Utils::RESET << std::endl;
    std::map<uint32_t, std::string> nameMap;
    ParseNameTable(file, nameMap);

    if (!nameMap.empty()) {
        std::cout << Utils::PASTEL_GREEN << "[+] Recovered " << nameMap.size() << " filenames." << Utils::RESET << std::endl;
    }

    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }

    std::cout << Utils::PASTEL_CYAN << "[*] Extracting files to '" << outputDir << "'..." << Utils::RESET << std::endl;

    auto startTime = std::chrono::high_resolution_clock::now();
    int successCount = ExtractFiles(file, entries, nameMap, outputDir);
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    std::cout << Utils::PASTEL_GREEN << Utils::BOLD << "[SUCCESS] Extracted " << successCount << " files in " << std::fixed << std::setprecision(2) << elapsed.count() << "s!" << Utils::RESET << std::endl;
    std::cout << Utils::PASTEL_PINK << "Thanks for using Lil Poop's scriptsrc.clump extractor! <3" << Utils::RESET << std::endl;
}

bool ClumpExtractor::ReadTOC(std::ifstream& file, std::vector<ClumpEntry>& entries, uint32_t& minDataOffset) {
    char buffer[16384];
    int chunkIndex = 0;

    while (true) {
        uint32_t chunkOffset = chunkIndex * 16384;
        if (chunkOffset >= minDataOffset) break;

        file.seekg(chunkOffset, std::ios::beg);
        file.read(buffer, 16384);
        std::streamsize bytesRead = file.gcount();
        if (bytesRead == 0) break;

        for (int i = 0; i < bytesRead; i += 16) {
            if (i + 16 > bytesRead) break;

            uint32_t* entryPtr = reinterpret_cast<uint32_t*>(buffer + i);
            uint32_t hash = entryPtr[0];
            uint32_t offsetBlocks = entryPtr[1];
            uint32_t size = entryPtr[2];
            uint32_t unk = entryPtr[3];

            if (hash == 0) break;

            uint32_t offsetBytes = offsetBlocks * 4096;
            if (offsetBytes > 0 && offsetBytes < minDataOffset) {
                minDataOffset = offsetBytes;
            }

            entries.push_back({ hash, offsetBytes, size, unk });
        }
        chunkIndex++;
    }
    return true;
}

void ClumpExtractor::ParseNameTable(std::ifstream& file, std::map<uint32_t, std::string>& nameMap) {
    try {
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        std::streampos readSize = std::min(static_cast<std::streampos>(2 * 1024 * 1024), fileSize);
        
        file.seekg(fileSize - readSize, std::ios::beg);
        
        std::vector<char> buffer(readSize);
        file.read(buffer.data(), readSize);
        
        std::string tailStr(buffer.begin(), buffer.end());
        
        size_t start = 0;
        size_t end = tailStr.find('\n');
        
        while (end != std::string::npos) {
            std::string line = tailStr.substr(start, end - start);
            // Trim CR if present
            if (!line.empty() && line.back() == '\r') line.pop_back();
            
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string hashStr = line.substr(0, colonPos);
                std::string name = line.substr(colonPos + 1);
                
                // Trim whitespace
                hashStr.erase(0, hashStr.find_first_not_of(" \t"));
                hashStr.erase(hashStr.find_last_not_of(" \t") + 1);
                name.erase(0, name.find_first_not_of(" \t"));
                name.erase(name.find_last_not_of(" \t") + 1);

                if (hashStr.length() == 8) {
                    try {
                        uint32_t hash = std::stoul(hashStr, nullptr, 16);
                        nameMap[hash] = name;
                    } catch (...) {}
                }
            }
            
            start = end + 1;
            end = tailStr.find('\n', start);
        }
    } catch (...) {
        // Ignore errors
    }
}

int ClumpExtractor::ExtractFiles(std::ifstream& file, const std::vector<ClumpEntry>& entries, const std::map<uint32_t, std::string>& nameMap, const std::string& outputDir) {
    int successCount = 0;
    for (const auto& entry : entries) {
        file.seekg(entry.offset, std::ios::beg);
        
        std::vector<char> data(entry.size);
        file.read(data.data(), entry.size);
        
        // Strip trailing null byte
        if (!data.empty() && data.back() == 0) {
            data.pop_back();
        }

        std::string outPath;
        if (nameMap.count(entry.hash)) {
            std::string name = nameMap.at(entry.hash);
            // Sanitize path separators
            std::replace(name.begin(), name.end(), '/', static_cast<char>(fs::path::preferred_separator));
            std::replace(name.begin(), name.end(), '\\', static_cast<char>(fs::path::preferred_separator));
            
            fs::path p = fs::path(outputDir) / name;
            fs::create_directories(p.parent_path());
            outPath = p.string();
        } else {
            char hashBuf[16];
            sprintf_s(hashBuf, "%08x.bin", entry.hash);
            fs::path p = fs::path(outputDir) / hashBuf;
            outPath = p.string();
        }

        std::ofstream outFile(outPath, std::ios::binary);
        if (outFile.is_open()) {
            outFile.write(data.data(), data.size());
            successCount++;
        } else {
            std::cout << Utils::PASTEL_RED << "[!] Error writing " << outPath << Utils::RESET << std::endl;
        }
    }
    return successCount;
}
