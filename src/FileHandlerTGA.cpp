//
// Created by wojnad on 20.05.2024.
//

#include "FileHandlerTGA.h"

FileHandlerTGA::FileHandlerTGA() {
    header = {
        0,
        0,
        2,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        32,
        0
    };
}

wnImage FileHandlerTGA::readFile(const std::string &srcPath) {
    //height, width w nagłówku
    std::vector<int> rawData; //32bitowy piksel w środku

    std::ifstream srcFile(srcPath, std::ios_base::in | std::ios_base::binary);
    if (!srcFile.good()){
        throw std::runtime_error("Blad otwarcia pliku: " + srcPath);
    }
    srcFile.read(reinterpret_cast<char*>(&header), sizeof(header));
    srcFile.seekg(header.id_length+header.colour_map_length*header.colour_map_type, srcFile.cur);
    rawData.resize(header.height*header.width);
    //Odczyt nieskompresowanych danych
    switch (header.bits_per_pixel){
        case 32:
            srcFile.read(reinterpret_cast<char*>(rawData.data()), rawData.size()*sizeof(int));
            break;
        case 24:
            for (auto& pix: rawData){
                pix = srcFile.get(); //B
                pix |= (srcFile.get() << 8); //G
                pix |= (srcFile.get() << 16); //R
                pix |= 0xff << 24; //A
            }
            break;
        default:
            break;
    }

    wnImage newImage(header.height, header.width, 0xff, std::cref(rawData), fileTypes::TGA);

    return newImage;
}

void FileHandlerTGA::saveFile(const std::string &dstPath, const wnImage &image) {
    auto rawData = image.getRawData(fileTypes::TGA);

    header.bits_per_pixel = 32;
    header.width = image.getWidth();
    header.height = image.getHeight();

    std::ofstream dstFile(dstPath, std::ios_base::out | std::ios_base::binary);
    if (!dstFile.good()){
        throw std::runtime_error("Brak mozliwosci zapisu w " + dstPath);
    }

    dstFile.write(reinterpret_cast<const char*>(&header), sizeof(TGAFILEHEADER));
    dstFile.write(reinterpret_cast<const char*>(rawData.data()), rawData.size()* (header.bits_per_pixel/8));
    dstFile.close();
}
