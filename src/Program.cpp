#include "ClumpExtractor.h"
#include "Utils.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Utils::EnableANSI();
    Utils::PrintBanner();

    if (argc < 2) {
        std::cout << Utils::PASTEL_YELLOW << "Usage: ScriptSource-Dumper.exe <scriptsrc.clump> [output_directory]" << Utils::RESET << std::endl;
        return 1;
    }

    std::string clumpPath = argv[1];
    std::string outputDir = (argc > 2) ? argv[2] : "extracted";

    ClumpExtractor extractor;
    extractor.Process(clumpPath, outputDir);

    return 0;
}
