#ifndef ATAQUE_HPP
#define ATAQUE_HPP

#include <string>

// Esta estructura representa un ataque, con nombre y daño
struct Ataque {
    std::string nombre;
    int danio;

    Ataque(const std::string& nombre = "", int danio = 0);
};

#endif