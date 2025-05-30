#include "Player.hpp"
#include <filesystem>
#include <iostream>


// Implementación del constructor
Player::Player() {
    // Constructor vacío o con inicializaciones básicas
    std::cout << "Constructor de Player llamado\n";
    pokemons.push_back(Pokemon("Pikachu", 100));
    pokemons.push_back(Pokemon("Charmander", 120));
}

void Player::loadTexture(const std::string& path) {
    // Convertir a ruta absoluta
    std::filesystem::path abs_path = std::filesystem::absolute(path);
    
    std::cout << "Intentando cargar textura desde:\n" << abs_path << "\n";
    
    // Verificar si el archivo existe
    if(!std::filesystem::exists(abs_path)) {
        std::cerr << "ERROR: El archivo no existe!\n";
        std::cerr << "Por favor, verifica la ruta y el nombre del archivo\n";
        exit(1);
    }
    
    // Intentar cargar la textura
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
    
    // Escala automática si es muy grande
    if(size.x > 200 || size.y > 200) {
        float scale = 200.0f / std::max(size.x, size.y);
        sprite.setScale(scale, scale);
        std::cout << "Escalado a: " << scale * 100 << "%\n";
    }

    // Establecer origen en el centro
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2, bounds.height / 2);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite); 
    //std::cout << "Dibujando sprite...\n";
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

Pokemon& Player::getPokemonActual() {
    return pokemons[pokemonActual];  // Devuelve referencia
}

void Player::cambiarPokemon() {
    pokemonActual = (pokemonActual + 1) % pokemons.size();
    std::cout << "¡Cambiaste a " << pokemons[pokemonActual].getNombre() << "!\n";
}