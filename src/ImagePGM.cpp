//
// Created by wojnad on 23.12.2023.
//

#include "ImagePGM.h"
#include "Filter.h"

ImagePGM::ImagePGM() {
    this->magic_seq = "P2";
    this->width = 0;
    this->height = 0;
    this->max_pix_val = 1;
}

ImagePGM::ImagePGM(const std::string& img_path) {
    std::ifstream img(img_path, std::ios_base::in | std::ios_base::binary);
    if (!img.is_open() || !img.good()){
        img.close();
        throw std::runtime_error("Nie udalo sie otworzyc obrazu.");
    }

    std::string line;
    int n_line = 2; // Numer linijki — zaczynamy od drugiej

    //Pierwsza linijka: typ pliku
    /*std::getline(img, line);
    this->magic_seq = line;
    removeWhitespace(this->magic_seq);
    std::cout << "Magiczna sekwencja: " << (this->magic_seq) << std::endl;*/
    img >> this->magic_seq;
    std::cout << "Magiczna sekwencja: " << (this->magic_seq) << std::endl;
    img.ignore(1);
    if (this->magic_seq == "P2"){
        while (std::getline(img, line)){
            if (line.starts_with("#")) continue; //Pomijanie linii z komentarzem
            std::stringstream line_buffer(line);
            switch (n_line) {
                case 2: //Druga linijka: szerokość i wysokość
                    line_buffer >> this->width >> this->height;
                    break;
                case 3: //Trzecia linijka: maksymalna wartość piksela
                    line_buffer >> this->max_pix_val;
                    break;
                default: //Pozostałe linijki z danymi;
                    int temp_pix_val; //Tymczasowa wartość odczytanego piksela
                    while (line_buffer >> temp_pix_val){
                        this->pix_map.push_back(temp_pix_val);
                    }
                    break;
            }
            n_line++;
        }
    }
    else if (this->magic_seq == "P5"){
        //Pomijanie komentarza
        while (img.peek() == '#'){
            img.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        img >> width >> height >> max_pix_val;
        img.ignore(1);

        //pix_map.resize(width*height);
        for (int i = 0; i < width*height; ++i){
            pix_map.push_back(static_cast<int>(img.get()));
        }

        img.close();
        //throw std::runtime_error("Plik binarny nie jest tymczasowo dostepny!");

        //Próby czata
        /*char comment;
        while (img.get(comment)) {
            if (comment == '#') {
                img.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                img.unget();
                break;
            }
        }

        // Wczytaj szerokość, wysokość i maksymalną wartość piksela
        img >> width >> height >> max_pix_val;
        img.ignore(1); // Pomiń biały znak (np. znak nowej linii)

        // Wczytaj właściwe dane pikseli obrazu
        pix_map.resize(width * height);
        img.read(reinterpret_cast<char*>(pix_map.data()), pix_map.size());


        std::cout << "Rozmiar obrazka: " << width * height << " Rozmiar danych: " << pix_map.size() << std::endl;*/
        //Próby moje
        /*while (std::getline(img, line) && n_line < 4){
            if (line.starts_with("#") || line.empty()) continue; //Pomijanie linii z komentarzem
            std::stringstream line_buffer(line);
            switch (n_line) {
                case 2: //Druga linijka: szerokość i wysokość
                    line_buffer >> this->width >> this->height;
                    if (this->width * this->height == 0){
                        throw std::runtime_error("Blad rozmiaru obrazka");
                    }
                    break;
                case 3: //Trzecia linijka: maksymalna wartość piksela
                    line_buffer >> this->max_pix_val;
                    break;
                default: //Pozostałe linijki z danymi;
                    *//*unsigned char temp_pix_val; //Tymczasowa wartość odczytanego piksela
                    while (line_buffer >> temp_pix_val){
                        this->pix_map.push_back(static_cast<int>(temp_pix_val));
                    }*//*
                    break;
            }
            n_line++;
        }*/

        /*img.get();
        std::vector<char> data(std::istreambuf_iterator<char>(img), {});


        std::vector<int> int_data(data.begin(), data.end());
        this->pix_map = int_data;
        std::cout << "Rozmiar obrazka:" << this->width*this->height << " Rozmiar danych:" << data.size() << std::endl;
        data.clear();*/


    }
    else{
        img.close();
        throw std::runtime_error("Bledny format pliku.");
    }

    if (pix_map.size() != width*height){
        img.close();
        throw std::runtime_error("Blad rozmiaru danych");
    }

    std::cout << "Poprawnie wczytano obraz z lokalizacji: " << img_path << std::endl;
    img.close();

}

ImagePGM::ImagePGM(int width, int height, bool isBinary) {
    this->magic_seq = (isBinary) ? "P5" : "P2"; // Odpowiednie przypisanie magicznej sekwencji
    this->width = width;
    this->height = height;
    this->max_pix_val = 255;

    this->pix_map.resize(width*height);
    for (int i=0; i<width*height; ++i){
        pix_map[i] = 0;
        //pix_map[i] = std::rand() % (max_pix_val+1);
    }
}

void ImagePGM::normalize_pic() {
    for (auto& pix: this->pix_map){
        pix = std::min(pix, this->max_pix_val);
        pix = std::max(pix, 0);
    }
}

double ImagePGM::calc_pixel(int row_n, int col_n, const Filter &filter, const std::vector<int> &old_pix_map) {
    double result = 0;

    for (int i=0; i < filter.size; ++i){
        for (int j=0; j < filter.size; ++j){
            int temp_row, temp_col; //Do kontrolowania czy wartości indeksów nie wychodzą poza zakres.
            temp_row = (int) fmax(1, row_n - ceil((double)filter.size / 2.f) + i);
            temp_col = (int) fmax(1, col_n - ceil((double)filter.size / 2.f) + j);
            temp_row = (int) fmin((double) temp_row, this->height-1);
            temp_col = (int) fmin((double) temp_col, this->width-1);
            result += old_pix_map[temp_row*this->width+temp_col] * filter.wages[i*filter.size+j];
        }
    }

    if (filter.sumK != 0){
        result /= filter.sumK;
    }

    return round(result);
}

void ImagePGM::calc_row(int row_n, const Filter &filter, const std::vector<int> &old_pix_map) {
    for (int cur_col = 0; cur_col < this->width; ++cur_col){
        this->pix_map[row_n * this->width+cur_col] = static_cast<int32_t> (calc_pixel(row_n, cur_col, filter, old_pix_map));
    }
}

void ImagePGM::calc_row_threads(int start_row, int threads_num, const Filter &filter,
                                const std::vector<int>& old_pix_map) {
    for (int cur_row = start_row; cur_row < this->height; cur_row += threads_num){
        calc_row(cur_row, filter, old_pix_map);
    }
}

void ImagePGM::apply_filter(const Filter &filter) {
    std::vector<int32_t> old_pix_map(this->pix_map);

    for (int i=0; i< this->height; ++i){
        for (int j=0; j < this->width; ++j){
            this->pix_map[i * this->width + j] = static_cast<int32_t>(calc_pixel(i, j, filter, old_pix_map));
        }
    }

    normalize_pic();
}

void ImagePGM::apply_filter_threads(const Filter &filter, int threads_num) {
    std::vector<int32_t> old_pix_map(this->pix_map);
    std::vector<std::thread> threads;

    if (threads_num <= 0) throw std::range_error("Nieprawidlowa ilosc watkow mniejsza od 1");

    for (int i=0; i < threads_num; ++i){
        threads.emplace_back(&ImagePGM::calc_row_threads, this, i, threads_num,
                             std::cref(filter), std::cref(old_pix_map));
    }
    for (auto& thread: threads){
        thread.join();
    }

    normalize_pic();
}

void ImagePGM::save_image(const std::string &dst_path) {
    std::ofstream img(dst_path, std::fstream::out | std::fstream::binary);
    if (!img.good() || !img.is_open()){
        img.close();
        throw std::runtime_error("Nie mozna uzyskac dostepu do pliku wyjscia!");
    }
    //Zapis pliku ASCII
    if (this->magic_seq == "P2"){
        img << this->magic_seq << std::endl;
        img << "# Created in FiltrObrazu" << std::endl;
        img << this->width << " " << this->height << std::endl;
        img << this->max_pix_val << std::endl;
        for (int i=0; i < this->height; ++i){
            for (int j=0; j < this->width; ++j){
                img << pix_map[this->width*i+j] << ((j < this->width-1) ? " " : "\n");
            }
        }

    }
    //Zapis pliku binarnego
    else if (this->magic_seq == "P5"){
        img << this->magic_seq << "\n";
        img << "# Created in FiltrObrazu" << "\n";
        img << this->width << " " << this->height << "\n";
        img << this->max_pix_val << "\n";
        for (int i=0; i < this->height; ++i){
            for (int j=0; j < this->width; ++j){
                img.put(static_cast<char>(pix_map[this->width*i+j]));
            }
        }
    }
    else{
        img.close();
        throw std::runtime_error("Nieprawidlowa magiczna sekwencja (powinna byc P2 lub P5)");
    }

    std::cout << "Poprawnie zapisano plik w lokalizacji: " << dst_path << std::endl;
    img.close();
}

void ImagePGM::resize_NN(int new_width, int new_height) {
    std::vector<int32_t> old_pix_map(this->pix_map);
    this->pix_map.clear();
    this->pix_map.resize(new_height*new_width);

    for (int new_row=0; new_row<new_height; ++new_row){
        for (int new_col=0; new_col<new_width; ++new_col){
            int old_row = static_cast<int>(new_row * height / static_cast<double>(new_height));
            int old_col = static_cast<int>(new_col * width / static_cast<double>(new_width));
            this->pix_map[new_row*new_width+new_col] = old_pix_map[old_row*width+old_col];
        }
    }
    width = new_width;
    height = new_height;
}
