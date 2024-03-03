//
// Created by wojnad on 03.01.2024.
//

#include "Filter.h"

Filter::Filter(int size) {
    this->size = size;
    this->wages.resize(size*size);
    std::fill(this->wages.begin(), this->wages.end(), 1.0);
    this->sumK = std::accumulate(this->wages.begin(), this->wages.end(), 0.0);
}

Filter::Filter(const std::string &filter_path) {
    std::ifstream filter(filter_path, std::fstream::in);
    if (!filter.good() || !filter.is_open()){
        filter.close();
        throw std::runtime_error("Blad otwierania pliku filtra!");
    }

    filter >> this->size;
    double temp_filter_wage;
    while(filter >> temp_filter_wage){
        this->wages.push_back(temp_filter_wage);
    }

    if (this->wages.size() != size*size){
        filter.close();
        throw std::runtime_error("Nieprawidlowy rozmiar filtra!");
    }

    this->sumK = std::accumulate(this->wages.begin(), this->wages.end(), 0.0);

    std::cout << "Poprawnie wczytano filtr z lokalizacji: " << filter_path << std::endl;
    filter.close();
}

void removeWhitespace(std::string &str) {
    str.erase(std::remove_if(str.begin(), str.end(), [](unsigned char x) { return std::isspace(x); }), str.end());
}
