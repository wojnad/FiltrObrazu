//
// Created by wojnad on 06.12.2023.
//

#include "ImageRGB.h"

ImageRGB::ImageRGB() {
    width = 0, height = 0, max_pix_val = 0;
}

ImageRGB::ImageRGB(std::string img_path) {
    std::fstream img_file(img_path, std::fstream::in);
    if (!img_file.good()){ //Sprawdzenie, czy plik jest dostępny.
        img_file.close();
        std::cerr << "Plik obrazu niedostepny!";
        exit(-1);
    }

    if(img_file.is_open()){
        std::string line;
        int n_line = 1;
        while(std::getline(img_file, line)){
            if (line[0] == '#') continue; // Linia z komentarzem
            if (n_line > 1){
                std::stringstream linebufor(line); // Bufor do zczytywania poszczególnych wartości
                switch(n_line){
                    case 2:
                        linebufor >> this->width >> this->height;
                        break;
                    case 3:
                        linebufor >> this->max_pix_val;
                        break;
                    default:
                        RGB temp;
                        while (linebufor>>temp.r>>temp.g>>temp.b){
                            this->pix_matrix.push_back(temp);
                        }
                        break;
                }
            }
            n_line++;
        }
        if (pix_matrix.size() != width*height){ // Zadeklarowana wielkość nie odpowiada zczytanej
            std::cerr << "Blad rozmiaru obrazu!";
            exit(-1);
        }
        img_file.close();
    }
    else{
        std::cerr << "Blad dostepu do pliku obrazu!";
        img_file.close();
        exit(-1);
    }
}

RGB ImageRGB::calc_pixel(int row_n, int col_n, const Filter &filter, const std::vector<RGB> &old_pix_matrix) {
    RGB result = {0, 0, 0};
    double temp_r=0, temp_g=0, temp_b=0;

    for (int i=0; i<filter.size; ++i){
        for (int j=0; j<filter.size; ++j){
            int temp_row, temp_col; //Do kontrolowania czy wartości indeksów nie wychodzą poza zakres.
            temp_row = (int) fmax(1, row_n - ceil((double)filter.size / 2.f) + i);
            temp_col = (int) fmax(1, col_n - ceil((double)filter.size / 2.f) + j);
            temp_row = (int) fmin((double) temp_row, this->height-1);
            temp_col = (int) fmin((double) temp_col, this->width-1);
            temp_r += old_pix_matrix[temp_row*this->width+temp_col].r * filter.matrix[i*filter.size+j];
            temp_g += old_pix_matrix[temp_row*this->width+temp_col].g * filter.matrix[i*filter.size+j];
            temp_b += old_pix_matrix[temp_row*this->width+temp_col].b * filter.matrix[i*filter.size+j];
        }
    }
    if (filter.sumK != 0){
        temp_r /= filter.sumK;
        temp_g /= filter.sumK;
        temp_b /= filter.sumK;
    }
    result.r = static_cast<int>(round(temp_r));
    result.g = static_cast<int>(round(temp_g));
    result.b = static_cast<int>(round(temp_b));
    return result;
}

void ImageRGB::calc_row(int row_n, const Filter &filter, const std::vector<RGB> &old_pix_matrix) {
    for (int cur_col = 0; cur_col < this->width; ++cur_col){ //Przejście przez wszystkie kolumny wiersza
        this->pix_matrix[row_n * this->width+cur_col] = calc_pixel(row_n, cur_col, filter, old_pix_matrix);
    }
}

void ImageRGB::thread_calc_row(int start_row, int threads_num, const Filter &filter,
                               const std::vector<RGB> &old_pix_matrix) {
    for (int cur_row = start_row; cur_row < this->height; cur_row += threads_num){
        this->calc_row(cur_row, filter, old_pix_matrix);
    }
}

void ImageRGB::apply_filter(const Filter& filter){
    std::vector<RGB> old_pix_matrix(this->pix_matrix); //Przekopiowane pierwotne wartości pikseli
    for (int i=0; i< this->height; ++i){
        for (int j=0; j < this->width; ++j){
            // Obliczanie nowych wartości dla wszystkich pikseli
            this->pix_matrix[i*this->width+j] = this->calc_pixel(i, j, filter, old_pix_matrix);
        }
    }
    normalize_pic();
}

void ImageRGB::apply_filter_threads(const Filter &filter, unsigned int num_threads) {
    std::vector<RGB> old_pix_matrix(this->pix_matrix);
    std::vector<std::thread> threads; //Wektor wątków
    for (unsigned int i=0; i<num_threads; ++i){
        threads.emplace_back(&ImageRGB::thread_calc_row, this, i, num_threads,
                             std::cref(filter), std::cref(old_pix_matrix));
    }
    for (auto& thread: threads){
        thread.join(); //Połączenie wątków
    }
    this->normalize_pic(); //Normalizacja koloru
}

void ImageRGB::reverse_color() {
    for (int i=0; i < this->height * this->width; ++i){
        this->pix_matrix[i].r = max_pix_val - this->pix_matrix[i].r;
        this->pix_matrix[i].g = max_pix_val - this->pix_matrix[i].g;
        this->pix_matrix[i].b = max_pix_val - this->pix_matrix[i].b;
    }
}

int ImageRGB::save_image(const std::string& dst_path) {
    std::ofstream img_file(dst_path, std::fstream::out);
    if (!img_file.good()){
        std::cerr << "Nie mozna uzyskac dostepu do pliku wyjsciowego";
        return -1;
    }
    if (img_file.is_open()){
        img_file << "P3" << std::endl;
        img_file << "# Created in FiltrObrazu" << std::endl;
        img_file << this->width << " " << this->height << std::endl;
        img_file << this->max_pix_val << std::endl;

        int line_counter = 0; //Licznik R G B w jednej linii w celu ograniczenia znaków;
        for (int i = 0; i < this->height; ++i){
            for (int j=0; j < this->width; ++j){
                img_file << pix_matrix[this->width*i+j].r << " ";
                img_file << pix_matrix[this->width*i+j].g << " ";
                img_file << pix_matrix[this->width*i+j].b << ((line_counter < 3) ? " " : "\n");
                line_counter++;
                line_counter %= 4;
            }
        }
        img_file.close();
        return 0;
    }
    else{
        std::cerr << "Blad podczas zapisu pliku!" << '\n';
        return -1;
    }
}

void ImageRGB::normalize_pic() {
    for (auto& pixel: this->pix_matrix){
        pixel.r = std::max(0, std::min(pixel.r, max_pix_val));
        pixel.g = std::max(0, std::min(pixel.g, max_pix_val));
        pixel.b = std::max(0, std::min(pixel.b, max_pix_val));
    }
}
