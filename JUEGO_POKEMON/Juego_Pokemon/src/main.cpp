// #include <iostream>
// #include "Pokemon.h"
// #include "Combate.h"

// // Función para crear Pokémon predefinidos
// Pokemon crearCharmander() {
//     std::vector<Ataque> ataques = {
//         Ataque("Lanzallamas", 50),
//         Ataque("Arañazo", 30)
//     };
//     return Pokemon("Charmander", 120, ataques);
// }

// Pokemon crearSquirtle() {
//     std::vector<Ataque> ataques = {
//         Ataque("Pistola Agua", 40),
//         Ataque("Placaje", 20)
//     };
//     return Pokemon("Squirtle", 150, ataques);
// }

// int main() {
//     Pokemon jugador = crearCharmander();
//     Pokemon enemigo = crearSquirtle();

//     Combate combate;
//     combate.iniciarCombate(jugador, enemigo);

//     return 0;
// }









#include "Game.hpp"

int main() {
    Game game;
    game.run();
    return 0;
}


// #include <SFML/Graphics.hpp>
// #include <iostream>
// #include <filesystem>

// int main() {
//     sf::RenderWindow window(sf::VideoMode(800, 600), "Test Textura");
//     sf::Texture texture;
//     std::string path = "assets/textures/player_new.png";

//     if (!texture.loadFromFile(path)) {
//         std::cerr << "Error en ruta: " << std::filesystem::absolute(path) << "\n";
//         return 1;
//     }

//     sf::Sprite sprite(texture);

//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed) window.close();
//         }

//         window.clear();
//         window.draw(sprite);
//         window.display();
//     }

//     return 0;
// }