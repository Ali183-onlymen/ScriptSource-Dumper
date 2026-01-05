#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <map>

struct ClumpEntry {
    uint32_t hash;
    uint32_t offset;
    uint32_t size;
    uint32_t unk;
};

class ClumpExtractor {
public:
    ClumpExtractor();
    ~ClumpExtractor();

    void Process(const std::string& clumpPath, const std::string& outputDir);

private:
    bool ReadTOC(std::ifstream& file, std::vector<ClumpEntry>& entries, uint32_t& minDataOffset);
    void ParseNameTable(std::ifstream& file, std::map<uint32_t, std::string>& nameMap);
    int ExtractFiles(std::ifstream& file, const std::vector<ClumpEntry>& entries, const std::map<uint32_t, std::string>& nameMap, const std::string& outputDir);
};
