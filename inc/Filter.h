//
// Created by wojnad on 20.12.2023.
//

#ifndef FILTROBRAZU_FILTER_H
#define FILTROBRAZU_FILTER_H
#include <iostream>
#include <bits/stdc++.h>

/**
 * \class Filter
 *
 * Klasa kwadratowego filtra-maski o rozmiarze size z wagami zapisanymi w
 * wektorze.
 *
 * Domyślny filtr składa się z samych jedynek.
 * Filtr może zostać też wczytany z pliku ASCII o strukturze:
 * - rozmiar w pierwszej linijce
 * - tablica wag w następnych linijkach
 * */
class Filter{
public:
    /// Konstruktor domyślny
    Filter();
    /**
     * Konstruktor tworzący filtr size x size wypełniony jedynkami.
     * */
    Filter(int size); //domyślny filtr o zadanym rozmiarze
    /**
     * Konstruktor tworzący filtr na podstawie danych z pliku tekstowego
     *
     * @param filter_path Ścieżka dostępu do pliku filtra
     * */
    Filter(const std::string& filter_path); //filtr ze ścieżki

    ///Rozmiar filtra
    int size;
    ///Macierz wag filtra
    std::vector<double> wages;
    ///Suma wszystkich wag
    double sumK;
};

/// @brief Usunięcie białych znaków z danego std::string
void removeWhitespace(std::string& str);


#endif //FILTROBRAZU_FILTER_H
