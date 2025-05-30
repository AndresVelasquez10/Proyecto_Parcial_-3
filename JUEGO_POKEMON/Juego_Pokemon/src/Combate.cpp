#include "Combate.hpp"
#include <iostream>
#include <cmath>

// Animación visual de ataque, la uso para dar feedback cuando un Pokémon ataca
AnimacionAtaque::AnimacionAtaque(sf::Sprite& sprite, sf::Vector2f direccion, float duracion)
    : sprite(sprite), direccion(direccion), duracion(duracion), tiempo(0.0f),
      posicionInicial(sprite.getPosition()) {}

bool AnimacionAtaque::actualizar(float deltaTime) {
    tiempo += deltaTime;
    
    if (tiempo < duracion / 2.0f) {
        float progreso = tiempo / (duracion / 2.0f);
        sprite.setPosition(posicionInicial + direccion * progreso * 100.0f);
    } else if (tiempo < duracion) {
        float t = (tiempo - duracion / 2.0f) / (duracion / 2.0f);
        sprite.setPosition(posicionInicial + direccion * 100.0f * (1.0f - t));
    } else {
        sprite.setPosition(posicionInicial);
        return true;
    }
    return false;
}

float calcularEscala(const sf::Texture& textura, float maxAncho, float maxAlto);

// Acá armo todo el escenario y los recursos para el combate Pokémon
Combate::Combate(sf::RenderWindow& ventana, Pokemon& jugador, Pokemon& oponente)
    : ventana(ventana), jugador(jugador), oponente(oponente),
      estadoActual(EstadoCombate::SeleccionAtaque),
      opcionSeleccionada(0), tiempoAnimacion(0.0f),
      animacionActual(nullptr) {
    
    if (!fuente.loadFromFile("assets/fonts/pokemon.ttf")) {
        std::cerr << "Error cargando fuente para combate\n";
        if (!fuente.loadFromFile("/usr/share/fonts/truetype/arial.ttf")) {
            std::cerr << "No se pudo cargar fuente alternativa\n";
        }
    }
    
    if (!texturaFondo.loadFromFile("assets/textures/combate/combate_bg.png")) {
        std::cerr << "Error cargando fondo de combate\n";
        texturaFondo.create(800, 600);
        sf::Image imgFondo;
        imgFondo.create(800, 600, sf::Color(30, 50, 70));
        texturaFondo.update(imgFondo);
    }
    spriteFondo.setTexture(texturaFondo);
    
    sf::Vector2u windowSize = ventana.getSize();
    sf::Vector2u textureSize = texturaFondo.getSize();
    
    if (textureSize.x > 0 && textureSize.y > 0) {
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        spriteFondo.setScale(scaleX, scaleY);
    }

    std::string rutaPokemons = "assets/textures/pokemons/";
    if (!texturaPokemonJugador.loadFromFile(rutaPokemons + jugador.getNombre() + ".png")) {
        std::cerr << "Error cargando sprite de " << jugador.getNombre() << "\n";
        texturaPokemonJugador.create(64, 64);
        sf::Image img;
        img.create(64, 64, sf::Color::Blue);
        texturaPokemonJugador.update(img);
    } else {
        spritePokemonJugador.setTexture(texturaPokemonJugador);
        sf::FloatRect bounds = spritePokemonJugador.getLocalBounds();
        spritePokemonJugador.setOrigin(bounds.width/2, bounds.height/2);
        float escala = calcularEscala(texturaPokemonJugador, 150.0f, 2150.0f);
        spritePokemonJugador.setScale(escala, escala);
        spritePokemonJugador.setPosition(
            ventana.getSize().x * 0.75f,
            ventana.getSize().y * 0.5f
        );
    }

    float maxAncho = 150.0f;
    float maxAlto = 150.0f;

    float escalaJugador = calcularEscala(texturaPokemonJugador, maxAncho, maxAlto);
    spritePokemonJugador.setScale(escalaJugador, escalaJugador);

    float escalaOponente = calcularEscala(texturaPokemonOponente, maxAncho, maxAlto);
    spritePokemonOponente.setScale(escalaOponente, escalaOponente);

    if (!texturaPokemonOponente.loadFromFile(rutaPokemons + oponente.getNombre() + ".png")) {
        std::cerr << "Error cargando sprite de " << oponente.getNombre() << "\n";
        texturaPokemonOponente.create(64, 64);
        sf::Image img;
        img.create(64, 64, sf::Color::Red);
        texturaPokemonOponente.update(img);
    } else {
        spritePokemonOponente.setTexture(texturaPokemonOponente);
        sf::FloatRect bounds = spritePokemonOponente.getLocalBounds();
        spritePokemonOponente.setOrigin(bounds.width/2, bounds.height/2);
        float escala = calcularEscala(texturaPokemonOponente, 200.0f, 200.0f);
        spritePokemonOponente.setScale(escala, escala);
        spritePokemonOponente.setPosition(
            ventana.getSize().x * 0.3f,
            ventana.getSize().y * 0.5f
        );
    }

    spritePokemonOponente.setTexture(texturaPokemonOponente);
    
    // Acá cargo los sonidos del combate, si falla lo aviso
    if (!bufferSeleccion.loadFromFile("assets/sounds/menu_select.mp3")) {
        std::cerr << "Error cargando sonido de selección\n";
    }
    sonidoSeleccion.setBuffer(bufferSeleccion);

    if (!bufferAtaque.loadFromFile("assets/sounds/attack.mp3")) {
        std::cerr << "Error cargando sonido de ataque\n";
    }
    sonidoAtaque.setBuffer(bufferAtaque);
    
    // Configuro los textos de la interfaz de combate
    textoJugador.setFont(fuente);
    textoJugador.setString(jugador.getNombre());
    textoJugador.setCharacterSize(24);
    textoJugador.setPosition(500, 400);
    textoJugador.setFillColor(sf::Color::White);
    
    textoOponente.setFont(fuente);
    textoOponente.setString(oponente.getNombre());
    textoOponente.setCharacterSize(24);
    textoOponente.setPosition(100, 100);
    textoOponente.setFillColor(sf::Color::White);
    
    const auto& ataques = jugador.getAtaques();
    for (size_t i = 0; i < ataques.size(); ++i) {
        sf::Text opcion(ataques[i].nombre, fuente, 20);
        opcion.setPosition(50, 400 + i * 40);
        opcionesAtaque.push_back(opcion);
    }
    
    if (!opcionesAtaque.empty()) {
        opcionesAtaque[0].setFillColor(sf::Color::Yellow);
    }
    
    panelInferior.setSize(sf::Vector2f(800, 150));
    panelInferior.setPosition(0, 450);
    panelInferior.setFillColor(sf::Color(0, 0, 0, 180));
}

