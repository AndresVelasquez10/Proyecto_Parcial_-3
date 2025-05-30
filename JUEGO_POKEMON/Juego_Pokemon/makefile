CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC_DIR = src
BIN_DIR = bin

# Lista CORRECTA de archivos fuente
SOURCES = $(SRC_DIR)/Ataque.cpp \
          $(SRC_DIR)/Combate.cpp \
          $(SRC_DIR)/Game.cpp \
          $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/Player.cpp \
          $(SRC_DIR)/Pokemon.cpp

# Generar lista de objetos
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)
EXECUTABLE = $(BIN_DIR)/pokemon_game

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)  # Crear directorio si no existe
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)/*

.PHONY: all clean