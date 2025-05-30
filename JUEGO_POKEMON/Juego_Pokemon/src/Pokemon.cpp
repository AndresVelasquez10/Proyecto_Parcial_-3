#include "Pokemon.hpp"

Pokemon::Pokemon(const std::string& nombre, int vida_max) 
    : nombre(nombre), vida_actual(vida_max), vida_maxima(vida_max) {
    // Ataques básicos
    ataques.push_back(Ataque("Golpeo", 20));
    ataques.push_back(Ataque("Látigo", 30));
}

// std::string Pokemon::getNombre() const {
//     return nombre;
// }

// int Pokemon::getVidaActual() const {
//     return vida_actual;
// }

// int Pokemon::getVidaMaxima() const {
//     return vida_maxima;
// }

// const std::vector<Ataque>& Pokemon::getAtaques() const {
//     return ataques;
// }

void Pokemon::recibirDanio(int cantidad) {
    vida_actual -= cantidad;
    if (vida_actual < 0) vida_actual = 0;
}

bool Pokemon::estaDerrotado() const {
    return vida_actual <= 0;
}