#include "ComplexPlane.h"
#include <sstream>
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
    m_pixelSize.x = pixelWidth;
    m_pixelSize.y = pixelHeight;

    m_aspectRatio = static_cast<float>(pixelHeight) / pixelWidth;

    m_planeCenter = { 0, 0 };

    // Set initial values for BASE_WIDTH and BASE_HEIGHT
    const float BASE_WIDTH = 4.0f;
    const float BASE_HEIGHT = BASE_WIDTH * m_aspectRatio;

    m_planeSize = { BASE_WIDTH, BASE_HEIGHT };

    m_zoomCount = 0;
    m_state = State::CALCULATING;

    m_vArray.setPrimitiveType(sf::Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
    if (m_state == State::CALCULATING)
    {
        for (int i = 0; i < m_pixelSize.y; ++i)
        {
            for (int j = 0; j < m_pixelSize.x; ++j)
            {
                m_vArray[j + i * m_pixelSize.x].position = { static_cast<float>(j), static_cast<float>(i) };
                sf::Vector2f complexCoord = mapPixelToCoords(sf::Vector2i(j, i));
                int iterations = counterIterations(complexCoord);

                sf::Uint8 r, g, b;
                iterationsToRGB(iterations, r, g, b);

                m_vArray[j + i * m_pixelSize.x].color = { r, g, b };
            }
        }
        m_state = State::DISPLAYING;
    }
}

void ComplexPlane::zoomIn()
{
    ++m_zoomCount;

    float newSizeX = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float newSizeY = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);

    m_planeSize = { newSizeX, newSizeY };

    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut()
{
    --m_zoomCount;

    float newSizeX = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float newSizeY = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);

    m_planeSize = { newSizeX, newSizeY };
    m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(sf::Vector2i mousePixel)
{
    m_planeCenter = mapPixelToCoords(mousePixel);
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel)
{
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(sf::Text& text)
{
    stringstream ss;
    ss << "Mandelbrot Set\n";
    ss << "Center: (" << m_planeCenter.x << ", " << m_planeCenter.y << ")\n";
    ss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n";
    ss << "Left-click to Zoom in\n";
    ss << "Right-click to Zoom out";

    text.setString(ss.str());
}

int ComplexPlane::counterIterations(sf::Vector2f coord)
{
    std::complex<float> c(coord.x, coord.y);
    std::complex<float> z(0.0f, 0.0f);
    size_t count = 0;

    while (count < MAX_ITER)
    {
        z = z * z + c;

        if (std::abs(z) > 2.0f)
            break;

        count++;
    }

    return count;
}


void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b)
{
    const size_t MAX_ITER = 64; // Adjust based on your application
    const size_t regionCount = 5;

    // Ensure count is within valid range
    count = std::min(count, MAX_ITER - 1);

    // Map count to RGB values
    size_t regionSize = MAX_ITER / regionCount;
    size_t regionIndex = count / regionSize;

    switch (regionIndex)
    {
    case 0: // Deep red to brown
        r = static_cast<sf::Uint8>(100 + 155 * count / regionSize);
        g = static_cast<sf::Uint8>(50 + 205 * count / regionSize);
        b = 0;
        break;
    case 1: // Orange to deep red
        r = 255;
        g = static_cast<sf::Uint8>(50 + 205 * count / regionSize);
        b = 0;
        break;
    case 2: // Dark orange
        r = 200;
        g = static_cast<sf::Uint8>(100 + 155 * count / regionSize);
        b = 0;
        break;
    case 3: // Brown
        r = 139;
        g = 69;
        b = 19;
        break;
    case 4: // Dark brown
        r = 101;
        g = 67;
        b = 33;
        break;
    default:
        r = g = b = 0; // Handle unknown region
    }
}






sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel)
{
    float xMin = m_planeCenter.x - m_planeSize.x / 2.0f;
    float xMax = m_planeCenter.x + m_planeSize.x / 2.0f;
    float yMin = m_planeCenter.y - m_planeSize.y / 2.0f;
    float yMax = m_planeCenter.y + m_planeSize.y / 2.0f;

    float xCoord = ((mousePixel.x - 0) / static_cast<float>(m_pixelSize.x)) * (xMax - xMin) + xMin;
    float yCoord = ((mousePixel.y - 0) / static_cast<float>(m_pixelSize.y)) * (yMin - yMax) + yMax;

    return sf::Vector2f(xCoord, yCoord);
}

void ComplexPlane::setState(State newState)
{
    m_state = newState;
}

