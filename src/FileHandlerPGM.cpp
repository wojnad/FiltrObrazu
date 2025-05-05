//
// Created by wojnad on 10.05.2024.
//

#include "FileHandlerPAM.h"
#include "wnImage.h"

FileHandlerPGM::FileHandlerPGM(bool isBinary) {
    if (isBinary){
        magicSequence = "P5";
    }
    else{
        magicSequence = "P2";
    }
}

wnImage FileHandlerPGM::readFile(const std::string &srcPath) {
    int height, width, maxPixValue;
    std::vector<int> rawData;


    std::fstream srcFile(srcPath, std::ios_base::in | std::ios_base::binary);
    if (!srcFile.good()){
        throw std::runtime_error("Blad otwarcia pliku: " + srcPath);
    }
    srcFile >> magicSequence;

    if (magicSequence != "P5" && magicSequence != "P2"){
        throw std::runtime_error("Nie mozna odczytac pliku - bledna magiczna sekwencja");
    }
    srcFile >> std::ws;

    while (srcFile.peek() == '#') {
        srcFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        srcFile >> std::ws;
    }
    srcFile >> width >> height >> maxPixValue;
    srcFile >> std::ws;

    switch (magicSequence[1]){
        case '2':
            rawData.resize(width * height);

            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    srcFile >> rawData[j + i * width];
                }
            }
            break;

        case '5':


            for (int i=0; i < width*height; ++i){
                rawData.push_back(static_cast<int>(srcFile.get()));
            }
            break;
        default:
            throw std::runtime_error("Blad odczytu danych");
    }

    srcFile.close();


    wnImage newImage(height, width, maxPixValue, std::cref(rawData), fileTypes::PGM);


    return newImage;
}

void FileHandlerPGM::saveFile(const std::string &dstPath, const wnImage& image) {
    auto rawData = image.getRawData(fileTypes::PGM);

    std::ofstream dstFile(dstPath, std::ios_base::out | std::ios_base::binary);
    if (!dstFile.good()){
        throw std::runtime_error("Brak mozliwosci zapisu w " + dstPath);
    }

    dstFile << magicSequence << '\n';
    dstFile << "# Created by wnGraphics\n";
    dstFile << image.getWidth() << ' ' << image.getHeight()
    << ' ' << image.getMaxPixValue() << '\n';
    switch (magicSequence[1]) {
        case '2':
            for (int i=0; i<image.getHeight(); ++i){
                for (int j=0; j<image.getWidth(); ++j){
                    dstFile << rawData[j + i * image.getWidth()] << " ";
                }
                std::cout << "\n";
            }
            break;
        case '5':
            for (const auto& pix: rawData){
                dstFile.put(static_cast<char>(pix));
            }
            break;
    }

    dstFile.close();




}
