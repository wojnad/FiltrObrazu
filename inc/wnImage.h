//
// Created by wojnad on 10.05.2024.
//

#ifndef WNGRAPHICS_WNIMAGE_H
#define WNGRAPHICS_WNIMAGE_H

#include <bits/stdc++.h>
#include "FileHandler.h"
#include "wnFilter.h"

namespace fileTypes {
    enum types {
        PGM = 1,
        PPM,
        PBM,
        BMP,
        TGA
    };
}
namespace grayscaleAlgos {
    enum algoType {
        Lightness = 1,
        Luminosity,
        Average
    };
}

namespace resamplingAlgos {
    enum algoType{
        NearestNeighbour = 1,
        Bilinear
    };
}

/**
 * \struct RGBA
 *
 * Struktura służąca do przechowywania wartości poszczególnych pikseli
 * */
#pragma pack(push, 1)
 struct RGBA{
     ///Poszczególne wartości piksela
     uint8_t r, g, b, a;
 };
#pragma pack(pop)

/**
 * \class Image
 *
 * Klasa która służy do obsługi macierzy pikseli, niezależnie od formatu wejściowego.
 *
 * Kolory przechowywane są w formacie RGB
 * */
class wnImage{
    ///Wysokośc i szerokość obrazu
    int height{},width{};
    ///Wartość maksymalna piksela zczytana z pliku- dla obrazów binarnych domyślnie 255
    int maxPixValue=0xff;
    std::vector<RGBA> pixMap;

    //Prywatne metody do konwersji poszczególnych surowych danych na mapę pikseli RGBA

    /**
     * @brief Metoda do zamiany surowych danych pozyskanych z formatu PPM
     *
     * Jako wartość kanału alfa przyjmuje się 0xff
     *
     * @param data Zestaw surowych danych pobranych z pliku PPM
     *
     * */
    void PPMtoRGBA(const std::vector<int>& data);
    /**
     * @brief Metoda do zamiany surowych danych pozyskanych z formatu PGM
     *
     * Odcienie szarości są konwertowane na RGBA zgodnie z następującym wzorem:\n
     *
     * Gr - odcień szarości\n
     * R = Gr, G = Gr, B = Gr, A = 0xff
     *
     * @param data Zestaw surowych danych w odcieniach szarości z pliku PGM
     * */
    void PGMtoRGBA(const std::vector<int>& data);
    /**
     * @brief Metoda do zamiany surowych danych pozyskanych z formatu PBM
     *
     * Zero-jedynkowy zbiór wartości konwertuje się na zasadzie:\n
     * Bit - wartość koloru\n
     *
     * R = Bit*0xff, G = Bit*0xff, B = Bit*0xff, A = 0xff
     * */
    void PBMtoRGBA(const std::vector<int>& data){}

