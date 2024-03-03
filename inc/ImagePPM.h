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

    /**
     * @brief Przeliczanie wartości piksela na podstawie filtra
     * @param row_num Numer wiersza piksela
     * @param col_num Numer kolumny piksela
     * @param filter Referencja do stosowanego filtra
     * @param old_pix_map Referencja do kopii pierwotnej mapy pikseli
     * */
    RGBPixel_int32 calc_pixel(int row_num, int col_num, const Filter& filter, const std::vector<RGBPixel_int32>& old_pix_map);
    /**
     * @brief Przeliczanie poszczególnych pikseli w jednym wierszu mapy pikseli
     * @param row_num Numer wiersza w obrazie
     * @param filter Referencja do stosowanego filtra
     * @param old_pix_map Referencja do kopii pierwotnej mapy pikseli
     * */
    void calc_row (int row_num, const Filter& filter, const std::vector<RGBPixel_int32>& old_pix_map);
    /**
      * @brief Obliczanie co n-tego wiersza obrazu, gdzie n to ilość wątków
      * @param start_row Wiersz startowy
      * @param threads_num Ilość utworzonych wątków
      * @param filter Referencja do stosowanego filtra
      * @param old_pix_map eferencja do kopii pierwotnej mapy pikseli
      * */
    void calc_row_threads (int row_num, int threads_num, const Filter& filter, const std::vector<RGBPixel_int32>& old_pix_map);
    /**
      * @brief Normalizowanie wartości kolorów pikseli do zakresu [0, max_pix_val]
      * */
    void normalize_pic();
public:
    ///@brief Konstruktor domyślny tworzący pusty obraz 0x0
    ImagePPM();
    /**
     * @brief Konstruktor tworzący czarny obraz o podanych wymiarach
     * @param width Szerokość nowego obrazu
     * @param height Wysokość nowego obrazu
     * @param is_binary Czy obraz ma być binarny
     * */
    ImagePPM(int width, int height, bool is_binary);
    /**
     * @brief Konstruktor tworzący obraz na podstawie danych odczytanych z pliku .pgm
     * @param img_path Ścieżka do pliku .pgm
     * */
    explicit ImagePPM(const std::string& img_path);
    /**
     * @brief Aplikowanie zadanego filtra na obrazie
     *
     * @param filter Referencja do stosowanego filtra
     * */
    void apply_filter(const Filter& filter) override;
    /**
     * @brief Aplikowanie zadanego filtra na obrazie, obliczenia rozdzielone pomiędzy podaną ilość wątków
     *
     * @param filter Referencja do stosowanego filtra
     * @param threads_num Ilość wątków do obliczenia
     * */
    void apply_filter_threads(const Filter& filter, int threads_num) override;
    /**
     * @brief Zapisywanie obrazu do pliku .ppm
     *
     * @param dst_path Ścieżka docelowa pliku
     * */
    void save_image(const std::string& dst_path) override;
    /**
     * @brief Funkcja zmieniająca rozmiar obrazu na podstawie algorytmu najbliższego sąsiada
     * @param new_width Nowa szerokość obrazu
     * @param new_height Nowa wysokość obrazu
     * */
    void resize_NN(int new_width, int new_height) override;
};

#endif //FILTROBRAZU_IMAGEPPM_H
