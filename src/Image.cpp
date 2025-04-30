//
// Created by wojnad on 17.05.2024.
//

#include "wnImage.h"

void wnImage::setMaxValue(int maxValue) {
    if (maxValue < 1){
        throw std::runtime_error("Maksymalna wartosc piksela mniejsza od 1");
    }
    this->maxPixValue = maxValue;
}

void wnImage::setSize(int height, int width) {
    if (height < 1 || width < 1){
        throw std::runtime_error("Wymiary obrazu powinny być większe od 1");
    }

    this->height = height;
    this->width = width;
    pixMap.resize(height*width);
}

void wnImage::showPixValues() {
    for (auto& pix: pixMap){
        std::cout << +pix.r << " " << +pix.b << " " << +pix.g << " " << +pix.a << "\n";
    }
}

void wnImage::setPixMap(const std::vector<int> &data, int fileType) {
    switch (fileType) {
        case fileTypes::PBM:
        case fileTypes::PGM:
            PGMtoRGBA(std::cref(data));
            break;
        case fileTypes::PPM:
            PPMtoRGBA(std::cref(data));
            break;
        case fileTypes::TGA:
            TGAtoRGBA(std::cref(data));
            break;
        case fileTypes::BMP:
        default:
            throw std::runtime_error("Niezaimplementowany format pliku do otwarcia");
    }
}

void wnImage::PGMtoRGBA(const std::vector<int> &data) {
    double rawToIm = static_cast<double>(maxPixImage) / maxPixValue;
    for (int r=0; r<height; ++r){
        for (int c=0; c<width; ++c){
            pixMap[r*width+c].r = static_cast<uint8_t>(rawToIm * data[r*width+c]);
            pixMap[r*width+c].g = static_cast<uint8_t>(rawToIm * data[r*width+c]);
            pixMap[r*width+c].b = static_cast<uint8_t>(rawToIm * data[r*width+c]);
            pixMap[r*width+c].a = 0xff;
        }
    }
}

void wnImage::PPMtoRGBA(const std::vector<int> &data) {
    double rawToIm = static_cast<double>(maxPixImage) / maxPixValue;
    for (int r=0; r<height; ++r){
        for (int c=0; c<width; ++c){
            pixMap[r*width+c].r = static_cast<uint8_t>(rawToIm * data[3*(r*width+c)]);
            pixMap[r*width+c].g = static_cast<uint8_t>(rawToIm * data[3*(r*width+c)+1]);
            pixMap[r*width+c].b = static_cast<uint8_t>(rawToIm * data[3*(r*width+c)+2]);
            pixMap[r*width+c].a = 0xff;
        }
    }
}

void wnImage::TGAtoRGBA(const std::vector<int> &data) {
    for (int r = 0, nr=height-1; r< height; ++r, --nr){
        for (int c=0; c<width; ++c){
            const auto& pix = data[nr*width+c];
            pixMap[r*width+c].a = ((pix & 0xff000000) >> 24);
            pixMap[r*width+c].r = ((pix & 0x00ff0000) >> 16);
            pixMap[r*width+c].b =  ((pix & 0x0000ff00) >> 8);
            pixMap[r*width+c].g = (pix & 0x000000ff);
        }
    }
}

// Konstruktory

wnImage::wnImage(int height, int width) {
    this->height = height;
    this->width = width;

    pixMap.resize(height*width, {0xff, 0xff, 0xff, 0xff});
}

wnImage::wnImage(int height, int width, int maxPixValue, const std::vector<int>& rawData, int fileType){
    setSize(height, width);
    setMaxValue(maxPixValue);
    setPixMap(std::cref(rawData), fileType);
}

// Konwersja danych do typów wyjściowych

std::vector<int> wnImage::RGBAtoPGM() const{
    std::vector<int> rawData(width*height, 0);
    ///Współczynnik maksymalnej wartości wyjściowej do maksymalnej wartości obszaru roboczego
    double imToRaw = static_cast<double>(maxPixValue) / maxPixImage;
    for (int r=0; r<height; ++r){
        for (int c=0; c<width; ++c){
            ///Stała referencja do wartości piksela o obecnych współrzędnych
            const auto& curPix = this->pixMap[r*width+c];
            rawData[r*width+c] = static_cast<int>(round(0.299*imToRaw*curPix.r +
                                                        0.587*imToRaw*curPix.g +
                                                        0.114*imToRaw*curPix.b));
        }
    }
    return rawData;
}

