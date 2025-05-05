//
// Created by wojnad on 13.04.2025.
//

#include "FileHandlerFactory.h"

std::unique_ptr<FileHandler> FileHandlerFactory::getFilehander(int fileType) {
    switch (fileType) {
        case fileTypes::TGA:
            return std::unique_ptr<FileHandler>(new FileHandlerTGA());
        case fileTypes::PBM:
            return std::unique_ptr<FileHandler>(new FileHandlerPBM(true));
        case fileTypes::PGM:
            return std::unique_ptr<FileHandler>(new FileHandlerPGM(true));
        case fileTypes::PPM:
            return std::unique_ptr<FileHandler>(new FileHandlerPPM(true));
        default:
            return nullptr;
    }
}