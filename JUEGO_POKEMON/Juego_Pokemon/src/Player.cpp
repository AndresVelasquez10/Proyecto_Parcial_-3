#include "Player.hpp"
#include <filesystem>
#include <iostream>

// Cuando creo el jugador, le agrego dos Pokémon por defecto
Player::Player() {
    std::cout << "Constructor de Player llamado\n";
    pokemons.push_back(Pokemon("Pikachu", 100));
    pokemons.push_back(Pokemon("Charmander", 120));
}

void Player::loadTexture(const std::string& path) {
    std::filesystem::path abs_path = std::filesystem::absolute(path);
    std::cout << "Intentando cargar textura desde:\n" << abs_path << "\n";
    if(!std::filesystem::exists(abs_path)) {
        std::cerr << "ERROR: El archivo no existe!\n";
        std::cerr << "Por favor, verifica la ruta y el nombre del archivo\n";
        exit(1);
    }
    if(!texture.loadFromFile(abs_path.string())) {
        std::cerr << "ERROR: Fallo al cargar la textura (aunque el archivo existe)\n";
        std::cerr << "Posibles causas:\n";
        std::cerr << "1. Formato de imagen no soportado\n";
        std::cerr << "2. Archivo corrupto\n";
        std::cerr << "3. Permisos insuficientes\n";
        exit(1);
    }
    sprite.setTexture(texture);
    std::cout << "¡Textura cargada con éxito!\n";

    sf::Vector2u size = texture.getSize();
    std::cout << "Tamaño de textura cargada: " 
              << size.x << "x" << size.y << " píxeles\n";
    if(size.x > 200 || size.y > 200) {
        float scale = 200.0f / std::max(size.x, size.y);
        sprite.setScale(scale, scale);
        std::cout << "Escalado a: " << scale * 100 << "%\n";
    }
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2, bounds.height / 2);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite); 
}

void Player::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Player::setScale(float factor) {
    sprite.setScale(factor, factor);
}

void Player::move(float dx, float dy, float deltaTime) {
    sprite.move(dx * speed * deltaTime, dy * speed * deltaTime);
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

sf::FloatRect Player::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

// Siempre devuelvo el Pokémon que está activo en el equipo
Pokemon& Player::getPokemonActual() {
    return pokemons[pokemonActual];
}

// Cambio al siguiente Pokémon del equipo
void Player::cambiarPokemon() {
    pokemonActual = (pokemonActual + 1) % pokemons.size();
    std::cout << "¡Cambiaste a " << pokemons[pokemonActual].getNombre() << "!\n";
}