std::vector<int> wnImage::RGBAtoPPM() const {
    std::vector<int> rawData;
    double imToRaw = static_cast<double>(maxPixValue) / maxPixImage;

    for (const auto& pix: pixMap){
        rawData.push_back(static_cast<int>(imToRaw * pix.r));
        rawData.push_back(static_cast<int>(imToRaw * pix.g));
        rawData.push_back(static_cast<int>(imToRaw * pix.b));
    }

    return rawData;
}

std::vector<int> wnImage::RGBAtoTGA() const {
    std::vector<int> rawData(height*width);

    for (int r = 0, nr=height-1; r< height; ++r, --nr){
        for (int c=0; c<width; ++c){
            int pix = (pixMap[r*width+c].a << 24);
            pix |= (pixMap[r*width+c].r << 16);
            pix |= (pixMap[r*width+c].b << 8);
            pix |= (pixMap[r*width+c].g);
            rawData[nr*width+c] = pix;
        }
    }

    return rawData;
}

std::vector<int> wnImage::getRawData(int fileType) const{
    switch (fileType){
        case fileTypes::PGM:
        case fileTypes::PBM:
            return RGBAtoPGM(); // Dane pliku PGM i PBM różnią się tylko tym, że PBM ma maxValue 1
        case fileTypes::PPM:
            return RGBAtoPPM();
        case fileTypes::TGA:
            return RGBAtoTGA();
        case fileTypes::BMP:
        default:
            throw std::runtime_error("Niezaimplementowany format pliku do zapisu");
    }
}

// Podstawowe operacje jednopikselowe

void wnImage::inverseColor() {
    for (auto& pix: pixMap){
        pix.r = 0xff - pix.r;
        pix.g = 0xff - pix.g;
        pix.b = 0xff - pix.b;
    }
}

void wnImage::convertToGrayscale(int algoType) {
    switch (algoType) {
        case grayscaleAlgos::Average:
            for (auto& pix: pixMap){
                auto avg = static_cast<uint8_t>(static_cast<double>(pix.r + pix.g + pix.b) / 3.0);
                pix.r = avg;
                pix.g = avg;
                pix.b = avg;
            }
            break;
        case grayscaleAlgos::Lightness:
            for (auto& pix: pixMap){
                auto max = std::max(pix.r, pix.g);
                max = std::max(max, pix.b);
                auto min = std::min(pix.r, pix.g);
                min = std::min(min, pix.b);

                auto avg = static_cast<uint8_t>(static_cast<double>(max+min) / 2.0);

                pix.r = avg;
                pix.g = avg;
                pix.b = avg;
            }
            break;
        case grayscaleAlgos::Luminosity:
            for (auto& pix: pixMap){
                auto gray = static_cast<uint8_t>(
                        round(0.299 * pix.r + 0.587 * pix.g + 0.114 *  pix.b));

                pix.r = gray;
                pix.g = gray;
                pix.b = gray;
            }
            break;
        default:
            break;
    }
}

void wnImage::corectionGamma(double gamma) {
    for (auto& pix: pixMap){
        double tempColor;

        tempColor = static_cast<double>(pix.r);
        tempColor /= static_cast<double>(maxPixImage);
        tempColor = pow(tempColor, gamma);
        tempColor *= static_cast<double>(maxPixImage);
        pix.r = static_cast<uint8_t>(round(tempColor));

        tempColor = static_cast<double>(pix.g);
        tempColor /= static_cast<double>(maxPixImage);
        tempColor = pow(tempColor, gamma);
        tempColor *= static_cast<double>(maxPixImage);
        pix.g = static_cast<uint8_t>(round(tempColor));

        tempColor = static_cast<double>(pix.b);
        tempColor /= static_cast<double>(maxPixImage);
        tempColor = pow(tempColor, gamma);
        tempColor *= static_cast<double>(maxPixImage);
        pix.b = static_cast<uint8_t>(round(tempColor));

    }
}

// Funkcje dla filtrów

