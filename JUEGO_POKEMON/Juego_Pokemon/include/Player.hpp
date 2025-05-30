#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Pokemon.hpp"

class Player {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed = 200.0f;
    
    // Acá guardo el equipo de Pokémon del jugador
    std::vector<Pokemon> pokemons;
    int pokemonActual = 0; // Siempre llevo el índice del Pokémon activo

public:
    Player();
    void loadTexture(const std::string& path);
    void draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
    void setScale(float factor);
    void move(float dx, float dy, float deltaTime);
    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;
    
    // Métodos para manejar el Pokémon activo
    void cambiarPokemon();
    Pokemon& getPokemonActual();
};

#endif