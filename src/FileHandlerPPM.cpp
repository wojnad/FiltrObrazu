//
// Created by wojnad on 19.05.2024.
//

#include "FileHandlerPAM.h"
#include "wnImage.h"

FileHandlerPPM::FileHandlerPPM(bool isBinary) {
    if (isBinary){
        magicSequence = "P6";
    }
    else{
        magicSequence = "P3";
    }
}

wnImage FileHandlerPPM::readFile(const std::string &srcPath) {
    int height, width, maxPixValue;
    std::vector<int> rawData;

    std::fstream srcFile(srcPath, std::ios_base::in | std::ios_base::binary);
    if (!srcFile.good()){
        throw std::runtime_error("Blad otwarcia pliku: " + srcPath);
    }
    srcFile >> magicSequence;
    if (magicSequence != "P6" && magicSequence != "P3"){
        throw std::runtime_error("Nie mozna odczytac pliku - bledna magiczna sekwencja");

    }

    srcFile >> std::ws;

    while (srcFile.peek() == '#'){
        srcFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        srcFile >> std::ws;
    }

    srcFile >> width >> height >> maxPixValue;
    srcFile >> std::ws;
    rawData.resize(width*height*3, 0);

    switch (magicSequence[1]) {
        case '3':
            for (auto& col: rawData){
                srcFile >> col;
            }
            break;
        case '6':
            for (auto& col: rawData){
                col = static_cast<int>(srcFile.get());
            }
            break;
        default:
            throw std::runtime_error("Blad odczytu danych");
    }

    srcFile.close();

    wnImage newImage(height, width, maxPixValue, std::cref(rawData), fileTypes::PPM);

    //newImage.showPixValues();

    return newImage;
}

void FileHandlerPPM::saveFile(const std::string &dstPath, const wnImage &image) {
    auto rawData = image.getRawData(fileTypes::PPM);

    std::ofstream dstFile(dstPath, std::ios_base::out | std::ios_base::binary);
    if (!dstFile.good()){
        throw std::runtime_error("Brak mozliwosci zapisu w " + dstPath);
    }

    dstFile << magicSequence << '\n';
    dstFile << "# Created by wnGraphics\n";
    dstFile << image.getWidth() << ' ' << image.getHeight()
    << ' ' << image.getMaxPixValue() << '\n';

    switch (magicSequence[1]){
        case '3':
            for (const auto& pix: rawData){
                dstFile << pix << " ";
            }
            break;
        case '6':
            for (const auto& pix: rawData){
                dstFile.put(static_cast<char>(pix));
            }
            break;
    }

    dstFile.close();
}