void wnImage::useConvolutionFilter(const wnFilter &filter, int numThreads) {
    auto oldPixMap(this->pixMap);
    std::vector<std::thread> threads;

    for (int i=0; i<numThreads; ++i){
        threads.emplace_back(&wnImage::calcRows_step, this, i, numThreads,
                             std::cref(filter), std::cref(oldPixMap));
    }

    for (auto& thread: threads){
        thread.join();
    }
}

RGBA wnImage::calcPixel(int rowNum, int colNum, const wnFilter &filter, const std::vector<RGBA> &oldPixMap) {
    RGBA resultPixel{0, 0, 0, 0};

    double tempR = 0., tempG = 0., tempB = 0.;
    for (int i=0; i < filter.getHeight(); ++i){
        for (int j=0; j < filter.getWidth(); ++j){
            int tempRow, tempCol;
            tempRow = static_cast<int>
                    (std::max(0., rowNum - ceil(static_cast<double>(filter.getHeight()) / 2.0) + i));
            tempCol = static_cast<int>
                    (std::max(0., colNum - ceil(static_cast<double>(filter.getWidth()) / 2.0) + j));
            tempRow = std::min(tempRow, this->height-1);
            tempCol = std::min(tempCol, this->width-1);

            tempR += oldPixMap[tempRow * this->width + tempCol].r * filter.getWages()[i*filter.getWidth()+j];
            tempG += oldPixMap[tempRow * this->width + tempCol].g * filter.getWages()[i*filter.getWidth()+j];
            tempB += oldPixMap[tempRow * this->width + tempCol].b * filter.getWages()[i*filter.getWidth()+j];
        }
    }
    if (filter.getSum() != 0){
        tempR /= filter.getSum();
        tempG /= filter.getSum();
        tempB /= filter.getSum();
    }

    tempR = std::max(std::min(tempR, 255.0), 0.0);
    tempG = std::max(std::min(tempG, 255.0), 0.0);
    tempB = std::max(std::min(tempB, 255.0), 0.0);

    resultPixel.r = static_cast<uint8_t>(round(tempR));
    resultPixel.g = static_cast<uint8_t>(round(tempG));
    resultPixel.b = static_cast<uint8_t>(round(tempB));
    resultPixel.a = oldPixMap[rowNum*this->width + colNum].a;

    return resultPixel;
}

void wnImage::calcRow(int rowNum, const wnFilter& filter, const std::vector<RGBA>& oldPixMap){
    for (int colNum=0; colNum < this->width; ++colNum){
        this->pixMap[rowNum*this->width + colNum] = calcPixel(rowNum, colNum, filter, oldPixMap);
    }
}

void wnImage::calcRows_step(int startRow, int step, const wnFilter &filter, const std::vector<RGBA> &oldPixMap) {
    for (int curRow = startRow; curRow < this->height; curRow+=step){
        calcRow(curRow, filter, oldPixMap);
    }
}

// Operacje arytmetyczne na obrazach

wnImage wnImage::operator+(const wnImage &second) {
    if (this->width != second.width || this->height != second.height){
        throw std::runtime_error("Blad rozmiaru obrazu: Oba obrazy maja rozna wielkosc");
    }

    auto newImage = wnImage(this->height, this->width);

    for (int i=0; i < this->height*this->width; ++i){
        RGBA result{0,0,0,0};
        result.r = std::max(std::min(this->pixMap[i].r + second.pixMap[i].r, 0xff), 0);
        result.g = std::max(std::min(this->pixMap[i].g + second.pixMap[i].g, 0xff), 0);
        result.b = std::max(std::min(this->pixMap[i].b + second.pixMap[i].b, 0xff), 0);
        result.a = std::max(std::min(this->pixMap[i].a + second.pixMap[i].a, 0xff), 0);
        newImage.pixMap[i] = result;
    }

    return newImage;
}

wnImage wnImage::operator*(double scalar) {
    auto newImage = wnImage(this->height, this->width);

    for (int i=0; i < this->height*this->width; ++i){
        RGBA result{0, 0, 0, 0};
        result.r = std::max(std::min(static_cast<int>(this->pixMap[i].r * scalar), 0xff), 0);
        result.g = std::max(std::min(static_cast<int>(this->pixMap[i].g * scalar), 0xff), 0);
        result.b = std::max(std::min(static_cast<int>(this->pixMap[i].b * scalar), 0xff), 0);
        result.a = std::max(std::min(static_cast<int>(this->pixMap[i].a * scalar), 0xff), 0);
        newImage.pixMap[i] = result;
    }
    return newImage;
}

