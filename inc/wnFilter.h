//
// Created by wojnad on 29.08.2024.
//

#ifndef WNGRAPHICS_WNFILTER_H
#define WNGRAPHICS_WNFILTER_H

#include <bits/stdc++.h>

/**
 * \class Filter
 *
 * Klasa służąca do obsługi filtra-maski o rozmiarze size z wagami zapisanymi
 * wektorze.
 *
 * Domyślny filtr składa się z samych jedynek.
 *
 * Struktura pliku filtra:
 * WYSOKOŚĆ SZEROKOŚĆ
 * WAGI
 *
 * */
class wnFilter{
    ///Wysokość i szerokość filtra
    int height{}, width{};
    ///Wektor wag filtra
    std::vector<double> wages;
    ///Suma wszystkich wag
    double sumW{};
public:
    ///Konstruktor domyślny
    wnFilter() = default;
    /**
     * @brief Konstruktor tworzący domyślny filtr o zadanym rozmiarze
     *
     * @param height Wysokość filtra
     * @param width Szerokość obrazu
     * */
    explicit wnFilter(int height, int width);
    /**
     * @brief Konstruktor odczytujący filtr z pliku o opisanej powyżej strukturze
     * */
    explicit wnFilter(const std::string& srcPath);
    [[nodiscard]] const std::vector<double>& getWages() const;
    [[nodiscard]] const int& getHeight() const;
    [[nodiscard]] const int& getWidth() const;
    [[nodiscard]] const double& getSum() const;

    void showWagesValues();
};

#endif //WNGRAPHICS_WNFILTER_H
