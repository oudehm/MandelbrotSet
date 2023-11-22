#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"

int main() {
    // Get desktop resolution
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    unsigned int screenWidth = desktopMode.width / 2;
    unsigned int screenHeight = desktopMode.height / 2;

    // Construct the RenderWindow
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Mandelbrot Set");

    // Construct the ComplexPlane
    ComplexPlane complexPlane(screenWidth, screenHeight);

    // Construct the Font and Text objects
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Handle font loading error
        return -1;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);

    // Main loop
    while (window.isOpen()) {
        // Handle Input
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Right) {
                    complexPlane.zoomOut(event.mouseButton.x, event.mouseButton.y);
                }
                else if (event.mouseButton.button == sf::Mouse::Left) {
                    complexPlane.zoomIn(event.mouseButton.x, event.mouseButton.y);
                }
            }
            else if (event.type == sf::Event::MouseMoved) {
                complexPlane.setMouseLocation(event.mouseMove.x, event.mouseMove.y);
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        // Update Scene
        complexPlane.updateRender();
        complexPlane.loadText(text);

        // Draw Scene
        window.clear();
        window.draw(complexPlane);
        window.draw(text);
        window.display();
    }

    return 0;
}
