#ifndef ATAQUE_HPP
#define ATAQUE_HPP

#include <string>

struct Ataque {
    std::string nombre;
    int danio;

    Ataque(const std::string& nombre = "", int danio = 0);
};

#endif