float calcularEscala(const sf::Texture& textura, float maxAncho, float maxAlto) {
    sf::Vector2u size = textura.getSize();
    float escalaX = maxAncho / size.x;
    float escalaY = maxAlto / size.y;
    return std::min(escalaX, escalaY);
}

// Acá dibujo la barra de vida de cada Pokémon, con color dinámico según la vida
void Combate::dibujarBarraVida(float x, float y, float ancho, float alto, float porcentaje) {
    porcentaje = std::max(0.0f, std::min(1.0f, porcentaje));
    sf::RectangleShape fondo(sf::Vector2f(ancho, alto));
    fondo.setPosition(x, y);
    fondo.setFillColor(sf::Color(100, 100, 100));
    fondo.setOutlineThickness(1.0f);
    fondo.setOutlineColor(sf::Color::Black);
    ventana.draw(fondo);
    
    sf::RectangleShape vida(sf::Vector2f(ancho * porcentaje, alto));
    vida.setPosition(x, y);
    if (porcentaje > 0.7) {
        vida.setFillColor(sf::Color(0, 200, 0));
    } else if (porcentaje > 0.3) {
        vida.setFillColor(sf::Color(255, 200, 0));
    } else {
        vida.setFillColor(sf::Color(220, 0, 0));
    }
    ventana.draw(vida);
    
    sf::Text texto;
    texto.setFont(fuente);
    texto.setString(std::to_string(static_cast<int>(porcentaje * 100)) + "%");
    texto.setCharacterSize(12);
    texto.setFillColor(sf::Color::White);
    texto.setPosition(x + ancho + 5, y);
    ventana.draw(texto);
}

