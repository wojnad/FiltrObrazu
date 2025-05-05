#include <iostream>
#include "FileHandlerPAM.h"
#include "FileHandlerFactory.h"
#include "ConsoleInterface.h"
#include <windows.h>

void printUsage(const std::string& programName) {
    std::cout << "Użycie: " << programName << " <ścieżka_wejściowa> [<ścieżka_wyjściowa>] [<typ_pliku>]\n";
    std::cout << "Dostępne typy plików:\n";
    std::cout << "  - tga-nieskompresowany (domyślny)\n";
    std::cout << "  - ppm\n";
    std::cout << "  - pbm\n";
    std::cout << "  - pgm\n";
}

int main(int argc, char** argv){
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    #else
    std::locale::global(std::locale(""));
    std::cout.imbue(std::locale());
    std::cin.imbue(std::locale());
    #endif

    try {

        if (argc < 2) {
            printUsage(argv[0]);
            return 1;
        }

        // Parsowanie argumentów
        const std::string inputPath = argv[1];
        const std::string outputPath = (argc > 2) ? argv[2] : inputPath;

        fileTypes::types fileType = fileTypes::TGA;
        if (argc > 3) {
            std::string typeStr = argv[3];
            if (typeStr == "ppm") {
                fileType = fileTypes::PPM;
            } else if (typeStr == "pgm") {
                fileType = fileTypes::PGM;
            } else if (typeStr == "pbm") {
                fileType = fileTypes::PBM;
            } else if (typeStr != "tga") {
                std::cerr << "Nieznany typ pliku: " << typeStr << "\n";
                printUsage(argv[0]);
                return 1;
            }
        }

        std::unique_ptr<FileHandler> fileHandler = FileHandlerFactory::getFilehander(fileType);
        wnImage image = fileHandler->readFile(inputPath);

        ConsoleInterface consoleInterface(image);
        consoleInterface.cliMenu();

        fileHandler->saveFile(outputPath, image);

    }
    catch (std::runtime_error& e) {
        std::cerr << e.what();
        return 1;
    }
    return 0;
}