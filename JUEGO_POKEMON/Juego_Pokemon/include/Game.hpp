// Game.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Combate.hpp"
#include "Pokemon.hpp"

class Game {
public:
    Game();
    void run();

private:
    void handleEvents();
    void update(float deltaTime);
    void render();
    void startPokemonBattle();
    void drawMenu();
    void handleMenuInput();

    // Estados del juego
    enum class GameState {
        MainMenu,
        Playing,
        Battle
    };

    GameState currentState = GameState::MainMenu;

    // Ventana
    sf::RenderWindow window;

    // Fondo
    sf::Texture backgroundTexture;
    sf::Sprite background;

    // Jugador
    Player player;

    // Pokémon salvaje
    Pokemon salvaje;

    // Reloj
    sf::Clock clock;

    // Menú principal
    sf::Font menuFont;
    std::vector<sf::Text> menuItems;
    int selectedMenuItem = 0;
    
    // Botones del menú
    sf::RectangleShape startButton;
    sf::RectangleShape exitButton;
    sf::Text startText;
    sf::Text exitText;
    
    // Texto de título
    sf::Text titleText;

    // Sonidos
    sf::SoundBuffer menuSelectBuffer;
    sf::Sound menuSelectSound;
};