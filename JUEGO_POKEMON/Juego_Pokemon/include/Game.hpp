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

    // Enum para saber en qué estado está el juego
    enum class GameState {
        MainMenu,
        Playing,
        Battle
    };

    sf::RenderWindow window;

    sf::Texture backgroundTexture;
    sf::Sprite background;

    Player player;

    GameState currentState = GameState::MainMenu;

    Pokemon salvaje;

    sf::Clock clock;

    // Todo lo relacionado al menú principal lo tengo aquí
    sf::Font menuFont;
    std::vector<sf::Text> menuItems;
    int selectedMenuItem = 0;
    sf::Text instructionText;
    
    sf::RectangleShape startButton;
    sf::RectangleShape exitButton;
    sf::Text startText;
    sf::Text exitText;
    
    sf::Text titleText;

    // Sonido para cuando selecciono algo en el menú
    sf::SoundBuffer menuSelectBuffer;
    sf::Sound menuSelectSound;

    // Para mostrar el resultado del combate
    sf::Text resultText;
    float resultDisplayTime = 0.0f;
    bool battleResult = false;
};