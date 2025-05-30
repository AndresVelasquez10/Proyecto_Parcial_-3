#ifndef POKEMON_HPP
#define POKEMON_HPP

#include <string>
#include <vector>
#include "Ataque.hpp"

class Pokemon {
private:
    std::string nombre;
    int vida_actual;
    int vida_maxima;
    std::vector<Ataque> ataques;

public:
    Pokemon(const std::string& nombre = "Pokemon", int vida_max = 100);
    
    const std::string& getNombre() const { return nombre; }
    int getVidaActual() const { return vida_actual; }
    int getVidaMaxima() const { return vida_maxima; }
    const std::vector<Ataque>& getAtaques() const { return ataques; }
    
    void recibirDanio(int cantidad);
    bool estaDerrotado() const;
};

#endif