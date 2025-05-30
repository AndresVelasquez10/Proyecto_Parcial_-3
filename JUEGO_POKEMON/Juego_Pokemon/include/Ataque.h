#ifndef ATAQUE_H
#define ATAQUE_H

#include <string>

struct Ataque {
    std::string nombre;
    int danio;

    // Constructor para inicializar fácilmente
    Ataque(const std::string& nombre, int danio) 
        : nombre(nombre), danio(danio) {}
};

#endif