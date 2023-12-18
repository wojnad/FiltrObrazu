//
// Created by wojnad on 08.11.2023.
//

#include <iostream>
#include "Image.h"
using namespace std;

/**
 * @arg Ścieżka źródłowego pliku .pgm
 * @arg Ścieżka filtra
 * @arg Ścieżka docelowa filtrowanego obrazka
 * @arg Ilość wątków - opcjonalnie
 * */
int main(int argn, char** argv){
    if (argn < 4) { ///Niepoprawna liczba wprowadzonych argumentów
        cerr << "Brak wystarczajacej liczby argumentow";
        exit(0);
    }
    else if (argn == 4){
        auto start_time = std::chrono::high_resolution_clock::now();
        std::vector<std::string> args(argv, argv+argn);
        Image im(args[1]);
        Filter filter(args[2]);
        im.apply_filter(filter);
        im.save_image(args[3]);
        auto end_time = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        cout << "Czas trwania programu: "<< duration.count() << " ms" << endl;
    }
    else if (argn == 5){
        auto start_time = std::chrono::high_resolution_clock::now();
        std::vector<std::string> args(argv, argv+(argn-1));
        int threads_num = std::atoi(argv[4]); //Wczytanie liczby wątków
        Image im(args[1]);
        Filter filter(args[2]);
        im.apply_filter_threads(filter, threads_num);
        im.save_image(args[3]);
        auto end_time = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        cout << "Czas trwania programu dla " << threads_num << " watkow" << ": "<< duration.count() << " ms" << endl;
    }
}