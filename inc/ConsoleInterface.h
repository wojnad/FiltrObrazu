//
// Created by wojnad on 14.04.2025.
//

#ifndef FILTROBRAZU_CONSOLEINTERFACE_H
#define FILTROBRAZU_CONSOLEINTERFACE_H

#include "wnImage.h"

class ConsoleInterface{
    std::unique_ptr<wnFilter> usedFilter = nullptr;
    wnImage& editedImage;
public:
    ConsoleInterface(wnImage& editedImage);
    void cliMenu();
    void setUsedFilter(std::unique_ptr<wnFilter>& filter);

    void applyInversion();

    void applyConversionToGrayscale();

    void applyGammaCorection();

    void applyFilter();

    void applyRotation();

    void applyResizing();
};

#endif //FILTROBRAZU_CONSOLEINTERFACE_H
