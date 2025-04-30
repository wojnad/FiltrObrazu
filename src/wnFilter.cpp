//
// Created by wojnad on 29.08.2024.
//

#include "wnFilter.h"

wnFilter::wnFilter(int height, int width) {
    this->height = height;
    this->width = width;

    this->wages.resize(this->height * this->width, 1.0);
    this->sumW = std::accumulate(this->wages.begin(), this->wages.end(), 0.0);
}

wnFilter::wnFilter(const std::string &srcPath) {
    std::ifstream srcFile(srcPath, std::ios_base::in);
    if (!srcFile.good()){
        throw std::runtime_error("Blad otwarcia pliku filtra: " + srcPath);
    }
    while (srcFile.peek() == '#'){
        srcFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    srcFile >> height >> width;
    srcFile.ignore(1);
    wages.resize(height * width, 0.0);

    ///Odczytanie wag zapisanych w formacie ASCII
    for (auto& wage: wages){
        srcFile >> wage;
    }

    //TODO: Dodanie odczytu filtra jako plik binarny

    srcFile.close();

    sumW = std::accumulate(wages.begin(), wages.end(), 0.0);
}

const int &wnFilter::getHeight() const{
    return std::cref(height);
}

const int &wnFilter::getWidth() const {
    return std::cref(width);
}

const double &wnFilter::getSum() const {
    return std::cref(sumW);
}

const std::vector<double> &wnFilter::getWages() const {
    return std::cref(wages);
}

void wnFilter::showWagesValues() {
    for (int i=0; i<this->height; ++i){
        for (int j=0; j<this->width; ++j){
            std::cout << this->wages[i*this->width+j] << " ";
        }
        std::cout << '\n';
    }
}
