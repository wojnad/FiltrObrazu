//
// Created by wojnad on 20.12.2023.
//

#ifndef FILTROBRAZU_IMAGEPPM_H
#define FILTROBRAZU_IMAGEPPM_H

#include "Filter.h"
#include "Image.h"

#pragma pack(push, 1)
/**
 * \struct RGBPixel_int32
 * Struktura przechowująca wartość RGB piksela
 *
 * Poszczególne wartości kolorów mogą być maksymalnie 32 bitowe.
 * */
struct RGBPixel_int32{
    uint32_t r,g,b;
};
#pragma pack(pop)

/**
 * \class ImagePPM
 *
 * Klasa obrazu kolorowego w formacie PPM, przechowuje obraz o wymiarach
 * width * height w std::vector<RGBPixel_int32> pixmap
 *
 * Dane są wczytywane z pliku w wypadku użycia konstruktora z podaną ścieżką dostępu jako argumentem
 * W przypadku tworzenia nowego obrazka domyślnie będzie to czarny obraz (zapisany następnie binarnie lub w ASCII)
 *
 * Obsługiwane magic_seq: P3 - ASCII, P6 - binarny
 * */
class ImagePPM : public Image{
    /// Maksymalna wartość poszczególnych składowych RGB
    unsigned int max_pix_val;
    ///Wektor wartości mikseli
    std::vector<RGBPixel_int32> pix_map;

    RGBPixel_int32 calc_pixel(int row_num, int col_num, const Filter& filter, const std::vector<RGBPixel_int32>& old_pix_map);
    void calc_row (int row_num, const Filter& filter, const std::vector<RGBPixel_int32>& old_pix_map);
    void calc_row_threads (int row_num, int threads_num, const Filter& filter, const std::vector<RGBPixel_int32>& old_pix_map);
    void normalize_pic();
public:
    ImagePPM();
    ImagePPM(int width, int height, bool is_binary);
    ImagePPM(const std::string& img_path);
    void apply_filter(const Filter& filter) override;
    void apply_filter_threads(const Filter& filter, int threads_num) override;
    void save_image(const std::string& dst_path) override;
};

#endif //FILTROBRAZU_IMAGEPPM_H
