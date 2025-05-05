//
// Created by wojnad on 19.05.2024.
//

#include "FileHandlerPAM.h"
#include "wnImage.h"

FileHandlerPBM::FileHandlerPBM(bool isBinary) {
    if (isBinary){
        magicSequence = "P4";
    }
    else{
        magicSequence = "P1";
    }
}

wnImage FileHandlerPBM::readFile(const std::string &srcPath) {
    int height, width;
    std::vector<int> rawData;

    std::ifstream srcFile(srcPath, std::ios_base::in | std::ios_base::binary);
    if (!srcFile.good()){
        throw std::runtime_error("Blad otwarcia pliku: " + srcPath);
    }
    srcFile >> magicSequence;

    if (magicSequence != "P1" && magicSequence != "P4"){
        throw std::runtime_error("Nie mozna odczytac pliku - bledna magiczna sekwecja");
    }

    srcFile >> std::ws;
    while (srcFile.peek() == '#'){
        srcFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        srcFile >> std::ws;
    }
    srcFile >> width >> height;
    srcFile >> std::ws;

    rawData.resize(width*height, 0);
    switch (magicSequence[1]){
        case '1':
            for (auto& pix: rawData){
                srcFile >> pix;
            }
            break;
        case '4':
            for (auto& pix: rawData){
                pix = static_cast<int>(srcFile.get());
            }
        default:
            throw std::runtime_error("Blad odczytu danych");
    }

    srcFile.close();

    wnImage newImage(height, width, 1, std::cref(rawData), fileTypes::PBM);

    newImage.showPixValues();

    return newImage;
}

void FileHandlerPBM::saveFile(const std::string &dstPath, const wnImage &image) {
    auto rawData = image.getRawData(fileTypes::PBM);

    std::ofstream dstFile(dstPath, std::ios_base::out | std::ios_base::binary);
    if (!dstFile.good()){
        throw std::runtime_error("Brak mozliwosci zapisu w " + dstPath);
    }

    dstFile << magicSequence << '\n';
    dstFile << "# Created by wnGraphics\n";
    dstFile << image.getWidth() << ' ' << image.getHeight() << '\n';

    switch (magicSequence[1]) {
        case '1':
            for (const auto& pix: rawData){
                dstFile << pix << " ";
            }
            break;
        case '4':
            for (const auto& pix: rawData){
                dstFile.put(static_cast<char>(pix));
            }
            break;
    }

    dstFile.close();
}
