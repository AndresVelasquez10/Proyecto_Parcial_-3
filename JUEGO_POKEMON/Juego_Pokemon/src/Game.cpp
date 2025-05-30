#include "Game.hpp"
#include <iostream>

Game::Game() : 
    window(sf::VideoMode(800, 600), "Pokemon Game"),
    currentState(GameState::MainMenu),
    salvaje("Bulbasaur", 110)
{
    // Cargo el fondo, si falla genero uno verde simple
    if (!backgroundTexture.loadFromFile("assets/textures/Fondo.png")) {
        std::cerr << "Error cargando fondo!\n";
        backgroundTexture.create(800, 600);
        sf::Image img;
        img.create(800, 600, sf::Color(0, 100, 0));
        backgroundTexture.update(img);
    }
    background.setTexture(backgroundTexture);
    
    // Escalo el fondo para que siempre ocupe toda la ventana
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();
    if (textureSize.x > 0 && textureSize.y > 0) {
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        background.setScale(scaleX, scaleY);
    }
    
    if (!menuFont.loadFromFile("assets/fonts/pokemon.ttf")) {
        std::cerr << "Error cargando fuente para menú\n";
        if (!menuFont.loadFromFile("/usr/share/fonts/truetype/arial.ttf")) {
            std::cerr << "No se pudo cargar fuente alternativa\n";
        }
    }

    // Configuro el título principal del menú
    titleText.setFont(menuFont);
    titleText.setString("POKEMON GAME");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setOutlineColor(sf::Color::Blue);
    titleText.setOutlineThickness(2);
    
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.left + titleBounds.width/2.0f,
                       titleBounds.top + titleBounds.height/2.0f);
    titleText.setPosition(window.getSize().x/2, 150);

    startButton.setPosition(window.getSize().x * 0.25f, window.getSize().y * 0.4f);
    exitButton.setPosition(window.getSize().x * 0.25f, window.getSize().y * 0.6f);

    startText.setPosition(startButton.getPosition().x + startButton.getSize().x/2,
                        startButton.getPosition().y + startButton.getSize().y/2);
    exitText.setPosition(exitButton.getPosition().x + exitButton.getSize().x/2,
                        exitButton.getPosition().y + exitButton.getSize().y/2);

    startButton.setSize(sf::Vector2f(400, 100));
    startButton.setFillColor(sf::Color(30, 150, 30, 255));
    startButton.setOutlineThickness(5);
    startButton.setOutlineColor(sf::Color::Black);
    startButton.setPosition(window.getSize().x * 0.25f, window.getSize().y * 0.4f);
    
    exitButton.setSize(sf::Vector2f(400, 100));
    exitButton.setFillColor(sf::Color(150, 30, 30, 255));
    exitButton.setOutlineThickness(5);
    exitButton.setOutlineColor(sf::Color::Black);
    exitButton.setPosition(window.getSize().x * 0.25f, window.getSize().y * 0.6f);
    
    // Configuro los textos de los botones del menú
    startText.setFont(menuFont);
    startText.setString("INICIAR COMBATE");
    startText.setCharacterSize(40);
    startText.setFillColor(sf::Color::White);
    startText.setStyle(sf::Text::Bold);
    startText.setOutlineColor(sf::Color::Black);
    startText.setOutlineThickness(1);
    
    sf::FloatRect textBounds = startText.getLocalBounds();
    startText.setOrigin(textBounds.width/2, textBounds.height/2);
    startText.setPosition(startButton.getPosition().x + startButton.getSize().x/2,
                         startButton.getPosition().y + startButton.getSize().y/2);
    
    exitText.setFont(menuFont);
    exitText.setString("SALIR DEL JUEGO");
    exitText.setCharacterSize(40);
    exitText.setFillColor(sf::Color::White);
    exitText.setStyle(sf::Text::Bold);
    exitText.setOutlineColor(sf::Color::Black);
    exitText.setOutlineThickness(1);
    
    textBounds = exitText.getLocalBounds();
    exitText.setOrigin(textBounds.width/2, textBounds.height/2);
    exitText.setPosition(exitButton.getPosition().x + exitButton.getSize().x/2,
                        exitButton.getPosition().y + exitButton.getSize().y/2);

    // Texto de instrucción para el modo Playing
    instructionText.setFont(menuFont);
    instructionText.setString("Presiona ESPACIO para iniciar");
    instructionText.setCharacterSize(40);
    instructionText.setFillColor(sf::Color::Yellow);
    instructionText.setOutlineColor(sf::Color::Black);
    instructionText.setOutlineThickness(2);
    
    sf::FloatRect instructionBounds = instructionText.getLocalBounds();
    instructionText.setOrigin(instructionBounds.width/2, instructionBounds.height/2);
    instructionText.setPosition(window.getSize().x/2, window.getSize().y * 0.7f);

    // Texto para mostrar el resultado del combate
    resultText.setFont(menuFont);
    resultText.setCharacterSize(80);
    resultText.setOutlineColor(sf::Color::Black);
    resultText.setOutlineThickness(4);
    resultText.setPosition(window.getSize().x/2, window.getSize().y/2);
    
    sf::FloatRect resultBounds = resultText.getLocalBounds();
    resultText.setOrigin(resultBounds.width/2, resultBounds.height/2);

    // Cargo el sprite del jugador
    player.loadTexture("assets/textures/player_new.png");
    player.setPosition(400, 300);
    player.setScale(0.8f);
}

