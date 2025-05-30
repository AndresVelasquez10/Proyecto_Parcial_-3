#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class TileMap : public sf::Drawable {
private:
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    std::vector<std::vector<int>> mapa;
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
public:
    bool load(const std::string& tileset, sf::Vector2u tileSize, 
              const std::vector<std::vector<int>>& mapa, unsigned int width, unsigned int height);
              
    bool isCollision(sf::FloatRect bounds) const;
};