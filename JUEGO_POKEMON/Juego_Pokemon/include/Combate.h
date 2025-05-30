#ifndef COMBATE_H
#define COMBATE_H

#include "Pokemon.h"

class Combate {
private:
    Pokemon* jugador;
    Pokemon* enemigo;

    void mostrarEstado() const;
    void turnoEnemigo();

public:
    void iniciarCombate(Pokemon& jugador, Pokemon& enemigo);
};

#endif