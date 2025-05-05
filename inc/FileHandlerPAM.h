//
// Created by wojnad on 10.05.2024.
//

/**
 * @file Plik zawiera deklaracje klas Handlerów dla obrazów typu Portable anymap
 * */

#ifndef WNGRAPHICS_FILEHANDLERPAM_H
#define WNGRAPHICS_FILEHANDLERPAM_H

#include "FileHandler.h"
#include "wnImage.h"

/**
 * @class Klasa Handlera do plików PGM
 * */
class FileHandlerPGM: public FileHandler{
    std::string magicSequence;
public:
    /**
     * @brief Konstruktor handlera (podczas zapisywania np. nowych obrazów)
     * */
    FileHandlerPGM(bool isBinary);
    /**
     * @brief Odczytanie i wczytanie do klasy Obrazu pliku PGM
     * */
    wnImage readFile(const std::string& srcPath) override;
    /**
     * @brief Zapis do pliku PGM Obrazu
     * */
    void saveFile(const std::string& dstPath, const wnImage& image) override;
};

/**
 * @class Klasa handlera do pliku PPM
 * */
class FileHandlerPPM: public FileHandler{
    std::string magicSequence;
public:
    /**
     * @brief Konstruktor handlera (gdy plik jest nowy lub format jest zmieniany)
     * */
     FileHandlerPPM(bool isBinary);
    /**
     * @brief Odczytanie i wczytanie do klasy Obrazu pliku PPM
     * */
    wnImage readFile(const std::string& srcPath) override;
    /**
     * @brief Zapis do pliku PPM
     * */
    void saveFile(const std::string& dstPath, const wnImage& image) override;
};

/**
 * @class Klasa handlera do pliku PBM
 * */
class FileHandlerPBM: public FileHandler{
    std::string magicSequence;
public:
    /**
     * @brief Konstruktor handlera (do zapisu nowego pliku)
     * */
     FileHandlerPBM(bool isBinary);
    /**
     * @brief Odczytanie i wczytanie do klasy Obrazu pliku PBM
     * */
    wnImage readFile(const std::string& srcPath) override;
    /**
     * @brief Zapis do pliku PBM
     * */
    void saveFile(const std::string& dstPath, const wnImage& image) override;
};

#endif //WNGRAPHICS_FILEHANDLERPAM_H
