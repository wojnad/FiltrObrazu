//
// Created by wojnad on 20.12.2023.
//

#ifndef FILTROBRAZU_IMAGEPGM_H
#define FILTROBRAZU_IMAGEPGM_H

#include "Filter.h"
#include "Image.h"

/**
 * \class ImagePGM
 *
 * Klasa obrazu obsługująca obraz w odcieniach szarości zapisany w pliku o rozszerzeniu PGM.
 * Przechowuje on obraz o wymiarach width*height w std::vector pix_map. Max_pix_val wskazuje na maksymalną piksela.
 *
 * Dane zczytywane są z pliku w wypadku użycia konstruktora ze ścieżką pliku.
 * W przypadku tworzenia nowego obrazka będzie to czarny obraz o zadanym rozmiarze.
 *
 * Obsługiwane magic_seq: P2 - ASCII, P5 - binarny.
 *
 * */
class ImagePGM : private Image{
    ///Maksymalna wartość odcienia szarości.
    int max_pix_val;
    ///Wektor wartości pikseli (Macierz dwuwymiarowa w jednowymiarowym wektorze)
    std::vector<int32_t> pix_map;

    /**
     * @brief Przeliczanie wartości piksela na podstawie filtra
     * @param row_n Numer wiersza piksela
     * @param col_n Numer kolumny piksela
     * @param filter Referencja do stosowanego filtra
     * @param old_pix_map Kopia pierwotnej mapy pikseli
     *
     * @return Wyliczona wartość piksela w typie double
     * */
     double calc_pixel(int row_n, int col_n, const Filter& filter, const std::vector<int>& old_pix_map);
     /**
      * @brief Przeliczanie poszczególnych pikseli w jednym wierszu obrazu
      * @param row_n Numer wiersza w obrazie
      * @param filter Referencja do stosowanego filtra
      * @param old_pix_map Referencja do kopii pierwotnej mapy pikseli
      * */
     void calc_row(int row_n, const Filter& filter, const std::vector<int>& old_pix_map);
     /**
      * @brief Obliczanie co n-tego wiersza obrazu, gdzie n to ilość wątków
      * @param start_row Wiersz startowy
      * @param threads_num Ilość utworzonych wątków
      * @param filter Referencja do stosowanego filtra
      * @param old_pix_map Kopia pierwotnej mapy pikseli
      * */
     void calc_row_threads(int start_row, int threads_num, const Filter& filter, const std::vector<int>& old_pix_map);
     /**
      * @brief Normalizowanie wartości pikseli do zakresu [0, max_pix_val]
      * */
     void normalize_pic();
public:
    ///Konstruktor domyślny tworzący pusty obraz 0x0
    ImagePGM();
    /**
     * @brief Konstruktor tworzący obraz na podstawie danych odczytanych z pliku .pgm
     * @param img_path Ścieżka do pliku .pgm
     * */
    explicit ImagePGM(const std::string& img_path);
    /**
     * Konstruktor tworzący czarny obraz o podanych wymiarach
     * */
    ImagePGM(int width, int height, bool is_binary);
    /**
     * @brief Funkcja zmieniająca rozmiar obrazu na podstawie algorytmu najbliższego sąsiada
     * @param new_width Nowa szerokość obrazu
     * @param new_height Nowa wysokość obrazu
     * */
     void resize_NN(int new_width, int new_height) override;
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
     * @brief Zapisywanie obrazu do pliku .pgm
     *
     * @param dst_path Ścieżka docelowa pliku
     * */
    void save_image(const std::string& dst_path) override;
};

#endif //FILTROBRAZU_IMAGEPGM_H