// Dibujo el menú principal
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

// Aquí manejo toda la interacción del usuario en el menú principal
void Game::handleMenuInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            
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

        if (event.type == sf::Event::MouseButtonPressed && 
            event.mouseButton.button == sf::Mouse::Left) {
            
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(
                event.mouseButton.x, 
                event.mouseButton.y
            ));
            
            if (startButton.getGlobalBounds().contains(mousePos)) {
                if (menuSelectSound.getStatus() == sf::Sound::Stopped) {
                    menuSelectSound.play();
                }
                currentState = GameState::Playing;
            } else if (exitButton.getGlobalBounds().contains(mousePos)) {
                window.close();
            }
        }

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

// Solo manejo eventos básicos fuera del menú (como cerrar la ventana)
void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

// Aquí es donde realmente ocurre el combate Pokémon
void Game::startPokemonBattle() {
    player.getPokemonActual().recibirDanio(-player.getPokemonActual().getVidaMaxima());
    salvaje.recibirDanio(-salvaje.getVidaMaxima());
    
    Combate combate(window, player.getPokemonActual(), salvaje);
    bool victoria = combate.iniciarCombate();
    battleResult = victoria;
    resultDisplayTime = 4.0f;
    
    resultText = sf::Text();
    resultText.setFont(menuFont);
    
    if (victoria) {
        resultText.setString("¡VICTORIA!");
        resultText.setFillColor(sf::Color::Green);
    } else {
        resultText.setString("¡DERROTA!");
        resultText.setFillColor(sf::Color::Red);
    }
    
    resultText.setCharacterSize(80);
    resultText.setOutlineColor(sf::Color::Black);
    resultText.setOutlineThickness(4);
    
    sf::FloatRect bounds = resultText.getLocalBounds();
    resultText.setOrigin(bounds.width/2, bounds.height/2);
    resultText.setPosition(window.getSize().x/2, window.getSize().y/3);
    
    window.clear();
    window.draw(background);
    
    sf::RectangleShape resultBackground(sf::Vector2f(
        bounds.width + 100, 
        bounds.height + 50
    ));
    resultBackground.setFillColor(sf::Color(0, 0, 0, 200));
    resultBackground.setOrigin(resultBackground.getSize().x/2, resultBackground.getSize().y/2);
    resultBackground.setPosition(resultText.getPosition());
    
    window.draw(resultBackground);
    window.draw(resultText);
    window.display();
    
    sf::Clock waitClock;
    while (waitClock.getElapsedTime().asSeconds() < 3.0f) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
        }
    }
}

// Actualizo el estado del juego, aquí detecto si se presiona espacio para iniciar el combate
void Game::update(float deltaTime) {
    if (resultDisplayTime > 0.0f) {
        resultDisplayTime -= deltaTime;
    }
    
    if (currentState == GameState::Playing) {
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

// Renderizo la ventana según el estado actual del juego
void Game::render() {
    window.clear();
    window.draw(background);
    
    if (currentState == GameState::MainMenu) {
        window.draw(titleText);
        window.draw(startButton);
        window.draw(exitButton);
        window.draw(startText);
        window.draw(exitText);
    } 
    else if (currentState == GameState::Playing) {
        window.draw(instructionText);
    }
    
    window.display();
}

// Bucle principal del juego
void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
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