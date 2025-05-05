# Filtr Obrazu — Aplikacja Konsolowa

Aplikacja konsolowa do edycji obrazu przy pomocy najpopularniejszych algorytmów.

## Funkcjonalności
- Obsługa nieskompresowanych plików TGA oraz plików PNM (Portable Any Map)
- Podstawowe operacje:
  - Negatyw obrazu
  - Skala szarości
  - Korekcja gamma
  - Filtrowanie (konwolucja)
  - Rotacja z interpolacją
  - Zmiana rozmiaru obrazu
- Interfejs konsolowy z menu
- Obsługa argumentów wiersza poleceń

## Wymagania
- Kompilator C++ 20

## Użycie
```bash
  ./filtr-obrazu sciezka/do/pliku.tga [sciezka/docelowa.tga] [typ_pliku]
```
Po uruchomieniu wyświetli się menu z opcjami.

### Przykłady
1. Konwersja obrazu do negatywu:
```bash
  ./filtr-obrazu input.tga output.tga tga
  > #MENU#
  > Opcja: 1
  > Opcja: 7 # Zapis obrazu
```
2. Obrót obrazu o 45 stopni:
```bash
  ./filtr-obrazu input.tga output.tga tga
  > #MENU#
  > Opcja: 5
  > Podaj kąt obrotu: 45
  > Opcja: 7 # Zapis obrazu
```

## Struktura projetku

```
.
├── data/               # Przykładowe filtry
│   ├── lp_filter
│   └── ...
├── include/            # Nagłówki
│   ├── ConsoleInterface.h
│   ├── wnImage.h
│   └── ...
├── src/               # Źródła
│   ├── CMakeLists.txt # Konfiguracja Cmake dla src
│   ├── main.cpp
│   ├── ConsoleInterface.cpp
│   └── ...
├── CMakeLists.txt     # Konfiguracja CMake
└── README.md          # Ten plik
```