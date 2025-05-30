#include "TileMap.hpp"

bool TileMap::load(const std::string& tileset, sf::Vector2u tileSize, 
                  const std::vector<std::vector<int>>& mapa, unsigned int width, unsigned int height) {
    this->mapa = mapa;
    
    if (!m_tileset.loadFromFile(tileset))
        return false;

    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    for (unsigned int i = 0; i < width; ++i) {
        for (unsigned int j = 0; j < height; ++j) {
            int tileNumber = mapa[j][i];
            if (tileNumber == 0) continue;  // 0 = vacío
            
            int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }
    }
    
    return true;
}

bool TileMap::isCollision(sf::FloatRect bounds) const {
    // Convertir posición del jugador a coordenadas de tile
    int tileX = static_cast<int>(bounds.left / 32);
    int tileY = static_cast<int>(bounds.top / 32);
    
    // Verificar colisión en el mapa
    if (tileX >= 0 && tileX < static_cast<int>(mapa[0].size()) &&
        tileY >= 0 && tileY < static_cast<int>(mapa.size())) {
        return mapa[tileY][tileX] > 0;  // Tile sólido
    }
    
    return true;  // Fuera de límites = colisión
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &m_tileset;
    target.draw(m_vertices, states);
}