wnImage wnImage::operator*(const wnImage& second) {
    if (this->width != second.width || this->height != second.height){
        throw std::runtime_error("Blad rozmiaru obrazu: Oba obrazy maja rozna wielkosc");
    }

    auto newImage = wnImage(this->height, this->width);

    for (int i=0; i < this->height*this->width; ++i){
        RGBA result{0,0,0,0};
        result.r = static_cast<uint8_t>(1.0 / this->maxPixValue * this->pixMap[i].r * second.pixMap[i].r);
        result.g = static_cast<uint8_t>(1.0 / this->maxPixValue * this->pixMap[i].g * second.pixMap[i].r);
        result.b = static_cast<uint8_t>(1.0 / this->maxPixValue * this->pixMap[i].b * second.pixMap[i].r);
        result.a = this->pixMap[i].a;

        result.r = std::max(std::min(static_cast<int>(result.r), 0xff), 0);
        result.g = std::max(std::min(static_cast<int>(result.g), 0xff), 0);
        result.b = std::max(std::min(static_cast<int>(result.b), 0xff), 0);
        result.a = std::max(std::min(static_cast<int>(result.a), 0xff), 0);
        newImage.pixMap[i] = result;
    }

    return newImage;
}

// Operacje logiczne na obrazach
wnImage wnImage::operator&(const wnImage& second) {
    if (this->width != second.width || this->height != second.height){
        throw std::runtime_error("Blad rozmiaru obrazu: Oba obrazy maja rozna wielkosc");
    }

    auto newImage = wnImage(this->height, this->width);

    for (int i=0; i < this->width*this->height; ++i){
        auto result = static_cast<uint8_t>(
                (this->pixMap[i].r && second.pixMap[i].r) ||
                (this->pixMap[i].g  && second.pixMap[i].g) ||
                (this->pixMap[i].b && second.pixMap[i].b) );

        newImage.pixMap[i].r = result;
        newImage.pixMap[i].g = result;
        newImage.pixMap[i].b = result;
        newImage.pixMap[i].a = 0xff;
    }

    return newImage;
}

wnImage wnImage::operator|(const wnImage& second) {
    if (this->width != second.width || this->height != second.height){
        throw std::runtime_error("Blad rozmiaru obrazu: Oba obrazy maja rozna wielkosc");
    }

    auto newImage = wnImage(this->height, this->width);

    for (int i=0; i < this->width*this->height; ++i){
        auto result = static_cast<uint8_t>(
                (this->pixMap[i].r || second.pixMap[i].r) ||
                (this->pixMap[i].g  || second.pixMap[i].g) ||
                (this->pixMap[i].b || second.pixMap[i].b) );

        newImage.pixMap[i].r = result;
        newImage.pixMap[i].g = result;
        newImage.pixMap[i].b = result;
        newImage.pixMap[i].a = 0xff;
    }

    return newImage;
}

wnImage wnImage::operator^(const wnImage& second) {
    if (this->width != second.width || this->height != second.height){
        throw std::runtime_error("Blad rozmiaru obrazu: Oba obrazy maja rozna wielkosc");
    }

    auto newImage = wnImage(this->height, this->width);

    for (int i=0; i < this->width*this->height; ++i){
        auto result = static_cast<uint8_t>(
                (!this->pixMap[i].r != !second.pixMap[i].r) &&
                (!this->pixMap[i].g  != !second.pixMap[i].g) &&
                (!this->pixMap[i].b != !second.pixMap[i].b) );

        newImage.pixMap[i].r = result;
        newImage.pixMap[i].g = result;
        newImage.pixMap[i].b = result;
        newImage.pixMap[i].a = 0xff;
    }

    return newImage;
}

wnImage wnImage::operator!(){
    auto newImage = wnImage(this->height, this->width);
    for (int i=0; i < this->width*this->height; ++i){
        auto result = static_cast<uint8_t>(
                !this->pixMap[i].r && !this->pixMap[i].g && !this->pixMap[i].b
                );
        newImage.pixMap[i].r = result;
        newImage.pixMap[i].g = result;
        newImage.pixMap[i].b = result;
        newImage.pixMap[i].a = 0xff;
    }

    return newImage;
}

