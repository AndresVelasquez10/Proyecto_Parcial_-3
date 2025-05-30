# Compilo el proyecto usando g++ y SFML
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC_DIR = src
BIN_DIR = bin

# Acá pongo todos los archivos fuente del proyecto
SOURCES = $(SRC_DIR)/Ataque.cpp \
          $(SRC_DIR)/Combate.cpp \
          $(SRC_DIR)/Game.cpp \
          $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/Player.cpp \
          $(SRC_DIR)/Pokemon.cpp

# Transformo los .cpp en .o para la compilación
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)
EXECUTABLE = $(BIN_DIR)/pokemon_game

# Acá se compila todo y se genera el ejecutable principal
all: $(EXECUTABLE)

# Enlazo todos los objetos y genero el binario final
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compilo cada archivo fuente por separado y guardo los .o en bin/
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Si quiero limpiar todo lo compilado, uso esto
clean:
	rm -rf $(BIN_DIR)/*

.PHONY: all clean