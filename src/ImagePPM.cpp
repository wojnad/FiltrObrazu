//
// Created by wojnad on 04.01.2024.
//

#include "ImagePPM.h"
#include "Filter.h"

ImagePPM::ImagePPM() {
    this->magic_seq = "P3";
    this->width = 0;
    this->height = 0;
    this->max_pix_val = 1;
}

ImagePPM::ImagePPM(int width, int height, bool is_binary) {
    this->magic_seq = (is_binary) ? "P6" : "P3";
    this->width = width;
    this->height = height;
    this->max_pix_val = 255;

    for (int i=0; i < width*height; ++i){
        RGBPixel_int32 temp = {0, 0, 0};
        this->pix_map[i] = temp;
    }
}

ImagePPM::ImagePPM(const std::string &img_path) {
    std::ifstream img(img_path, std::ios_base::in | std::ios_base::binary);
    if (!img.is_open() || !img.good()){
        img.close();
        throw std::runtime_error("Nie udalo sie otworzyc obrazu.");
    }

    std::string line;
    int n_line = 2;

    /*std::getline(img, line);
    this->magic_seq = line;*/
    img >> this->magic_seq;
    img.ignore(1);
    if (this->magic_seq == "P3"){
        while(std::getline(img, line)){
            if (line.starts_with("#")) continue;
            std::stringstream line_buffer(line);
            switch (n_line) {
                case 2:
                    line_buffer >> this->width >> this->height;
                    break;
                case 3:
                    line_buffer >> this->max_pix_val;
                    break;
                default:
                    RGBPixel_int32 temp_pix_val = {0,0,0};
                    while (line_buffer >> temp_pix_val.r >> temp_pix_val.g >> temp_pix_val.b){
                        this->pix_map.push_back(temp_pix_val);
                    }
                    break;
            }
            n_line++;
        }
    }
    else if (this->magic_seq == "P6"){
        if (img.peek() == '#'){
            img.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        img >> width >> height >> max_pix_val;
        img.ignore(1);

        for (int i = 0; i < width*height; ++i){
            RGBPixel_int32 temp_pix_val = {0,0,0};
            temp_pix_val.r = static_cast<int>(img.get());
            temp_pix_val.g = static_cast<int>(img.get());
            temp_pix_val.b = static_cast<int>(img.get());
            this->pix_map.push_back(temp_pix_val);
        }

        img.close();

        /*while(std::getline(img, line)){
            std::stringstream line_buffer(line);
            switch (n_line) {
                case 2:
                    line_buffer >> this->width >> this->height;
                    break;
                case 3:
                    line_buffer >> this->max_pix_val;
                    break;
                default:
                    unsigned char temp_rgb_val[3];
                    RGBPixel_int32 temp_pix_val = {0,0,0};
                    while(line_buffer >> temp_rgb_val[0] >> temp_rgb_val[1] >> temp_rgb_val[2]){
                        temp_pix_val = {static_cast<uint32_t>(temp_rgb_val[0]),
                                        static_cast<uint32_t>(temp_rgb_val[1]),
                                        static_cast<uint32_t>(temp_rgb_val[2])};
                        this->pix_map.push_back(temp_pix_val);
                    }
                    break;
            }
            n_line++;*/

    }
    else{
        img.close();
        throw std::runtime_error("Bledny format pliku.");
    }

    if (pix_map.size() != width*height){
        img.close();
        throw std::runtime_error("Blad rozmiaru danych.");
    }
}

RGBPixel_int32 ImagePPM::calc_pixel(int row_num, int col_num, const Filter &filter,
                                    const std::vector<RGBPixel_int32>& old_pix_map) {
    RGBPixel_int32 result = {0,0,0};
    double temp_r = 0, temp_g = 0, temp_b = 0;
    for (int i=0; i<filter.size; ++i){
        for (int j=0; j<filter.size; ++j){
            int temp_row, temp_col;
            temp_row = static_cast<int>( fmax(1, row_num - ceil((double)filter.size / 2.0) + i));
            temp_col = static_cast<int>(fmax(1, col_num - ceil((double)filter.size / 2.0) + j));
            temp_row = static_cast<int>(fmin((double) temp_row, this->height-1));
            temp_col = static_cast<int>(fmin((double) temp_col, this->width-1));
            temp_r += old_pix_map[temp_row* this->width + temp_col].r * filter.wages[i*filter.size+j];
            temp_g += old_pix_map[temp_row* this->width + temp_col].g * filter.wages[i*filter.size+j];
            temp_b += old_pix_map[temp_row* this->width + temp_col].b * filter.wages[i*filter.size+j];
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

void ImagePPM::calc_row(int row_num, const Filter &filter, const std::vector<RGBPixel_int32>& old_pix_map) {
    for (int cur_col = 0; cur_col < this->width; ++cur_col){
        this->pix_map[row_num * this->width+cur_col] = calc_pixel(row_num, cur_col, filter, old_pix_map);
    }
}

void ImagePPM::calc_row_threads(int row_num, int threads_num, const Filter &filter, const std::vector<RGBPixel_int32> &old_pix_map) {
    for (int cur_row = row_num; cur_row < this->height; cur_row+=threads_num){
        this->calc_row(cur_row, filter, old_pix_map);
    }
}

void ImagePPM::normalize_pic() {
    for (auto& pix: this->pix_map){
        pix.r = std::min(pix.r, this->max_pix_val);
        pix.r = std::max(pix.r, 0u);
        pix.g = std::min(pix.g, this->max_pix_val);
        pix.g = std::max(pix.g, 0u);
        pix.b = std::min(pix.b, this->max_pix_val);
        pix.b = std::max(pix.b, 0u);
    }
}

void ImagePPM::apply_filter(const Filter &filter) {
    auto old_pix_matrix(this->pix_map);
    for (int cur_row=0; cur_row < this->height; ++cur_row){
        for (int cur_col=0; cur_col < this->width; ++cur_col){
            this->pix_map[cur_row * this->width + cur_col] =
                    calc_pixel(cur_row, cur_col, filter, old_pix_matrix);
        }
    }
    normalize_pic();
}

void ImagePPM::apply_filter_threads(const Filter &filter, int threads_num) {
    auto old_pix_matrix(this->pix_map);
    std::vector<std::thread> threads;
    for (int i=0; i<threads_num; ++i){
        threads.emplace_back(&ImagePPM::calc_row_threads, this, i, threads_num,
                         std::cref(filter), std::cref(old_pix_matrix));
    }
    for (auto& thread: threads){
        thread.join();
    }
    normalize_pic();
    }

void ImagePPM::save_image(const std::string &dst_path) {
    std::ofstream img(dst_path, std::fstream::out | std::fstream::binary);
    if (!img){
        img.close();
        throw std::runtime_error("Nie mozna uzyskac dostepu do pliku wyjscia!");
    }

    if (this->magic_seq == "P3"){
        img << this->magic_seq << std::endl;
        img << "#Created in FiltrObrazu" << std::endl;
        img << this->width << " " << this->height << std::endl;
        img << this->max_pix_val << std::endl;

        int line_counter = 0;
        for (int i=0; i < this->height; ++i){
            for (int j=0; j < this->width; ++j){
                img << pix_map[this->width*i+j].r << " ";
                img << pix_map[this->width*i+j].g << " ";
                img << pix_map[this->width*i+j].b << ((line_counter < 3) ? " " : "\n");
                line_counter++;
                line_counter %= 4;
            }
        }
    }

    else if (this->magic_seq == "P6"){
        img << this->magic_seq << '\n';
        img << "#Created in FiltrObrazu" << '\n';
        img << this->width << " " << this->height << '\n';
        img << this->max_pix_val << '\n';
        for (int i=0; i < this->height; ++i){
            for (int j=0; j < this->width; ++j){
                img.put(static_cast<char>(pix_map[i* this->width+j].r));
                img.put(static_cast<char>(pix_map[i* this->width+j].g));
                img.put(static_cast<char>(pix_map[i* this->width+j].b));
            }
        }
    }
    else{
        img.close();
        throw std::runtime_error("Nieprawidlowa magiczna sekwencja (powinna byc P3 lub P6)");

    }

    std::cout << "Poprawnie zapisano plik w lokalizacji: " << dst_path << std::endl;
    img.close();
}

void ImagePPM::resize_NN(int new_width, int new_height) {
    std::vector<RGBPixel_int32> old_pix_map(this->pix_map);
    this->pix_map.clear();
    this->pix_map.resize(new_height*new_width);

    for (int new_row = 0; new_row < new_height; ++new_row){
        for (int new_col = 0; new_col < new_width; ++new_col){
            int old_row = static_cast<int>(new_row * height / static_cast<double>(new_height));
            int old_col = static_cast<int>(new_col * width / static_cast<double>(new_width));
            this->pix_map[new_row*new_width+new_col] = old_pix_map[old_row*width+old_col];
        }
    }
    width = new_width;
    height = new_height;
}
