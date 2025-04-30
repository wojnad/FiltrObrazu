//
// Created by wojnad on 10.05.2024.
//

#ifndef WNGRAPHICS_FILEHANDLER_H
#define WNGRAPHICS_FILEHANDLER_H

#include <bits/stdc++.h>
#include "wnImage.h"

class wnImage;

/**
 * \class FileHandler
 *
 * Klasa obsługująca wczytywanie i zapisywanie poszczególnych plików graficznych
 * */
class FileHandler{
public:
    virtual wnImage readFile(const std::string& srcPath) = 0;
    virtual void saveFile(const std::string& dstPath, const wnImage& image) = 0;

};

#endif //WNGRAPHICS_FILEHANDLER_H