// Transformacje macierzowe obrazu //

void wnImage::translation(int rowDelta, int colDelta) {
    std::vector<RGBA> newPixMap(this->height*this->width, {0, 0, 0, 0});
    for (int curRow=0; curRow < this->height; ++curRow){
        for (int curCol=0; curCol < this->width; ++curCol) {
            int newRow = curRow + rowDelta;
            int newCol = curCol + colDelta;

            if (newCol >= this->width || newRow >= this->height) continue;
            if (newCol < 0 || newRow <0) continue;

            newPixMap[newRow*this->width + newCol] = this->pixMap[curRow*this->width + curCol];
        }
    }
    this->pixMap = newPixMap;
}


// Implementacja obrotu dla różnych algorytmów resamplingu

void wnImage::rotationNN(double angle) {
    angle *= std::numbers::pi / 180.0;


    auto halfHeight = this->height/2;
    auto halfWidth = this->width/2;

    std::vector<RGBA> newPixMap(this->height*this->width, {0, 0, 0, 0});

    //Algorytm odtylcowy — NN
    for (int newRow=0; newRow < this->height; ++newRow){
        for (int newCol=0; newCol < this->width; ++newCol){
            int oldRow = static_cast<int>(cos(angle) * (newRow - halfHeight) + sin(angle) * (newCol - halfWidth));
            int oldCol = static_cast<int>(cos(angle) * (newCol - halfWidth) - sin(angle) * (newRow - halfHeight));

            oldRow += halfHeight;
            oldCol += halfWidth;

            if (oldCol >= this->width || oldRow >= this->height) continue;
            if (oldCol < 0 || oldRow < 0) continue;

            newPixMap[newRow*this->width + newCol] = this->pixMap[oldRow*this->width + oldCol];
        }
    }


    this->pixMap = newPixMap;

}

void wnImage::rotationBL(double angle) {
    angle *= std::numbers::pi / 180.0;

    auto halfHeight = this->height/2;
    auto halfWidth = this->width/2;

    std::vector<RGBA> newPixMap(this->height*this->width, {0, 0, 0, 0});

    for (int newRow=0; newRow < this->height; ++newRow){
        for (int newCol=0; newCol < this->width; ++newCol){
            double oldRow = cos(angle) * (newRow - halfHeight) + sin(angle) * (newCol - halfWidth);
            double oldCol = cos(angle) * (newCol - halfWidth) - sin(angle) * (newRow - halfHeight);

            oldRow += halfHeight;
            oldCol += halfWidth;

            if (oldCol >= this->width || oldRow >= this->height) continue;
            if (oldCol < 0 || oldRow < 0) continue;

            newPixMap[newRow*this->width + newCol] = bilinearPixel(newRow, newCol, oldRow, oldCol);
        }
    }


    this->pixMap = newPixMap;
}

void wnImage::rotation(double angle, int algoType) {
    switch (algoType){
        case resamplingAlgos::Bilinear:
            rotationBL(angle);
            break;
        case resamplingAlgos::NearestNeighbour:
            rotationNN(angle);
            break;
        default:
            throw std::runtime_error("Wybrano błędny algorytm.");
    }
}


// Implementacja algorytmów resamplingu rozciągania obrazu

void wnImage::shearingNN(double rowCoeff, double colCoeff) {
    std::vector<RGBA> newPixMap(this->height*this->width, {0,0,0,0});
    for (int newRow=0; newRow < this->height; ++newRow){
        for (int newCol=0; newCol < this->width; ++newCol){
            double tempCol = (1.0 / (1.0 - colCoeff * rowCoeff)) * (newCol - colCoeff * newRow);
            double tempRow = newRow - rowCoeff * tempCol;

            int oldRow = static_cast<int>(round(tempRow));
            int oldCol = static_cast<int>(round(tempCol));

            if (oldCol >= this->width || oldRow >= this->height) continue;
            if (oldCol < 0 || oldRow < 0) continue;

            newPixMap[newRow*this->width + newCol] = this->pixMap[oldRow*this->width + oldCol];
        }
    }

    this->pixMap = newPixMap;
}

