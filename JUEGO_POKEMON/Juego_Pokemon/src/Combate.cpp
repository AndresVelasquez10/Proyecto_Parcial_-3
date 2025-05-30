#include "Combate.hpp"
#include <iostream>
#include <cmath>

// Implementación de AnimacionAtaque
AnimacionAtaque::AnimacionAtaque(sf::Sprite& sprite, sf::Vector2f direccion, float duracion)
    : sprite(sprite), direccion(direccion), duracion(duracion), tiempo(0.0f),
      posicionInicial(sprite.getPosition()) {}

bool AnimacionAtaque::actualizar(float deltaTime) {
    tiempo += deltaTime;
    
    if (tiempo < duracion / 2.0f) {
        // Movimiento hacia adelante
        float progreso = tiempo / (duracion / 2.0f);
        sprite.setPosition(posicionInicial + direccion * progreso * 100.0f);
    } else if (tiempo < duracion) {
        // Movimiento de regreso
        float t = (tiempo - duracion / 2.0f) / (duracion / 2.0f);
        sprite.setPosition(posicionInicial + direccion * 100.0f * (1.0f - t));
    } else {
        sprite.setPosition(posicionInicial);
        return true; // Animación completada
    }
    return false;
}

float calcularEscala(const sf::Texture& textura, float maxAncho, float maxAlto);

