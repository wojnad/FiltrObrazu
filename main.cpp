//
// Created by wojnad on 20.12.2023.
//

#include "Image.h"
#include "Filter.h"
#include "ImagePGM.h"
#include "ImagePPM.h"

void filtering(const std::string& img_path, const std::string& filter_path, const std::string& dst_path, int threads_num);

int main(int argc, char* argv[]){
    try{
        if (argc < 4){
            throw std::runtime_error("Zbyt mala liczba argumentow!\n"
                                     "Prawidlowa postac:\n"
                                     "FiltrObrazu.exe -f sciezka_obrazu sciezka_filtra sciezka_docelowa");
        }
        else{
            std::vector<std::string> args(argv+1, argv+argc);
            if (args[0] == "-f"){
                if (args.size() == 5){
                    filtering(args[1], args[2], args[3], std::stoi(args[4]));
                }
                else if (args.size() == 4){
                    filtering(args[1], args[2], args[3], 1);
                }
            }
        }

    }
    catch (std::exception& exp) {
        std::cerr << exp.what() << '\n';
    }

}

void filtering(const std::string& img_path, const std::string& filter_path, const std::string& dst_path, int threads_num){
    auto start_time = std::chrono::high_resolution_clock::now();
    Filter filter(filter_path);
    if (img_path.ends_with(".pgm")){
        ImagePGM img(img_path);
        img.apply_filter_threads(filter, threads_num);
        img.save_image(dst_path);
    }
    else if (img_path.ends_with(".ppm")){
        ImagePPM img(img_path);
        img.apply_filter_threads(filter, threads_num);
        img.save_image(dst_path);
    }
    else{
        throw std::runtime_error("Nieobslugiwany format pliku obrazu!");
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Czas wykonania filtracji dla liczby watkow rownej " << threads_num << ": " << duration.count() << " ms\n";
}