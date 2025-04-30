//
// Created by wojnad on 20.05.2024.
//

#ifndef WNGRAPHICS_FILEHANDLERTGA_H
#define WNGRAPHICS_FILEHANDLERTGA_H

#include "FileHandler.h"
#include "wnImage.h"

#pragma pack(push, 1)
struct TGAFILEHEADER{
    uint8_t id_length;
    uint8_t colour_map_type;
    uint8_t data_type_code;
    uint16_t colour_map_origin;
    uint16_t colour_map_length;
    uint8_t colour_map_depth;
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t bits_per_pixel;
    uint8_t image_descriptor;
};
#pragma pack(pop)

/**
 * @class Klasa handlera dla obrazu TGA\n
 *
 * Zawiera ona nagłówek pliku TGA. Dane są odczytane do wektora int, który
 * ma w sobie składowe ułożone z następującym wzorem: BGRA
 *
 * W przypadku obrazów 24bitowych A jest domyślnie ustawiane jako 0xff
 * */
class FileHandlerTGA: public FileHandler{
    TGAFILEHEADER header;
public:
    /**
     * @brief Konstruktor podstawowy handlera (same zera, 32 bpp, wymiary podczas zapisu)
     * */
    FileHandlerTGA();
    wnImage readFile(const std::string &srcPath) override;
    void saveFile(const std::string &dstPath, const wnImage &image) override;
};

#endif //WNGRAPHICS_FILEHANDLERTGA_H
