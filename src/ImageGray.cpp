//
// Created by wojnad on 08.11.2023.
//

#include "Image.h"
#include <bits/stdc++.h>

Image::Image() {
    width=0, height=0, max_pix_val=0;
}

Image::Image(const std::string& img_path) {
    std::fstream img_file(img_path, std::fstream::in);
    if (!img_file.good()){ // Sprawdzenie, czy plik jest dostępny.
        img_file.close();
        std::cerr << "Plik obrazu niedostepny!";
        exit(-1);
    }
    if (img_file.is_open()){
        std::string line;
        int n_line = 1;
        while(std::getline(img_file, line)){
            if (line[0] == '#') continue; // Linia z komentarzem jest pomijana.
            if (n_line > 1){
                std::stringstream linebufor(line); // Bufor do zczytywania wartości z poszczególnych linijek.
                switch (n_line) {
                    case 2: // Linijka druga zawiera szerokość i wysokość.
                        linebufor >> this->width >> this->height;
                        break;
                    case 3: // Linijka trzecia zawiera maksymalną wartość piksela w obrazku.
                        linebufor >> this->max_pix_val;
                        break;
                    default: // Pozostałe linijki powinny zawierać tablicę wartości pikseli.
                        int pix_val;
                        while (linebufor>>pix_val){
                            this->pix_matrix.push_back(pix_val);
                        }
                        break;
                }
            }
            n_line++;
        }
        img_file.close();
    }
    else{
        img_file.close();
        exit(-1);
    }
}

void Image::print() {
    std::cout << "Width: " << this->width << " Height: " << this->height <<std::endl;
    std::cout << "Max value: " << this->max_pix_val <<std::endl;

    // Wyświetlanie przy użyciu podwójnej pętli wartości poszczególnych pikseli.
    for (int i=0; i < this->height; ++i){
        for (int j=0; j < this->width; ++j){
            std::cout << this->pix_matrix[this->width*i+j] << " ";
        }
        std::cout << "\n";
    }
}

void Image::reverse_gray(){
    for (int i = 0; i < this->height * this->width; ++i){
        this->pix_matrix[i] = max_pix_val - this->pix_matrix[i];
    }
}

double Image::calc_pixel(int row_n, int col_n, const Filter &filter, const std::vector<int>& old_pix_matrix) {
    double result = 0;

    for (int i=0; i<filter.size; ++i){
        for (int j=0; j<filter.size; ++j){
            int temp_row, temp_col; //Do kontrolowania czy wartości indeksów nie wychodzą poza zakres.
            temp_row = (int) fmax(1, row_n - ceil((double)filter.size / 2.f) + i);
            temp_col = (int) fmax(1, col_n - ceil((double)filter.size / 2.f) + j);
            temp_row = (int) fmin((double) temp_row, this->height-1);
            temp_col = (int) fmin((double) temp_col, this->width-1);
            result += old_pix_matrix[temp_row*this->width+temp_col] * filter.matrix[i*filter.size+j];
        }
    }
    if (filter.sumK != 0){ // Gdy suma jest równa 0, wtedy dzielimy przez 1, czego nie trzeba zapisywać
        result /= filter.sumK;
    }


    return round(result); // Zwracana jest wartość zaokrąglona, bo piksele zapisane są jako liczby całkowite.
}

void Image::calc_row(int row_n, const Filter& filter, const std::vector<int>& old_pix_matrix){
    for (int cur_col = 0; cur_col<this->width; ++cur_col){ //Przejście przez wszystkie kolumny wiersza
        this->pix_matrix[row_n * this->width+cur_col] = static_cast<int>(calc_pixel(row_n, cur_col, filter, old_pix_matrix));
    }
}

void Image::thread_calc_row(int start_row, int threads_num, const Filter& filter, const std::vector<int>& old_pix_matrix) {
    for (int cur_row = start_row; cur_row < this->height; cur_row += threads_num){
        this->calc_row(cur_row, filter, old_pix_matrix);
    }
}

void Image::normalize_pic() {
    const auto [min_ptr, max_ptr] = std::minmax_element(this->pix_matrix.begin(), this->pix_matrix.end());
    int min = *min_ptr, max = *max_ptr; // Odczytanie wartości minimalnej i maksymalnej obrazka
    if (min < 0){
        this->max_pix_val = max - min;
        for (int i=0; i< this->height * this->width; ++i){
            this->pix_matrix[i] -= min;
        }
    }

}

