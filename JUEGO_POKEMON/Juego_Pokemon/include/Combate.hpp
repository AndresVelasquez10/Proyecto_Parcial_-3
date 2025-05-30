#ifndef COMBATE_HPP
#define COMBATE_HPP

#include "Pokemon.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

enum class EstadoCombate {
    SeleccionAtaque,
    EjecutandoAtaqueJugador,
    EjecutandoAtaqueOponente,
    Resultado
};

class AnimacionAtaque;

// Esta clase es la que se encarga de todo el combate entre dos Pokémon
class Combate {
private:
    sf::RenderWindow& ventana;
    Pokemon& jugador;
    Pokemon& oponente;
    sf::Font fuente;
    sf::Texture texturaFondo;
    sf::Sprite spriteFondo;
    
    EstadoCombate estadoActual;
    int opcionSeleccionada;
    float tiempoAnimacion;
    
    // Acá guardo todos los elementos de la interfaz de combate
    sf::Text textoJugador;
    sf::Text textoOponente;
    std::vector<sf::Text> opcionesAtaque;
    sf::RectangleShape panelInferior;

    // Sprites de los Pokémon en combate
    sf::Texture texturaPokemonJugador;
    sf::Sprite spritePokemonJugador;
    sf::Texture texturaPokemonOponente;
    sf::Sprite spritePokemonOponente;

    // Sonidos para selección y ataque
    sf::SoundBuffer bufferSeleccion;
    sf::Sound sonidoSeleccion;
    sf::SoundBuffer bufferAtaque;
    sf::Sound sonidoAtaque;

    // Animación de ataque actual
    std::unique_ptr<AnimacionAtaque> animacionActual;
    
    void dibujarBarraVida(float x, float y, float ancho, float alto, float porcentaje);
    void dibujarInterfaz();
    void procesarEventos();
    void actualizar(float deltaTime);
    
public:
    Combate(sf::RenderWindow& ventana, Pokemon& jugador, Pokemon& oponente);
    bool iniciarCombate();
};

// Esta clase la uso para animar los ataques visualmente
class AnimacionAtaque {
public:
    AnimacionAtaque(sf::Sprite& sprite, sf::Vector2f direccion, float duracion = 0.5f);
    bool actualizar(float deltaTime);
    
private:
    sf::Sprite& sprite;
    sf::Vector2f direccion;
    float duracion;
    float tiempo;
    sf::Vector2f posicionInicial;
};

#endif