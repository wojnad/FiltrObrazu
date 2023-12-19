//
// Created by wojnad on 06.12.2023.
//

#ifndef FILTROBRAZU_IMAGERGB_H
#define FILTROBRAZU_IMAGERGB_H
#include "ImageGray.h"

/**
 * \struct RGB
 *
 * Struktura składająca się z trzech poszczególnych składowych koloru: R, G, B.
 * */
#pragma pack(push, 1) //Potrzebne do pliku binarnego
struct RGB {
    ///Wartości poszczególnych składowych koloru.
    int b,g,r;
};
#pragma pack(pop)

/**
 * \class ImageRGB
 *
 * Klasa obrazu kolorowego RGB o rozmiarze width x height przechowująca wartość pikseli w wektorze struktur RGB
 *
 * Maksymalna wartość poszczególnej składowej piksela przechowywana w max_pix_val
 *
 * Klasa obsługuje pliki obrazów w formacie .ppm
 * */
class ImageRGB {
private:
    ///Szerokość i wysokość obrazu
    int width, height;
    ///Maksymalna wartość poszczególnych składowych RGB
    int max_pix_val;
    ///Wektor wartości pikseli (Przechowuje macierz dwuwymiarową w jednowymiarowym wektorze)
    std::vector<RGB> pix_matrix;

    /**
     * Prywatna metoda przeliczająca na podstawie filtra pojedynczy piksel obrazu przy
     * pomocy splotu obrazu
     *
     * @param row_n - Indeks wiersza obrazka
     * @param col_n - Indeks kolumny obrazka
     * @param filter - Referencja do używanego w algorytmie filtra
     * @param old_pix_matrix - Referencja do przekopiowanego wektora pikseli
     * @return Struktura zawierająca wyliczone wartości kolorów piksela
     * */

    RGB calc_pixel(int row_n, int col_n, const Filter& filter, const std::vector<RGB>& old_pix_matrix);

    /**
     * Prywatna metoda służąca do obliczenia jednego wiersza obrazka przy użyciu metody calc_pixel. Metoda do stosowania
     * w przeliczaniu przy użyciu wielu wątków
     * @param row_n - Indeks wiersza obrazka
     * @param filter - Referencja do używanego w algorytmie filtra
     * @param old_pix_matrix - Referencja do przekopiowanego wektora pierwotnych wartości pikseli
     *
     * */
     void calc_row(int row_n, const Filter& filter, const std::vector<RGB>& old_pix_matrix);
    /**
    * Prywatna metoda obsługująca obliczenia poszczególnych wierszy przez wątki - używa metody calc_row.
    * @param start_row - Początkowy wiersz
    * @param threads_num - Liczba utworzonych wątków służąca jako krok w pętli obliczania wierszy.
    * */
    void thread_calc_row(int start_row, int threads_num, const Filter& filter, const std::vector<RGB>& old_pix_matrix);
    ///Normalizacja obrazu
    void normalize_pic();
public:
    ///Konstruktor domyślny tworzący pusty obraz 0x0
    ImageRGB();
    /**
     * Konstruktor tworzący obraz na podstawie danych odczytanych z pliku
     * \param img_path - Ścieżka dostępu do pliku .ppm*/
    ImageRGB(std::string img_path);
    /**
     * Metoda odwracająca kolory w obrazie*/
    void reverse_color();
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
    /**
     * Metoda zapisująca edytowany obraz w pliku pod ścieżką dst_path
     * @param dst_path - Ścieżka docelowego pliku .ppm*/
    int save_image(const std::string& dst_path);
};


#endif //FILTROBRAZU_IMAGERGB_H