// Dibujo toda la interfaz del combate en cada frame
void Combate::dibujarInterfaz() {
    ventana.clear();
    ventana.draw(spriteFondo);
    ventana.draw(spritePokemonOponente);
    ventana.draw(spritePokemonJugador);
    dibujarBarraVida(100, 130, 200, 20, 
        static_cast<float>(oponente.getVidaActual()) / oponente.getVidaMaxima());
    dibujarBarraVida(500, 430, 200, 20, 
        static_cast<float>(jugador.getVidaActual()) / jugador.getVidaMaxima());
    ventana.draw(textoJugador);
    ventana.draw(textoOponente);
    ventana.draw(panelInferior);
    for (const auto& opcion : opcionesAtaque) {
        ventana.draw(opcion);
    }
    ventana.display();
}

// Acá proceso los eventos del usuario durante el combate
void Combate::procesarEventos() {
    sf::Event event;
    while (ventana.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            ventana.close();
        }
        
        if (estadoActual == EstadoCombate::SeleccionAtaque) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    sonidoSeleccion.play();
                    opcionesAtaque[opcionSeleccionada].setFillColor(sf::Color::White);
                    opcionSeleccionada = (opcionSeleccionada - 1 + opcionesAtaque.size()) % opcionesAtaque.size();
                    opcionesAtaque[opcionSeleccionada].setFillColor(sf::Color::Yellow);
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    sonidoSeleccion.play();
                    opcionesAtaque[opcionSeleccionada].setFillColor(sf::Color::White);
                    opcionSeleccionada = (opcionSeleccionada + 1) % opcionesAtaque.size();
                    opcionesAtaque[opcionSeleccionada].setFillColor(sf::Color::Yellow);
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    sonidoAtaque.play();
                    estadoActual = EstadoCombate::EjecutandoAtaqueJugador;
                    tiempoAnimacion = 1.0f;
                    animacionActual = std::make_unique<AnimacionAtaque>(
                        spritePokemonJugador, 
                        sf::Vector2f(-1.0f, 0.0f)
                    );
                    oponente.recibirDanio(jugador.getAtaques()[opcionSeleccionada].danio);
                }
            }
        }
    }
}

// Acá actualizo el estado del combate y las animaciones
void Combate::actualizar(float deltaTime) {
    if (animacionActual && animacionActual->actualizar(deltaTime)) {
        animacionActual.reset();
    }
    
    switch (estadoActual) {
        case EstadoCombate::EjecutandoAtaqueJugador:
            tiempoAnimacion -= deltaTime;
            if (tiempoAnimacion <= 0) {
                if (oponente.estaDerrotado()) {
                    estadoActual = EstadoCombate::Resultado;
                } else {
                    estadoActual = EstadoCombate::EjecutandoAtaqueOponente;
                    tiempoAnimacion = 1.0f;
                    animacionActual = std::make_unique<AnimacionAtaque>(
                        spritePokemonOponente, 
                        sf::Vector2f(1.0f, 0.0f)
                    );
                    int ataqueIndex = rand() % oponente.getAtaques().size();
                    jugador.recibirDanio(oponente.getAtaques()[ataqueIndex].danio);
                }
            }
            break;
            
        case EstadoCombate::EjecutandoAtaqueOponente:
            tiempoAnimacion -= deltaTime;
            if (tiempoAnimacion <= 0) {
                if (jugador.estaDerrotado()) {
                    estadoActual = EstadoCombate::Resultado;
                } else {
                    estadoActual = EstadoCombate::SeleccionAtaque;
                }
            }
            break;
            
        case EstadoCombate::Resultado:
            // Acá termina el combate, el resultado se muestra afuera
            break;
            
        default:
            break;
    }
}

// Este método corre el bucle principal del combate hasta que haya un ganador
bool Combate::iniciarCombate() {
    sf::Clock clock;
    
    while (ventana.isOpen() && estadoActual != EstadoCombate::Resultado) {
        float deltaTime = clock.restart().asSeconds();
        procesarEventos();
        actualizar(deltaTime);
        dibujarInterfaz();
    }
    
    return !jugador.estaDerrotado();
}