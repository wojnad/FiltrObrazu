//
// Created by wojnad on 08.11.2023.
//

#include <iostream>
#include "ImageGray.h"
#include "ImageRGB.h"
using namespace std;

/**
 * Funkcja obsługująca filtrowanie, sprawdzająca format pliku
 * @param img_path - Ścieżka filtrowanego obrazu
 * @param filter_path - Ścieżka używanego filtra
 * @param dst_path - Ścieżka docelowa przefiltrowanego obrazu
 * @param num_threads - Ilość obsługiwanych wątków obrazu*/
int filtering(const std::string& img_path, const std::string& filter_path, const std::string& dst_path, int num_threads);
/**
 * Funkcja obsługująca odwracanie kolorów, sprawdzająca format pliku
 * @param img_path - Ścieżka odwracanego obrazu
 * @param dst_path - Ścieżka docelowa odwróconego obrazu*/
int reversing(const std::string& img_path, const std::string& dst_path);
/**
 * @arg Tryb działania:
 * -f filtrowanie
 * -r odwracanie koloru
 * @arg Ścieżka źródłowego pliku .pgm
 * @arg Ścieżka filtra
 * @arg Ścieżka docelowa filtrowanego obrazka
 * @arg Ilość wątków - opcjonalnie
 * */
int main(int argn, char** argv){
    /*int return_val = -1;
    if (argn < 4) { ///Niepoprawna liczba wprowadzonych argumentów
        cerr << "Brak wystarczajacej liczby argumentow";
        exit(-1);
    }
    else if (argn == 4){
        auto start_time = std::chrono::high_resolution_clock::now();
        std::vector<std::string> args(argv, argv+argn);
        Filter filter(args[2]);
        if (args[1].ends_with(".pgm")){
            ImageGray im(args[1]);
            im.apply_filter(filter);
            return_val = im.save_image(args[3]);
        }
        if (args[1].ends_with(".ppm")){
            ImageRGB im(args[1]);
            im.apply_filter(filter);
            return_val = im.save_image(args[3]);
        }
        auto end_time = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        cout << "Czas trwania programu bez podzialu na watki: "<< duration.count() << " ms" << endl;
    }
    else if (argn == 5){
        auto start_time = std::chrono::high_resolution_clock::now();
        std::vector<std::string> args(argv, argv+(argn-1));
        Filter filter(args[2]);
        int threads_num = std::atoi(argv[4]); //Wczytanie liczby wątków
        if (args[1].ends_with(".pgm")) { //Obraz monochromatyczny
            ImageGray im(args[1]);
            im.apply_filter_threads(filter, threads_num);
            return_val = im.save_image(args[3]);
        }
        if (args[1].ends_with(".ppm")) { //Obraz kolorowy
        }
        auto end_time = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        cout << "Czas trwania programu dla " << threads_num << " watkow" << ": "<< duration.count() << " ms" << endl;
    }
    return return_val;*/
    int return_val = 0; //Kod zakończenia programu
    if (argn < 4){
        cerr << "Zbyt mala liczba argumentow!\n";
        exit(-1);
    }
    else{
        std::vector<std::string> args(argv+1, argv+argn);
        if (args[0] == "-r"){
            return_val = reversing(args[1], args[2]);
        }
        else if (args[0] == "-f"){
            if (args.size() == 5){
                return_val = filtering(args[1], args[2], args[3], std::stoi(args[4]));
            }
            else{
                return_val = filtering(args[1], args[2], args[3], 1);
            }
        }
        else{
            cerr << "Ta opcja nie jest obslugiwana! Sprawdz czy wpisales poprawnie\n";
            return -1;
        }
    }
    return return_val;
}

int filtering(const std::string& img_path, const std::string& filter_path, const std::string& dst_path, int num_threads){
    auto start_time = std::chrono::high_resolution_clock::now();
    Filter filter(filter_path);
    int return_val = 0;
    if (img_path.ends_with(".pgm")){
        ImageGray im(img_path);
        im.apply_filter_threads(filter, num_threads);
        return_val = im.save_image(dst_path);
    }
    else if (img_path.ends_with(".ppm")){
        ImageRGB im(img_path);
        im.apply_filter_threads(filter, num_threads);
        return_val = im.save_image(dst_path);
    }
    else{
        cerr << "Nieobslugiwany format pliku!\n";
        return -1;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    cout << "Czas trwania programu dla " << num_threads << " watkow" << ": "<< duration.count() << " ms" << endl;
    return return_val;
}
int reversing(const std::string& img_path, const std::string& dst_path){
    int return_val = 0;
    if (img_path.ends_with(".pgm")){
        ImageGray im(img_path);
        im.reverse_gray();
        return_val = im.save_image(dst_path);
    }
    else if (img_path.ends_with(".ppm")){
        ImageRGB im(img_path);
        im.reverse_color();
        return_val = im.save_image(dst_path);
    }
    else{
        cerr << "Nieobslugiwany format pliku!\n";
        return -1;
    }
    return return_val;
}