void wnImage::shearingBL(double rowCoeff, double colCoeff) {
    std::vector<RGBA> newPixMap(this->height*this->width, {0,0,0,0});
    for (int newRow=0; newRow < this->height; ++newRow){
        for (int newCol=0; newCol < this->width; ++newCol) {
            double oldCol = (1.0 / (1.0 - colCoeff * rowCoeff)) * (newCol - colCoeff * newRow);
            double oldRow = newRow - rowCoeff * oldCol;

            if (oldCol >= this->width || oldRow >= this->height) continue;
            if (oldCol < 0 || oldRow < 0) continue;

            newPixMap[newRow*this->width + newCol] = bilinearPixel(newRow, newCol, oldRow, oldCol);
        }
    }

    this->pixMap = newPixMap;
}

void wnImage::shearing(double rowCoeff, double colCoeff, int algoType) {
    switch (algoType){
        case resamplingAlgos::Bilinear:
            shearingBL(rowCoeff, colCoeff);
            break;
        case resamplingAlgos::NearestNeighbour:
            shearingNN(rowCoeff, colCoeff);
            break;
        default:
            throw std::runtime_error("Wybrano błędny algorytm.");
    }
}

//Implementacja algorytmów resamplingu do zmiany wielkości obrazu
void wnImage::resizeNN(double rowCoeff, double colCoeff) {
    if (rowCoeff <= 0 || colCoeff <= 0){
        throw std::runtime_error("Współczynniki muszą byc wieksze od 0");
    }
    int newHeight = static_cast<int>(round(rowCoeff * this->height));
    int newWidth = static_cast<int>(round(colCoeff * this->width));

    std::vector<RGBA> newPixMap(newHeight*newWidth, {0,0,0,0});

    for (int newRow=0; newRow<newHeight; ++newRow){
        for (int newCol=0; newCol<newWidth; ++newCol){
            int oldRow = static_cast<int>(round(1.0/rowCoeff * newRow));
            int oldCol = static_cast<int>(round(1.0/colCoeff * newCol));

            if (oldCol >= this->width || oldRow >= this->height) continue;
            if (oldCol < 0 || oldRow < 0) continue;

            newPixMap[newRow*newWidth + newCol] = this->pixMap[oldRow*this->width + oldCol];
        }
    }

    this->height = newHeight;
    this->width = newWidth;
    this->pixMap = newPixMap;
}

void wnImage::resizeBL(double rowCoeff, double colCoeff) {
    if (rowCoeff <= 0 || colCoeff <= 0){
        throw std::runtime_error("Współczynniki muszą byc wieksze od 0");
    }
    int newHeight = static_cast<int>(round(rowCoeff * this->height));
    int newWidth = static_cast<int>(round(colCoeff * this->width));

    std::vector<RGBA> newPixMap(newHeight*newWidth, {0,0,0,0});

    for (int newRow=0; newRow<newHeight; ++newRow){
        for (int newCol=0; newCol<newWidth; ++newCol){

            double oldRow = 1.0/rowCoeff * newRow;
            double oldCol = 1.0/colCoeff * newCol;

            newPixMap[newRow*newWidth + newCol] = bilinearPixel(newRow, newCol, oldRow, oldCol);
            /*if (oldCol >= this->width || oldRow >= this->height) continue;
            if (oldCol < 0 || oldRow < 0) continue;

            newPixMap[newRow*this->width + newCol] = this->pixMap[oldRow*this->width + oldCol];*/
        }
    }

    this->height = newHeight;
    this->width = newWidth;
    this->pixMap = newPixMap;
}

void wnImage::resizeVal(int newHeight, int newWidth, int algoType) {
    double rowCoeff = 1.0 * newHeight / this->height;
    double colCoeff = 1.0 * newWidth / this->width;
    switch (algoType){
        case resamplingAlgos::Bilinear:
            resizeBL(rowCoeff, colCoeff);
            break;
        case resamplingAlgos::NearestNeighbour:
            resizeNN(rowCoeff, colCoeff);
            break;
        default:
            throw std::runtime_error("Wybrano błędny algorytm.");
    }
}

