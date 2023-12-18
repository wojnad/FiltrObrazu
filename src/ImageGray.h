//
// Created by wojnad on 08.11.2023.
//

#ifndef OBIEKTOWE_IMAGE_H
#define OBIEKTOWE_IMAGE_H
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

/**
 * \class Filter
 *
 * Klasa filtra składająca się z kwadratowego filtra o rozmiarze size z wagami zapisanymi w
 * kontenerze typu std::vector
 * Domyślny filtr składa się z macierzy 3x3 wypełnionej 1.
 * Istnieje możliwość wczytania go z pliku o strukturze:
 * - rozmiar w pierwszej linijce
 * - tablica wag w linijkach następnych
 * Program nie obsługuje komentarzy zawartych w pliku filtra
 * */
class Filter{
public:
    /// Rozmiar filtra.
    unsigned int size;
    /// Wektor przechowujący wagi filtra o typie double (Przechowuje macierz dwuwymiarową w jednowymiarowym wektorze).
    std::vector<double> matrix;
    /// Suma wag filtra obliczana automatycznie przez funkcję wczytującą nowy filtr.
    double sumK;
    /// Konstruktor domyślny tworzący filtr 3x3 wypełniony jedynkami
    Filter();
    /**
     * Konstruktor tworzący filtr na podstawie pliku tekstowego o strukturze opisanej w opisie klasy:
     * - rozmiar w pierwszej linijce tekstu
     * - tablica (size x size) w linijkach następnych
     *
     * @param filter_path - ścieżka pliku filtra*/
    Filter(const std::string& filter_path);
    /** Destruktor czyszczący zawartość wektora liczb, ustawia rozmiar na 0 oraz sumę na 1 (aby uniknąć ewentualnego
    dzielenia przez 0.
     */
    ~Filter();
};

/**
 * \class Image
 *
 * Klasa obrazu o rozmiarze width x height przechowująca wartość pikseli w kontenerze std::vector
 * liczb całkowitych. Max_pix_val
 * zczytywany jest automatycznie z pliku .pgm.
 *
 * Program na razie obsługuje wyłącznie pliki monochromatyczne formatu .pgm.
 * */
class Image{
    /// Szerokość i wysokość obrazu
    int width, height;
    /// Maksymalna wartość piksela zczytana z pliku pgm
    int max_pix_val;
    /// Wektor wartości pikseli (Przechowuje macierz dwuwymiarową w jednowymiarowym wektorze)
    std::vector<int> pix_matrix;
    /**
     * Prywatna metoda przeliczająca na podstawie filtra pojedynczy piksel obrazu przy pomocy splotu obrazu.
     * @param row_n - Współrzędna X piksela obrazu
     * @param col_n - Współrzędna Y piksela obrazu
     * @param filter - Referencja do używanego w algorytmie filtra
     * @param old_pix_matrix - Referencja do przekopiowanego wektora pierwotnych wartości pikseli
     * @return Wyliczona wartość piksela
     * */
    double calc_pixel(int row_n, int col_n, const Filter& filter, const std::vector<int>& old_pix_matrix);
    /**
     * Prywatna metoda służąca do obliczenia jednego wiersza obrazka przy użyciu metody calc_pixel. Metoda do stosowania
     * w przeliczaniu przy użyciu wielu wątków
     * @param row_n - Indeks wiersza obrazka
     * @param filter - Referencja do używanego w algorytmie filtra
     * @param old_pix_matrix - Referencja do przekopiowanego wektora pierwotnych wartości pikseli
     *
     * */
    void calc_row(int row_n, const Filter& filter, const std::vector<int>& old_pix_matrix);
    /**
     * Prywatna metoda obsługująca obliczenia poszczególnych wierszy przez wątki - używa metody calc_row.
     * @param start_row - Początkowy wiersz
     * @param threads_num - Liczba utworzonych wątków służąca jako krok w pętli obliczania wierszy.
     * */
    void thread_calc_row(int start_row, int threads_num, const Filter& filter, const std::vector<int>& old_pix_matrix);
    /**
     * Prywatna metoda normalizująca wartość pikseli - nowym 0 jest najmniejsza wartość piksela całego obrazka,
     * natomiast maksymalna wartość to max-min
     * */
    void normalize_pic();

public:
    ///Konstruktor domyślny tworzący pusty obraz 0x0
    Image();
    /**
     * Konstruktor odczytujący wartości z pliku .pgm przechowującego obraz
     * @param img_path - Ścieżka do pliku .pgm*/
    Image(const std::string& img_path);
    //~Image();
    /**
     * Metoda, która przy użyciu metoda calc_pixel przelicza w pętli wartości dla wszystkich pikseli obrazu
     * @param filter - Referencja do używanego w algorytmie filtra
     * */
    void apply_filter(const Filter& filter);
    /**
     * Metoda, która przy użyciu metody calc_row przelicza wartości dla wszystkich pikseli obrazu tworząc odpowiednią
     * ilość wątków, przekazywaną w parametrze num_threads
     *
     * @param filter - Referencja do używanego w algorytmie filtra
     * @param num_threads - Liczba wątków do utworzenia
     * */
    void apply_filter_threads(const Filter& filter, unsigned int num_threads);
    /// Metoda odwracająca odcienie obrazu
    void reverse_gray();
    /// Metoda wyświetlająca wartości pikseli w formie tablicy liczb
    void print();
    /**
     * Metoda zapisująca przefiltrowany obraz do pliku .pgm
     * @param dst_path - Ścieżka docelowa pliku*/
    int save_image(const std::string& dst_path);
};


#endif //OBIEKTOWE_IMAGE_H
