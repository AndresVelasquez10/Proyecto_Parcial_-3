#include "Game.hpp"
#include <iostream>

Game::Game() : 
    window(sf::VideoMode(800, 600), "Pokemon Game"),
    salvaje("Bulbasaur", 110),
    currentState(GameState::MainMenu)  // Inicializar estado como menú
{
    if (!backgroundTexture.loadFromFile("assets/textures/Fondo.png")) {
        std::cerr << "Error cargando fondo!\n";
        // Crear fondo de emergencia
        backgroundTexture.create(800, 600);
        sf::Image img;
        img.create(800, 600, sf::Color(0, 100, 0)); // Verde oscuro
        backgroundTexture.update(img);
    }
    background.setTexture(backgroundTexture);
    
    // Cargar textura del jugador (descomentado)
    // player.loadTexture("assets/textures/player_new.png");
    // player.setPosition(400, 300);
    // player.setScale(0.8f);

    // Inicializar menú
    if (!menuFont.loadFromFile("assets/fonts/pokemon.ttf")) {
        std::cerr << "Error cargando fuente para menú\n";
        // Intentar cargar una fuente por defecto
        if (!menuFont.loadFromFile("/usr/share/fonts/truetype/arial.ttf")) {
            std::cerr << "No se pudo cargar fuente alternativa\n";
        }
    }

    // Configurar título
    titleText.setFont(menuFont);
    titleText.setString("POKEMON GAME");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setOutlineColor(sf::Color::Blue);
    titleText.setOutlineThickness(2);
    titleText.setPosition(200, 100);

    // Configurar botones
    startButton.setSize(sf::Vector2f(400, 100));
    startButton.setFillColor(sf::Color(30, 150, 30, 255));  // Color verde (sin transparencia)
    startButton.setOutlineThickness(5);
    startButton.setOutlineColor(sf::Color::Black);
    startButton.setPosition(200, 250);
    
    exitButton.setSize(sf::Vector2f(400, 100));
    exitButton.setFillColor(sf::Color(150, 30, 30, 255));   // Color rojo (sin transparencia)
    exitButton.setOutlineThickness(5);
    exitButton.setOutlineColor(sf::Color::Black);
    exitButton.setPosition(200, 380);
    
    // Configurar textos de botones - CORREGIDO
    startText.setFont(menuFont);
    startText.setString("INICIAR COMBATE");
    startText.setCharacterSize(40);
    startText.setFillColor(sf::Color::White);
    startText.setStyle(sf::Text::Bold);
    
    // Centrar texto en botones
    sf::FloatRect textBounds = startText.getLocalBounds();
    startText.setOrigin(textBounds.left + textBounds.width/2.0f,
                       textBounds.top + textBounds.height/2.0f);
    startText.setPosition(startButton.getPosition().x + startButton.getSize().x/2,
                         startButton.getPosition().y + startButton.getSize().y/2);
    
    exitText.setFont(menuFont);
    exitText.setString("SALIR DEL JUEGO");
    exitText.setCharacterSize(40);
    exitText.setFillColor(sf::Color::White);
    exitText.setStyle(sf::Text::Bold);
    
    textBounds = exitText.getLocalBounds();
    exitText.setOrigin(textBounds.left + textBounds.width/2.0f,
                      textBounds.top + textBounds.height/2.0f);
    exitText.setPosition(exitButton.getPosition().x + exitButton.getSize().x/2,
                        exitButton.getPosition().y + exitButton.getSize().y/2);

    // Sonido de menú
    if (!menuSelectBuffer.loadFromFile("assets/sounds/menu_select.mp3")) {
        std::cerr << "Error cargando sonido de menú\n";
    } else {
        menuSelectSound.setBuffer(menuSelectBuffer);
    }

    // Configurar texto de instrucción
    // instructionText.setFont(menuFont);
    // instructionText.setString("Presiona ESPACIO para iniciar");
    // instructionText.setCharacterSize(40);
    // instructionText.setFillColor(sf::Color::White);

    // sf::FloatRect instructionBounds = instructionText.getLocalBounds();
    // instructionText.setOrigin(instructionBounds.left + instructionBounds.width/2.0f,
    //                          instructionBounds.top + instructionBounds.height/2.0f);
    // instructionText.setPosition(window.getSize().x/2, 350);
}

