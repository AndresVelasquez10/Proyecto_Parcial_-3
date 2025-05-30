#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Pokemon.hpp"  // Asegúrate de incluir esto

class Player {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed = 200.0f;
    
    // Sistema de Pokémon
    std::vector<Pokemon> pokemons;  // Equipo de Pokémon
    int pokemonActual = 0;           // Índice del Pokémon activo

public:
    Player();
    void loadTexture(const std::string& path);
    void draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
    void setScale(float factor);
    void move(float dx, float dy, float deltaTime);
    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;
    
    // Métodos para Pokémon - CORREGIDOS
    void cambiarPokemon();
    Pokemon& getPokemonActual();  // Devuelve referencia
};

#endif