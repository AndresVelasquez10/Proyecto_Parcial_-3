#include "Ataque.hpp"

// Cuando creo un ataque, simplemente guardo el nombre y el daño que le paso
Ataque::Ataque(const std::string& nombre, int danio) 
    : nombre(nombre), danio(danio) {}