void Game::drawMenu() {
    window.clear();
    window.draw(background);
    window.draw(titleText);
    window.draw(startButton);
    window.draw(exitButton);
    window.draw(startText);
    window.draw(exitText);
    window.display();
}

void Game::handleMenuInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Manejar movimiento del mouse - CORRECTO
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            
            // Resaltar botón bajo el mouse
            if (startButton.getGlobalBounds().contains(mousePos)) {
                startButton.setOutlineColor(sf::Color::Yellow);
                exitButton.setOutlineColor(sf::Color::Black);
            } else if (exitButton.getGlobalBounds().contains(mousePos)) {
                exitButton.setOutlineColor(sf::Color::Yellow);
                startButton.setOutlineColor(sf::Color::Black);
            } else {
                startButton.setOutlineColor(sf::Color::Black);
                exitButton.setOutlineColor(sf::Color::Black);
            }
        }

        // Manejar clics del mouse - CORREGIDO (Vector2i en lugar de Vector2f)
        if (event.type == sf::Event::MouseButtonPressed && 
            event.mouseButton.button == sf::Mouse::Left) {
            
            // Usar Vector2i para las coordenadas del mouse
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(
                event.mouseButton.x, 
                event.mouseButton.y
            ));
            
            std::cout << "Mouse clicked at: " << mousePos.x << ", " << mousePos.y << "\n";
            
            if (startButton.getGlobalBounds().contains(mousePos)) {
                std::cout << "Start button clicked!\n";
                if (menuSelectSound.getStatus() == sf::Sound::Stopped) {
                    menuSelectSound.play();
                }
                currentState = GameState::Playing;
            } else if (exitButton.getGlobalBounds().contains(mousePos)) {
                std::cout << "Exit button clicked!\n";
                window.close();
            }
        }

        // Manejar teclado
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                if (menuSelectSound.getStatus() == sf::Sound::Stopped) {
                    menuSelectSound.play();
                }
                currentState = GameState::Playing;
            }
            else if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        // Manejar eventos específicos del juego
        if (currentState == GameState::Playing) {
            // Aquí puedes añadir eventos específicos del juego si son necesarios
        }
    }
}

void Game::startPokemonBattle() {
    // Curar Pokémon antes del combate
    player.getPokemonActual().recibirDanio(-player.getPokemonActual().getVidaMaxima());
    salvaje.recibirDanio(-salvaje.getVidaMaxima());
    
    // Crear instancia de Combate
    Combate combate(window, player.getPokemonActual(), salvaje);
    
    // Iniciar combate
    bool victoria = combate.iniciarCombate();
    
    // Manejar resultado
    if (victoria) {
        std::cout << "¡Ganaste el combate!\n";
    } else {
        std::cout << "Perdiste el combate...\n";
    }
}

void Game::update(float deltaTime) {
    if (currentState == GameState::Playing) {
        // Manejar movimiento
        float dx = 0, dy = 0;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) dx = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dx = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) dy = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) dy = 1;
        
        player.move(dx, dy, deltaTime);
        
        // Cambiar Pokémon
        static bool cPressed = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && !cPressed) {
            player.cambiarPokemon();
            cPressed = true;
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
            cPressed = false;
        }
        
        // Iniciar combate - SOLO si estamos en estado Playing
        static bool spacePressed = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !spacePressed) {
            spacePressed = true;
            startPokemonBattle();
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            spacePressed = false;
        }
    }
}

void Game::render() {
    window.clear();
    
    if (currentState == GameState::MainMenu) {
        drawMenu();
    } 
    else if (currentState == GameState::Playing) {
        window.draw(background);
        player.draw(window);
    }
    
    window.display();
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
        // Manejar eventos según el estado actual
        if (currentState == GameState::MainMenu) {
            handleMenuInput();
        } 
        else {
            handleEvents();
        }
        
        update(deltaTime);
        render();
    }
}