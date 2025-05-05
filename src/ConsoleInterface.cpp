//
// Created by wojnad on 14.04.2025.
//
#include "ConsoleInterface.h"

ConsoleInterface::ConsoleInterface(wnImage &editedImage): editedImage(editedImage){}

void ConsoleInterface::cliMenu() {
    int choice = 0;

    while (choice != 7 && choice != 8) {

        std::cout << "===========EDYTOR OBRAZOW===========\n\n";
        std::cout << "WYBOR NARZEDZI:\n\t1. Negatyw\n\t2. Skala szarosci\n\t3. Korekcja gamma"
                     "\n\t4. Filtrowanie\n\t5. Rotacja\n\t6. Zmiana rozmiaru\n\t7. Zapisać i wyjść\n\t8. Wyjdź bez zapisu\n";

        std::cout << "Opcja: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case 1:
                applyInversion();
                break;
            case 2:
                applyConversionToGrayscale();
                break;
            case 3:
                applyGammaCorection();
                break;
            case 4:
                applyFilter();
                break;
            case 5:
                applyRotation();
                break;
            case 6:
                applyResizing();
                break;
            case 7:
                break;
            case 8:
                exit(0);
            default:
                std::cout << "Nieprawidłowa opcja! Wybierz ponownie.\n";
                break;
        }
    }
}

void ConsoleInterface::setUsedFilter(std::unique_ptr<wnFilter> &filter) {
    this->usedFilter = std::move(filter);
}

void ConsoleInterface::applyInversion() {
    try {
        editedImage.inverseColor();
    } catch (const std::exception& e) {
        std::cerr << "Błąd podczas stosowania negatywu: " << e.what() << "\n";
    }
}

void ConsoleInterface::applyConversionToGrayscale() {
    std::cout << "Wybierz metodę konwersji:\n"
              << "\t1. Luminosity (domyślna)\n"
              << "\t2. Średnia\n"
              << "\t3. Lightness\n"
              << "Opcja: ";

    int method;
    std::cin >> method;

    grayscaleAlgos::algoType algo;
    switch (method) {
        case 1:
            algo = grayscaleAlgos::algoType::Luminosity;
            break;
        case 2:
            algo = grayscaleAlgos::algoType::Average;
            break;
        case 3:
            algo = grayscaleAlgos::algoType::Lightness;
            break;
        default:
            std::cout << "Nieprawidłowy wybór, używam metody Luminosity.\n";
            algo = grayscaleAlgos::algoType::Luminosity;
    }

    try {
        editedImage.convertToGrayscale(algo);
        std::cout << "Konwersja do skali szarości zakończona.\n";
    } catch (const std::exception& e) {
        std::cerr << "Błąd konwersji: " << e.what() << "\n";
    }
}

void ConsoleInterface::applyGammaCorection() {
    double gammaCoeff;
    std::cout << "Podaj współczynnik gamma (0.1-5.0): ";
    std::cin >> gammaCoeff;

    if (gammaCoeff < 0.1 || gammaCoeff > 5.0) {
        std::cout << "Nieprawidłowa wartość, używam domyślnej 1.0\n";
        gammaCoeff = 1.0;
    }

    try {
        editedImage.corectionGamma(gammaCoeff);
        std::cout << "Korekcja gamma zastosowana.\n";
    } catch (const std::exception& e) {
        std::cerr << "Błąd korekcji gamma: " << e.what() << "\n";
    }
}

void ConsoleInterface::applyFilter() {
    if (usedFilter == nullptr){
        std::string filterPath;

        std::cout << "Nie podano filtra\n";
        std::cout << "Podaj ścieżkę do filtra: ";
        std::getline(std::cin, filterPath);

        try {
            std::unique_ptr<wnFilter> filter(new wnFilter(filterPath));
            setUsedFilter(filter);
        } catch (const std::exception& e) {
            std::cerr << "Błąd wyboru filtra: " << e.what() << "\n";
            return;
        }
    }

    try {
        editedImage.useConvolutionFilter(std::cref(*usedFilter), 8);
        std::cout << "Filtr zastosowany pomyślnie.\n";
    } catch (const std::exception& e) {
        std::cerr << "Błąd filtrowania: " << e.what() << "\n";
    }
}

void ConsoleInterface::applyRotation() {
    double angle;
    std::cout << "Podaj kąt obrotu (w stopniach): ";
    std::cin >> angle;

    std::cout << "Wybierz metodę próbkowania:\n"
              << "\t1. Najbliższy sąsiad\n"
              << "\t2. Biliniowa (domyślna)\n"
              << "Opcja: ";

    int method;
    std::cin >> method;

    resamplingAlgos::algoType algo;
    switch (method) {
        case 1:
            algo = resamplingAlgos::algoType::NearestNeighbour;
            break;
        case 2:
            algo = resamplingAlgos::algoType::Bilinear;
            break;
        default:
            std::cout << "Nieprawidłowy wybór, używam metody biliniowej.\n";
            algo = resamplingAlgos::algoType::Bilinear;
    }

    try {
        editedImage.rotation(angle, algo);
        std::cout << "Obrót wykonany pomyślnie.\n";
    } catch (const std::exception& e) {
        std::cerr << "Błąd rotacji: " << e.what() << "\n";
    }
}

void ConsoleInterface::applyResizing() {
    double scale;
    std::cout << "Podaj współczynnik skalowania (np. 0.5 dla zmniejszenia o połowę): ";
    std::cin >> scale;

    if (scale <= 0) {
        std::cout << "Nieprawidłowa wartość, używam 1.0\n";
        scale = 1.0;
    }

    try {
        editedImage.resizeCoeff(scale, resamplingAlgos::algoType::Bilinear);
        std::cout << "Zmiana rozmiaru wykonana pomyślnie.\n";
    } catch (const std::exception& e) {
        std::cerr << "Błąd zmiany rozmiaru: " << e.what() << "\n";
    }
}