// Constructor de Combate
Combate::Combate(sf::RenderWindow& ventana, Pokemon& jugador, Pokemon& oponente)
    : ventana(ventana), jugador(jugador), oponente(oponente),
      estadoActual(EstadoCombate::SeleccionAtaque),
      opcionSeleccionada(0), tiempoAnimacion(0.0f),
      animacionActual(nullptr) {
    
    // Cargar recursos
    if (!fuente.loadFromFile("assets/fonts/pokemon.ttf")) {
        std::cerr << "Error cargando fuente para combate\n";
        // Intentar cargar una fuente por defecto
        if (!fuente.loadFromFile("/usr/share/fonts/truetype/arial.ttf")) {
            std::cerr << "No se pudo cargar fuente alternativa\n";
        }
    }
    
    if (!texturaFondo.loadFromFile("assets/textures/combate/combate_bg.png")) {
        std::cerr << "Error cargando fondo de combate\n";
        // Crear fondo de emergencia
        texturaFondo.create(800, 600);
        sf::Image imgFondo;
        imgFondo.create(800, 600, sf::Color(30, 50, 70));  // Azul oscuro
        texturaFondo.update(imgFondo);
    }
    spriteFondo.setTexture(texturaFondo);
    
    // ESCALAR EL FONDO PARA QUE OCUPE TODA LA VENTANA
    sf::Vector2u windowSize = ventana.getSize();
    sf::Vector2u textureSize = texturaFondo.getSize();
    
    if (textureSize.x > 0 && textureSize.y > 0) {
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        spriteFondo.setScale(scaleX, scaleY);
    }

    // Cargar sprites de Pokémon
    std::string rutaPokemons = "assets/textures/pokemons/";
    if (!texturaPokemonJugador.loadFromFile(rutaPokemons + jugador.getNombre() + ".png")) {
        std::cerr << "Error cargando sprite de " << jugador.getNombre() << "\n";
        // Crear sprite de emergencia
        texturaPokemonJugador.create(64, 64);
        sf::Image img;
        img.create(64, 64, sf::Color::Blue);
        texturaPokemonJugador.update(img);
    } else {
        spritePokemonJugador.setTexture(texturaPokemonJugador);
        
        // Centrar origen
        sf::FloatRect bounds = spritePokemonJugador.getLocalBounds();
        spritePokemonJugador.setOrigin(bounds.width/2, bounds.height/2);
        
        // Calcular escala automática
        float escala = calcularEscala(texturaPokemonJugador, 150.0f, 2150.0f);
        spritePokemonJugador.setScale(escala, escala);
        
        // Posicionar
        spritePokemonJugador.setPosition(
            ventana.getSize().x * 0.75f,
            ventana.getSize().y * 0.5f
        );
    }

    float maxAncho = 150.0f; // Máximo ancho deseado
    float maxAlto = 150.0f;  // Máximo alto deseado

    // Para jugador
    float escalaJugador = calcularEscala(texturaPokemonJugador, maxAncho, maxAlto);
    spritePokemonJugador.setScale(escalaJugador, escalaJugador);

    // Para oponente
    float escalaOponente = calcularEscala(texturaPokemonOponente, maxAncho, maxAlto);
    spritePokemonOponente.setScale(escalaOponente, escalaOponente);


    if (!texturaPokemonOponente.loadFromFile(rutaPokemons + oponente.getNombre() + ".png")) {
        std::cerr << "Error cargando sprite de " << oponente.getNombre() << "\n";
        // Crear sprite de emergencia
        texturaPokemonOponente.create(64, 64);
        sf::Image img;
        img.create(64, 64, sf::Color::Red);
        texturaPokemonOponente.update(img);
    } else {
        spritePokemonOponente.setTexture(texturaPokemonOponente);
        
        // Centrar origen
        sf::FloatRect bounds = spritePokemonOponente.getLocalBounds();
        spritePokemonOponente.setOrigin(bounds.width/2, bounds.height/2);
        
        // Calcular escala automática
        float escala = calcularEscala(texturaPokemonOponente, 200.0f, 200.0f);
        spritePokemonOponente.setScale(escala, escala);
        
        // Posicionar
        spritePokemonOponente.setPosition(
            ventana.getSize().x * 0.3f,
            ventana.getSize().y * 0.5f
        );
    }


    spritePokemonOponente.setTexture(texturaPokemonOponente);
    
    // Cargar sonidos
    if (!bufferSeleccion.loadFromFile("assets/sounds/attack.mp3")) {
        std::cerr << "Error cargando sonido de selección\n";
    }
    sonidoSeleccion.setBuffer(bufferSeleccion);

    if (!bufferAtaque.loadFromFile("assets/sounds/attack.mp3")) {
        std::cerr << "Error cargando sonido de ataque\n";
    }
    sonidoAtaque.setBuffer(bufferAtaque);
    
    // Configurar UI
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
    
    // Configurar opciones de ataque
    const auto& ataques = jugador.getAtaques();
    for (size_t i = 0; i < ataques.size(); ++i) {
        sf::Text opcion(ataques[i].nombre, fuente, 20);
        opcion.setPosition(50, 400 + i * 40);
        opcionesAtaque.push_back(opcion);
    }
    
    // Resaltar opción seleccionada
    if (!opcionesAtaque.empty()) {
        opcionesAtaque[0].setFillColor(sf::Color::Yellow);
    }
    
    // Panel inferior
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

void Combate::dibujarBarraVida(float x, float y, float ancho, float alto, float porcentaje) {
    // Asegurar que el porcentaje esté entre 0 y 1
    porcentaje = std::max(0.0f, std::min(1.0f, porcentaje));
    
    // Dibujar fondo de la barra (gris)
    sf::RectangleShape fondo(sf::Vector2f(ancho, alto));
    fondo.setPosition(x, y);
    fondo.setFillColor(sf::Color(100, 100, 100));
    fondo.setOutlineThickness(1.0f);
    fondo.setOutlineColor(sf::Color::Black);
    ventana.draw(fondo);
    
    // Dibujar parte de vida (color dinámico)
    sf::RectangleShape vida(sf::Vector2f(ancho * porcentaje, alto));
    vida.setPosition(x, y);
    
    // Cambiar color según el porcentaje
    if (porcentaje > 0.7) {
        vida.setFillColor(sf::Color(0, 200, 0));    // Verde
    } else if (porcentaje > 0.3) {
        vida.setFillColor(sf::Color(255, 200, 0));  // Amarillo
    } else {
        vida.setFillColor(sf::Color(220, 0, 0));    // Rojo
    }
    
    ventana.draw(vida);
    
    // Dibujar texto con el porcentaje
    sf::Text texto;
    texto.setFont(fuente);
    texto.setString(std::to_string(static_cast<int>(porcentaje * 100)) + "%");
    texto.setCharacterSize(12);
    texto.setFillColor(sf::Color::White);
    texto.setPosition(x + ancho + 5, y);
    ventana.draw(texto);
}

void Combate::dibujarInterfaz() {
    ventana.clear();
    ventana.draw(spriteFondo);
    
    // Dibujar sprites de Pokémon
    ventana.draw(spritePokemonOponente);
    ventana.draw(spritePokemonJugador);
    
    // Dibujar barras de vida
    dibujarBarraVida(100, 130, 200, 20, 
        static_cast<float>(oponente.getVidaActual()) / oponente.getVidaMaxima());
    dibujarBarraVida(500, 430, 200, 20, 
        static_cast<float>(jugador.getVidaActual()) / jugador.getVidaMaxima());
    
    // Dibujar nombres
    ventana.draw(textoJugador);
    ventana.draw(textoOponente);
    
    // Dibujar panel inferior
    ventana.draw(panelInferior);
    
    // Dibujar opciones de ataque
    for (const auto& opcion : opcionesAtaque) {
        ventana.draw(opcion);
    }
    
    ventana.display();
}

void Combate::procesarEventos() {
    sf::Event event;
    while (ventana.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            ventana.close();
        }
        
        if (estadoActual == EstadoCombate::SeleccionAtaque) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    // Cambiar selección hacia arriba
                    sonidoSeleccion.play();
                    opcionesAtaque[opcionSeleccionada].setFillColor(sf::Color::White);
                    opcionSeleccionada = (opcionSeleccionada - 1 + opcionesAtaque.size()) % opcionesAtaque.size();
                    opcionesAtaque[opcionSeleccionada].setFillColor(sf::Color::Yellow);
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    // Cambiar selección hacia abajo
                    sonidoSeleccion.play();
                    opcionesAtaque[opcionSeleccionada].setFillColor(sf::Color::White);
                    opcionSeleccionada = (opcionSeleccionada + 1) % opcionesAtaque.size();
                    opcionesAtaque[opcionSeleccionada].setFillColor(sf::Color::Yellow);
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    // Seleccionar ataque
                    sonidoAtaque.play();
                    estadoActual = EstadoCombate::EjecutandoAtaqueJugador;
                    tiempoAnimacion = 1.0f; // 1 segundo de animación
                    
                    // Iniciar animación
                    animacionActual = std::make_unique<AnimacionAtaque>(
                        spritePokemonJugador, 
                        sf::Vector2f(-1.0f, 0.0f)  // Movimiento hacia la izquierda (hacia el oponente)
                    );
                    
                    // Aplicar daño
                    oponente.recibirDanio(jugador.getAtaques()[opcionSeleccionada].danio);
                }
            }
        }
    }
}

void Combate::actualizar(float deltaTime) {
    // Actualizar animación si existe
    if (animacionActual && animacionActual->actualizar(deltaTime)) {
        animacionActual.reset(); // Finalizar animación
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
                    
                    // Iniciar animación para el oponente
                    animacionActual = std::make_unique<AnimacionAtaque>(
                        spritePokemonOponente, 
                        sf::Vector2f(1.0f, 0.0f)  // Movimiento hacia la derecha (hacia el jugador)
                    );
                    
                    // Ataque del oponente
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
            // Aquí mostraríamos el resultado del combate
            break;
            
        default:
            break;
    }
}

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