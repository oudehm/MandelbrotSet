#include "ComplexPlane.h"
#include <sstream>
#include <cmath>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

using namespace std;

ComplexPlane::ComplexPlane(unsigned int pixelWidth, unsigned int pixelHeight)
    : screenWidth(pixelWidth), screenHeight(pixelHeight), state(State::CALCULATING), zoomCount(0)
{
    // Calculate aspect ratio
    aspectRatio = static_cast<float>(pixelHeight) / static_cast<float>(pixelWidth);

    // Initialize other member variables
    planeCenter = { 0.0f, 0.0f };
    planeSize = { BASE_WIDTH, BASE_HEIGHT * aspectRatio };

    // Initialize VertexArray
    vertices.setPrimitiveType(sf::Points);
    vertices.resize(screenWidth * screenHeight);

    // Initialize state
    state = State::CALCULATING;
}

void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(vertices);
}

void ComplexPlane::updateRender() {
    if (state == State::CALCULATING) {
        for (int i = 0; i < screenHeight; ++i) {
            for (int j = 0; j < screenWidth; ++j) {
                sf::Vector2f position(static_cast<float>(j), static_cast<float>(i));
                vertices[j + i * screenWidth].position = sf::Vector2i(static_cast<int>(position.x), static_cast<int>(position.y));

                sf::Vector2f coord = mapPixelToCoords(position);
                size_t iterations = countIterations(coord);

                sf::Uint8 r, g, b;
                iterationsToRGB(iterations, r, g, b);

                vertices[j + i * screenWidth].color = sf::Color(r, g, b);
            }
        }

        state = State::DISPLAYING;
    }
}

void ComplexPlane::zoomIn(int x, int y) {
    zoomCount++;
    float xSize = BASE_WIDTH * pow(BASE_ZOOM, static_cast<float>(zoomCount));
    float ySize = BASE_HEIGHT * aspectRatio * pow(BASE_ZOOM, static_cast<float>(zoomCount));
    planeSize = { xSize, ySize };
    state = State::CALCULATING;
}

void ComplexPlane::zoomOut(int x, int y) {
    if (zoomCount > 0) {
        zoomCount--;
        float xSize = BASE_WIDTH * pow(BASE_ZOOM, static_cast<float>(zoomCount));
        float ySize = BASE_HEIGHT * aspectRatio * pow(BASE_ZOOM, static_cast<float>(zoomCount));
        planeSize = { xSize, ySize };
        state = State::CALCULATING;
    }
}

void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel) {
    mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(sf::Text& text) {
    stringstream ss;
    ss << "Center: (" << planeCenter.x << ", " << planeCenter.y << ")\n";
    ss << "Mouse: (" << mouseLocation.x << ", " << mouseLocation.y << ")\n";
    text.setString(ss.str());
}

size_t ComplexPlane::countIterations(sf::Vector2f coord) {
    sf::Vector2f z = coord;
    size_t iterations = 0;

    while (iterations < MAX_ITER) {
        float x = z.x * z.x - z.y * z.y + coord.x;
        float y = 2.0f * z.x * z.y + coord.y;

        z = sf::Vector2f(x, y);

        if (abs(z) > 2.0) {
            break;
        }

        iterations++;
    }

    return iterations;
}

void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) {
    if (count == MAX_ITER) {
        r = g = b = 0;
    }
    else {
        float t = static_cast<float>(count) / static_cast<float>(MAX_ITER);
        r = static_cast<sf::Uint8>(255 * (1.0 - t));
        g = 0;
        b = static_cast<sf::Uint8>(255 * t);
    }
}

sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel) {
    float x = (static_cast<float>(mousePixel.x) / static_cast<float>(screenWidth)) * planeSize.x
        + (planeCenter.x - planeSize.x / 2.0f);

    float y = (static_cast<float>(mousePixel.y) / static_cast<float>(screenHeight)) * planeSize.y
        + (planeCenter.y - planeSize.y / 2.0f);

    return sf::Vector2f(x, y);
}