    /**
     * @brief Metoda do zamiany surowych danych pozyskanych z formatu TGA
     *
     * */
    void TGAtoRGBA(const std::vector<int>& data);
    void setSize(int height, int width);
    /**
     * @brief Metoda do zamiany mapy pikseli RGBA na odcienie szarości
     *
     * W celu przeliczenia wartości na skalę szarości użyto wzoru NTSC
     *
     * Gr = 0.299*R + 0.587*G + 0.114*B
     */
     [[nodiscard]] std::vector<int> RGBAtoPGM() const;
     /**
     * @brief Metoda do zamiany mapy pikseli RGBA na RGB w formacie PPM
     *
     */
    [[nodiscard]] std::vector<int> RGBAtoPPM() const;
    /**
     * @brief Metoda do zamiany mapy pikseli RGBA do formatu TGA
     * */
    [[nodiscard]] std::vector<int> RGBAtoTGA() const;
    /**
     * @brief Metoda przypisująca po uprzednim sprawdzeniu dodatniości wartość maxPixValue
     *
     * */
    void setMaxValue(int maxValue);
    /**
     * @brief Ustawienie macierzy pikseli odczytanej z pliku
     * */
    void setPixMap(const std::vector<int>& data, int fileType);
    /**
     * @brief Obliczanie wartości piksela obrazu z nałożoną maską
     *
     * \param rowNum numer wiersza
     * \param colNum numer kolumny
     * \param filter stała referencja do stosowanego filtra
     * \param oldPixMap stała referencja do początkowej macierzy pikseli
     * */
    RGBA calcPixel(int rowNum, int colNum, const wnFilter& filter,
                   const std::vector<RGBA>& oldPixMap);
    /**
     * @brief Obliczanie wartości pikseli jednego wiersza po filtracji
     *
     * \param rowNum numer wiersza
     * \param filter stała referencja do stosowanego filtra
     * \param oldPixMap stała referencja do początkowej macierzy pikseli
     * */
    void calcRow(int rowNum, const wnFilter& filter, const std::vector<RGBA>& oldPixMap);
     /**
      * @brief Obliczanie wartości pikseli dla wierszy z krokiem (do obliczeń równoległych)
      *
      * */
    void calcRows_step(int startRow, int step, const wnFilter& filter, const std::vector<RGBA>& oldPixMap);
    //Pomocnicza formuła do obliczenia piksela z algorytmu dwuliniowego
    /**
     * @brief Obliczanie wartości piksela dla algorytmów wykorzystujących resampling dwuliniowy
     *
     * @param newRow Numer wiersza obrazu po modyfikacji
     * @param newCol Numer kolumny obrazu po modyfikacji
     * @param oldRow Pozycja nowego wiersza w starym obrazie (liczba zmiennoprzecinkowa)
     * @param oldCol Pozycja nowej kolumny w starym obrazie (liczba zmiennoprzecinkowa)
     * */
    RGBA bilinearPixel(int newRow, int newCol, double oldRow, double oldCol);
    //Algorytmy resamplingu do powiększania obrazów
    /**
       * @brief Implementacja zmiany wielkości obrazu na podstawie algorytmu najbliższego sąsiada
       *
       * @param rowCoeff Współczynnik powiększenia wysokości
       * @param colCoeff Współczynnik powiększenia szerokości
       * */
    void resizeNN(double rowCoeff, double colCoeff);
    /**
     * @brief Implementacja zmiany wielkości obrazu na podstawie algorytmu dwuliniowego
     *
     * @param rowCoeff Współczynnik powiększenia wysokości
     * @param colCoeff Współczynnik powiększenia szerokości
     * */
    void resizeBL(double rowCoeff, double colCoeff);
    /**
     * @brief Implementacja rozciągnięcia obrazu z próbkowaniem najbliższego sąsiada
     *
     * @param rowCoeff Współczynnik rozszerzenia wysokości
     * @param colCoeff Współczynnik rozszerzenia szerokości
     * */

    void shearingNN(double rowCoeff, double colCoeff);
    /**
     * @brief Implementacja rozciągnięcia obrazu z próbkowaniem dwuliniowym
     *
     * @param rowCoeff Współczynnik rozszerzenia wysokości
     * @param colCoeff Współczynnik rozszerzenia szerokości
     * */
    void shearingBL(double rowCoeff, double colCoeff);
    /**
     * @brief Implementacja obrotu obrazu o wybrany kąt z próbkowaniem najbliższego sąsiada
     *
     * @param angle Kąt obrotu obrazu w stopniach
     * */
    void rotationNN(double angle);
    /**
     * @brief Implementacja obrotu obrazu o wybrany kąt z próbkowaniem dwuliniowym
     *
     * @param angle Kąt obrotu obrazu w stopniach
     * */
    void rotationBL(double angle);
public:
    ///Systemowa wartość maksymalnego piksela obrazu obrabianego w programie
    static constexpr short maxPixImage = 0xff;
    /**
     * @brief Domyślny konstruktor
     * */
    wnImage() = default;
    /**
     * @brief Tworzenie pustego obrazu o podanych wymiarach
     * */
    explicit wnImage(int height, int width);
    /**
     * @brief Konstruktor wykorzystywany przez FileHandler
     *
     * @param height Wysokość obrazu
     * @param width Szerokość obrazu
     * @param maxPixValue Maksymalna wartość piksela obrazu
     * @param rawData Surowe dane zczytane przez FileHandler
     * @param fileType Typ pliku odczytanego
     *
     * */
     explicit wnImage(int height, int width, int maxPixValue, const std::vector<int>& rawData, int fileType);

    /**
     * @brief Przekształcenie obrazu w jego negatyw
     * */
    void inverseColor();
    /**
     * @brief Konwertowanie kolorowej mapy pikseli na obraz czarnobiały
     * */
    void convertToGrayscale(int algoType);
    /**
     * @brief Implementacja algorytmu korekcji gamma
     *
     * @param gamma Współczynnik gamma
     * */
    void corectionGamma(double gamma);
    void showPixValues();
    /**
     * @brief Funkcja aplikująca filtr na obraz z możliwością dostosowania liczb stosowanych wątków
     *
     * @param filter Referencja do stosowanego filtra
     * @param numThreads Liczba stosowanych wątków
     * */
    void useConvolutionFilter(const wnFilter& filter, int numThreads);
    /**
     * @brief Konwertowanie mapy pikseli RGBA na poszczególne dane odpowiednie do zapisu w obsługiwanych formatach
     * */
    [[nodiscard]] std::vector<int> getRawData(int fileType) const;

