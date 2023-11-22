#pragma once

#include <SFML/Graphics.hpp>

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum class State {
    CALCULATING,
    DISPLAYING
};

class ComplexPlane : public sf::Drawable {
public:
    ComplexPlane(unsigned int width, unsigned int height);

    void zoomIn(int x, int y);
    void zoomOut(int x, int y);
    void setMouseLocation(sf::Vector2i mousePixel);
    void updateRender();
    void loadText(sf::Text& text);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    State state;
    sf::VertexArray vertices;
    sf::Vector2i mouseLocation;
    sf::Vector2f planeCenter;
    sf::Vector2f planeSize;
    float aspectRatio;
    unsigned int screenWidth;
    unsigned int screenHeight;
    unsigned int zoomCount;

    sf::Vector2f mapPixelToCoords(sf::Vector2i mousePixel);
    size_t countIterations(sf::Vector2f coord);
    void iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b);
};