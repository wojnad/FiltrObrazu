//
// Created by wojnad on 20.12.2023.
//

#ifndef FILTROBRAZU_IMAGE_H
#define FILTROBRAZU_IMAGE_H
#include <iostream>
#include <bits/stdc++.h>

class Filter;

class Image{
public:
    std::string magic_seq; //Kod z początku pliku
    int width, height; // Wysokość i szerokość obrazu
    //Tutaj jest miejsce dla macierzy pikseli, jakiegoś headera czy cuś

    Image() = default;
    // Image (std::string img_path); //Obrazek ze ścieżki
    // Image (int width, int height); //Bazowy obrazek - czarny np.


    /**
     * Metoda, która przy użyciu metoda calc_pixel przelicza w pętli wartości dla wszystkich pikseli obrazu
     * @param filter - Referencja do używanego w algorytmie filtra
     * */
    virtual void apply_filter(const Filter& filter) = 0;
    /**
    * @brief Aplikowanie zadanego filtra na obrazie, obliczenia rozdzielone pomiędzy podaną ilość wątków
    *
    * @param filter Referencja do stosowanego filtra
    * @param threads_num Ilość wątków do obliczenia
    * */
    virtual void apply_filter_threads(const Filter& filter, int threads_num) = 0;
    /**
     * @brief Zapisywanie obrazu do pliku
     *
     * @param dst_path Ścieżka docelowa pliku
     * */
    virtual void save_image(const std::string& dst_path) = 0; //Zapisywanie wirtualne;
};

#endif //FILTROBRAZU_IMAGE_H