    [[nodiscard]] const int& getHeight() const{
        return std::cref(height);
    }
    [[nodiscard]] const int& getWidth() const{
        return std::cref(width);
    }
    [[nodiscard]] const int& getMaxPixValue() const{
        return std::cref(maxPixValue);
    }

    ///Operatory

    /**
     * @brief Dodawanie dwóch obrazów do siebie
     *
     * Wartości są ucinane do granic [0, 255]
     * */
     wnImage operator+(const wnImage& second);
     /**
      * @brief Mnożenie obrazu przez liczbę
      *
      * Wartości są ucinane do granic [0, 255]
      *
      **/
     wnImage operator*(double scalar);
     /**
      * @brief Mnożenie obrazu przez obraz
      *
      * Jest to mnożenie pixel-wise (nie iloczyn skalarny)
      *
      * */
      wnImage operator*(const wnImage& second);
      // Do operacji logicznych zastosowano operacje bitowe.
      // Piksele wyjściowe mają wartość 1 lub 0.

      /**
       * @brief Operacja logiczna AND
       *
       * Działanie:
       *
       * f'(x,y) = {1: f(x,y) > 0 && g(x,y) > 0; 0 : otherwise}
       * */
      wnImage operator&(const wnImage& second);
      /**
       * @brief Operacja logiczna OR
       *
       * Działanie:
       *
       * f'(x,y) = {1: f(x,y) > 0 || g(x,y) > 0; 0 : otherwise}
       * */
       wnImage operator|(const wnImage& second);
      /**
       * @brief Operacja logiczna XOR
       *
       * Działanie:
       *
       * f'(x,y) = {1: f(x,y) > 0 ^ g(x,y) > 0; 0 : otherwise}
       * */
       wnImage operator^(const wnImage& second);
      /**
       * @brief Operacja logiczna NOT
       *
       * Działanie:
       *
       * f'(x,y) = {1: f(x,y) == 0; 0 : otherwise}
       * */
       wnImage operator!();

       ///Transformacje obrazu
       /**
        * @brief Rotacja obrazu o wybrany kąt wokół środka
        *
        * @param angle Kąt obrotu w stopniach
        * @param algoType Rodzaj wykorzystanego algorytmu resamplingu
        * */
       void rotation(double angle, int algoType);
       /**
        * @brief Przesunięcie obrazu
        *
        * @param rowDelta Ilość pikseli wysokości przesunięcia
        * @param colDelta Ilość pikseli szerokości przesunięcia
        * */
       void translation(int rowDelta, int  colDelta);
       /***
        * @brief Rozciągnięcie obrazu (Bez zmiany wielkości
        *
        * @param rowCoeff Współczynnik rozciągnięcia wysokości
        * @param colCoeff Współczynnik rozciągnięcia szerokości
        * @param algoType Rodzaj wykorzystanego algorytmu resamplingu
        * */
       void shearing(double rowCoeff, double colCoeff, int algoType);
       /**
        * @brief Powiększanie obrazu do zadanej wielkości
        *
        * @param newHeight Nowa wysokość obrazu
        * @param newWidth Nowa szerokość obrazu
        * @param algoType Rodzaj wykorzystanego algorytmu resamplingu
        * */
       void resizeVal(int newHeight, int newWidth, int algoType);
       /**
        * @brief Powiększanie obrazu razy zadany współczynnik
        *
        * @param coeff Współczynnik powiększenia obrazu
        * @param algoType Rodzaj wykorzystanego algorytmu
        * */
       void resizeCoeff(double coeff, int algoType);

       // Metody graficzne
       /**
        * @brief Kolorowanie piksela o danych współrzędnych
        *
        * @param rowNum numer wiersza
        * @param colNum numer kolumny
        * @param rgba kolor na jaki ma zostać pokolorowany piksel
        * */
       void putPixel(int rowNum, int colNum, const RGBA& rgba);
       /**
        * @brief Rysowanie prostokąta na obrazie
        *
        * @param x1 Mniejszy numer wiersza
        * @param y1 Mniejszy numer kolumny
        * @param x2 Większy numer wiersza
        * @param y2 Większy numer kolumny
        * @param rgba Kolor prostokąta
        *
        * \verbatim
        *     x1,y1*------*
        *          |      |
        *          |      |
        *          *------*x2,y2
        *\endverbatim
        * */
       void drawRectange(int x1, int y1, int x2, int y2, const RGBA& rgba);

};

#endif //WNGRAPHICS_WNIMAGE_H
