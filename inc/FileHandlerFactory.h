//
// Created by wojnad on 13.04.2025.
//

#ifndef FILTROBRAZU_FILEHANDLERFACTORY_H
#define FILTROBRAZU_FILEHANDLERFACTORY_H

#include "FileHandler.h"
#include "FileHandlerPAM.h"
#include "FileHandlerTGA.h"
#include <memory>

/**
 * \class FileHandlerFactory
 *
 * Klasa obsługująca tworzenie obiektów poszczególnych klas dziedziczących po FileHandler
 * w zależności od rozszerzenia pliku
 * */
class FileHandlerFactory {
public:
    /**
     * Metoda statyczna zwracająca wskaźnik unikalny do odpowiedniego typu FileHandlera.
     * */
    static std::unique_ptr<FileHandler> getFilehander(int fileType);
};

#endif //FILTROBRAZU_FILEHANDLERFACTORY_H