void wnImage::resizeCoeff(double coeff, int algoType) {
    switch (algoType){
        case resamplingAlgos::Bilinear:
            resizeBL(coeff, coeff);
            break;
        case resamplingAlgos::NearestNeighbour:
            resizeNN(coeff, coeff);
            break;
        default:
            throw std::runtime_error("Wybrano błędny algorytm.");
    }
}

// Obliczanie piksela bilinearnie
RGBA wnImage::bilinearPixel(int newRow, int newCol, double oldRow, double oldCol) {
    int oldRowT = static_cast<int>(floor(oldRow));
    oldRowT = std::min(std::max(oldRowT, 0), this->height - 1);
    int oldRowB = static_cast<int>(ceil(oldRow));
    oldRowB = std::min(std::max(oldRowB, 0), this->height - 1);
    int oldColL = static_cast<int>(floor(oldCol));
    oldColL = std::min(std::max(oldColL, 0), this->width - 1);
    int oldColR = static_cast<int>(ceil(oldCol));
    oldColR = std::min(std::max(oldColR, 0), this->width - 1);

    double rowWage = oldRow - oldRowT;
    double colWage = oldCol - oldColL;

    RGBA oldLTPixel = this->pixMap[oldRowT*this->width + oldColL];
    RGBA oldLBPixel = this->pixMap[oldRowB*this->width + oldColL];
    RGBA oldRTPixel = this->pixMap[oldRowT*this->width + oldColR];
    RGBA oldRBPixel = this->pixMap[oldRowB*this->width + oldColR];

    double tempR = 0.0, tempG = 0.0, tempB = 0.0, tempA = 0.0;

    tempR = (1.0-rowWage)*(1.0-colWage)*oldLTPixel.r;
    tempR += (1.0-rowWage)*colWage*oldRTPixel.r;
    tempR += (1.0-colWage)*rowWage*oldLBPixel.r;
    tempR += rowWage*colWage*oldRBPixel.r;

    tempR = std::max(0.0, (std::min(tempR, 255.0)));

    tempG = (1.0-rowWage)*(1.0-colWage)*oldLTPixel.g;
    tempG += (1.0-rowWage)*colWage*oldRTPixel.g;
    tempG += (1.0-colWage)*rowWage*oldLBPixel.g;
    tempG += rowWage*colWage*oldRBPixel.g;

    tempG = std::max(0.0, (std::min(tempG, 255.0)));

    tempB = (1.0-rowWage)*(1.0-colWage)*oldLTPixel.b;
    tempB += (1.0-rowWage)*colWage*oldRTPixel.b;
    tempB += (1.0-colWage)*rowWage*oldLBPixel.b;
    tempB += rowWage*colWage*oldRBPixel.b;

    tempB = std::max(0.0, (std::min(tempB, 255.0)));

    tempA = (1.0-rowWage)*(1.0-colWage)*oldLTPixel.a;
    tempA += (1.0-rowWage)*colWage*oldRTPixel.a;
    tempA += (1.0-colWage)*rowWage*oldLBPixel.a;
    tempA += rowWage*colWage*oldRBPixel.a;

    tempA = std::max(0.0, (std::min(tempA, 255.0)));

    return {static_cast<uint8_t>(tempR),static_cast<uint8_t>(tempG),
     static_cast<uint8_t>(tempB),static_cast<uint8_t>(tempA)};
}

//Metody graficzne
void wnImage::putPixel(int rowNum, int colNum, const RGBA& rgba) {
    if (rowNum > this->getHeight() || colNum > this->getWidth() || rowNum < 0 || colNum < 0){
        throw std::runtime_error("Blad wspolrzednych: piksel poza obrazem");
    }

    this->pixMap[colNum + rowNum*this->getWidth()] = rgba;
}

void wnImage::drawRectange(int x1, int y1, int x2, int y2, const RGBA &rgba) {
    if (x1 > this->getHeight() || x1 < 0 || x2 > this->getHeight() || x2 < 0){
        throw std::runtime_error("Blad wspolrzednej X: poza zakresem");
    }
    if (y1 > this->getWidth() || y1 < 0 || y2 > this->getWidth() || y2 < 0){
        throw std::runtime_error("Blad wspolrzednej Y: poza zakresem");
    }
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);

    for (int row = x1; row <= x2; ++row){
        for (int col = y1; col <= y2; ++col){
            putPixel(row, col, rgba);
        }
    }
}