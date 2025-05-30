#ifndef POKEMON_H
#define POKEMON_H

#include <string>
#include <vector>
#include "Ataque.h"

class Pokemon {
private:
    std::string nombre;
    int vida_actual;
    int vida_maxima;
    std::vector<Ataque> ataques;

public:
    // Constructor
    Pokemon(const std::string& nombre, int vida_max, const std::vector<Ataque>& ataques);

    // Métodos
    void recibirDanio(int cantidad);
    bool estaDerrotado() const;

    // Getters
    std::string getNombre() const;
    int getVidaActual() const;
    const std::vector<Ataque>& getAtaques() const;
};

#endif