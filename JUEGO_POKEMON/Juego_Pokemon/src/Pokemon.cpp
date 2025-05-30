#include "Pokemon.hpp"

Pokemon::Pokemon(const std::string& nombre, int vida_max) 
    : nombre(nombre), vida_actual(vida_max), vida_maxima(vida_max) {
    // Siempre le doy dos ataques básicos a cada Pokémon al crearlo
    ataques.push_back(Ataque("Trueno", 20));
    ataques.push_back(Ataque("Cabezazo", 30));
}

void Pokemon::recibirDanio(int cantidad) {
    vida_actual -= cantidad;
    if (vida_actual < 0) vida_actual = 0;
}

bool Pokemon::estaDerrotado() const {
    return vida_actual <= 0;
}