void Image::apply_filter(const Filter &filter) {
    std::vector<int> old_pix_matrix(this->pix_matrix); // Przekopiowane pierwotne wartości pikseli
    for (int i=0; i<this->height; ++i){
        for (int j=0; j< this->width; ++j){
            // Obliczanie nowych wartości dla wszystkich pikseli
            this->pix_matrix[i*this->width+j] = static_cast<int> (this->calc_pixel(i, j, filter, old_pix_matrix));
        }
    }

    this->normalize_pic();

    /*//Odczytanie obecnego minimum i maksimum wartości pikseli przed normalizacją
    const auto[min_ptr, max_ptr] = std::minmax_element(this->pix_matrix.begin(), this->pix_matrix.end());
    int min = *min_ptr, max = *max_ptr;
      //Normalizacja wartości pikseli do wartości 0 - max
    if (min < 0){
        double cur_pix_value, cur_max_value;
        cur_max_value = static_cast<double>(max - min);
        for (int i=0; i< this->height * this->width; ++i){
            cur_pix_value = static_cast<double> (this->pix_matrix[i]-(min));
            //if (cur_pix_value < 0) std::cout << cur_pix_value << std::endl;
            this->pix_matrix[i] = static_cast<int>(round(this->max_pix_val * (cur_pix_value/cur_max_value)));
        }
        std::cout << *min_ptr << std::endl;
    }
    if (min < 0){
        this->max_pix_val = max - min;
        for (int i=0; i< this->height * this->width; ++i){
            this->pix_matrix[i] -= min;
        }
    }*/
}

int Image::save_image(const std::string& dst_path) {
    std::ofstream img_file(dst_path, std::fstream::out);
    if (!img_file.good()){
        std::cerr << "Nie mozna uzyskac dostepu do pliku wejsciowego!";
        return -1;
    }
    if (img_file.is_open()){
        img_file << "P2" << std::endl;
        img_file << "# Created in FiltrObrazu" << std::endl;
        img_file << this->width << " " << this->height << std::endl;
        img_file << this->max_pix_val << std::endl;
        for (int i = 0; i< this->height; ++i){
            for (int j=0; j< this->width; ++j){
                img_file << pix_matrix[this->width*i+j] << ((j < this->width-1) ? " " : "\n");
            }

        }
        img_file.close();
        return 0;
    }
    else{
        std::cerr << "Błąd podczas zapisu pliku" << "\n";
        return -1;
    }
}

void Image::apply_filter_threads(const Filter& filter, unsigned int num_threads){
    std::vector<int> old_pix_matrix(this->pix_matrix); // Przekopiowane pierwotne wartości pikseli
    std::vector<std::thread> threads; // Wektor wątków
    for (unsigned int i=0; i<num_threads; ++i){
        threads.emplace_back(&Image::thread_calc_row, this, i, num_threads, std::ref(filter), std::cref(old_pix_matrix));
        //Konfiguracja wątku
    }
    for (std::thread& thread: threads){
        thread.join(); // Połączenie wątków
    }
    this->normalize_pic(); //Normalizacja obrazka
}

Filter::Filter() {
    this->size = 3;
    for (unsigned int i=0; i< this->size * this->size; ++i) {
        this->matrix.push_back(1);
    }
    this->sumK = 9.f;
}
Filter::Filter(const std::string& filter_path) {
    std::fstream filter_file(filter_path, std::fstream::in);
    if (!filter_file.good()){
        filter_file.close();
        std::cerr << "Nie mozna uzyskac dostepu do pliku filtra!";
        exit(-1);
    }
    if (filter_file.is_open()){
        std::string line;
        int n_line = 1;
        while (std::getline(filter_file, line)){
            std::stringstream linebufor(line);
            if (n_line == 1){
                linebufor >> this->size;
            }
            else{
                double filter_val;
                while (linebufor >> filter_val){
                    this->matrix.push_back(filter_val);
                }
            }
            n_line++;
        }
        for (double element : this->matrix){
            sumK += element;
        }
        if (sumK == 0){
            sumK = 1;
        }
        filter_file.close();
    }
    else{
        filter_file.close();
        exit(-1);
    }

}
Filter::~Filter() {
    this->size = 0;
    this->matrix = std::vector<double>();
    this->sumK = 1;
}