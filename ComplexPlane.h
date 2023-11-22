#include <SFML/Graphics.hpp>
#include <complex>

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum class State {
    CALCULATING,
    DISPLAYING
};

class ComplexPlane {
public:
    // Constructors
    ComplexPlane(sf::RenderWindow& window);
    ComplexPlane(sf::RenderWindow& window, float centerX, float centerY, float width, float height);

    // Destructor
    ~ComplexPlane();

    // Setters
    void setCenter(float centerX, float centerY);
    void setWidth(float width);
    void setHeight(float height);
    void setState(State state);

    // Getters
    float getCenterX() const;
    float getCenterY() const;
    float getWidth() const;
    float getHeight() const;
    State getState() const;

    // SFML specific functions
    void draw();
    void handleEvent(sf::Event& event);

private:
    sf::RenderWindow& window;
    float centerX;
    float centerY;
    float width;
    float height;
    State state;

    // Other private members as needed for SFML integration
};
