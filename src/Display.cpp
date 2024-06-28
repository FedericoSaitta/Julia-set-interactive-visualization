#include "Display.h"
#include "CoordPlane.h"

#include <iostream>
#include <cstdint>
#include <cstring>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "MandelBrot.h"

// Keyboard commands to interactively change the iteration count and save the image.
enum keyInputs{
    ADD10ITER = sf::Keyboard::Up,
    SUB10ITER = sf::Keyboard::Down,
    MUL2ITER = sf::Keyboard::Right,
    DIV2ITER = sf::Keyboard::Left,

    SAVE = sf::Keyboard::S
};


// HSV Colour pallette
static sf::Vector3f hsvToRgb(const float h, const float s, const float v) {
    float r{}, g{}, b{};

    int i = static_cast<int>(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
        default: break;
    }

    return {r * 255, g * 255, b * 255};
}



static void computeMandelBrotPixels(std::vector<std::uint8_t>& pixels, CoordPlane ComplexPlane, const int resolution, const int maxIter, const Complex juliaCoeff) {

    // the -1 ensures that the bounds are inclusive on both sides
    const double xOffset = ComplexPlane.getXSize() / (resolution - 1);
    const double yOffset = ComplexPlane.getYSize() / (resolution - 1);

    const double yMax = ComplexPlane.yMax;
    const double xMin = ComplexPlane.xMin;

    int index{};

    for (int i = 0; i < resolution; i++) {
        const double imag = yMax - yOffset * i;

        for (int j = 0; j < resolution; j++) {
            const double real = xMin + xOffset * j;
            const Complex z{ real, imag };

            const float phase = mandelBrotSmooth(z, maxIter, juliaCoeff);
            const sf::Vector3f col = hsvToRgb(phase / maxIter, 1.0, phase != maxIter? 1.0f : 0.0f);

            const std::uint8_t rgba[4]{
                static_cast<std::uint8_t>(col.x),
                static_cast<std::uint8_t>(col.y),
                static_cast<std::uint8_t>(col.z),
                255
            };

            // Inserting the colours in the pixel array
            std::memcpy(&pixels[index], rgba, 4);
            index += 4;
        }
    }
}


void displayMandelBrot(CoordPlane ComplexPlane, const int resolution, int maxIter, const Complex juliaCoeff) {
    sf::RenderWindow window(sf::VideoMode(resolution, resolution), "Mandelbrot Set");
    sf::Image image;
    image.create(resolution, resolution, sf::Color::Black);

    // Each pixel has data for [r0, g0, b0, alpha0, r1, b1, .... ]
    std::vector<std::uint8_t> pixels(resolution * resolution * 4); // Four times longer to hold r, g, b, a

    // Start the timer
    auto start = std::chrono::high_resolution_clock::now();

    computeMandelBrotPixels(pixels, ComplexPlane, resolution, maxIter, juliaCoeff);

    // End timer
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Duration: " << duration.count() << " milliseconds" << std::endl;

    sf::Texture texture;
    texture.create(resolution, resolution);
    texture.update(pixels.data());

    sf::Sprite sprite(texture);

    // Rectangle stuff
    sf::RectangleShape zoomRect;
    zoomRect.setFillColor(sf::Color(0, 0, 255, 128)); // Semi-transparent blue color
    zoomRect.setOutlineThickness(2.f);
    zoomRect.setOutlineColor(sf::Color::Blue);

    sf::Vector2f rectPosition;
    sf::Vector2f rectSize;

    bool isDrawing = false;
    sf::Vector2f startPos;
    // Finish rectangle stuff

    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            switch (event.type) {

            // KEYBOARD EVENTS //
            case sf::Event::KeyPressed:

                switch (event.key.code) {
                    case ADD10ITER:
                        maxIter += 10;
                        computeMandelBrotPixels(pixels, ComplexPlane, resolution, maxIter, juliaCoeff);
                        texture.update(pixels.data());

                        std::cout << "Updated MaxIter to " << maxIter << '\n';
                        break;

                    case SUB10ITER:
                        maxIter -= 10;
                        computeMandelBrotPixels(pixels, ComplexPlane, resolution, maxIter, juliaCoeff);
                        texture.update(pixels.data());

                        std::cout << "Updated MaxIter to " << maxIter << '\n';
                        break;

                    case MUL2ITER:
                        maxIter *= 2;
                        computeMandelBrotPixels(pixels, ComplexPlane, resolution, maxIter, juliaCoeff);
                        texture.update(pixels.data());

                        std::cout << "Updated MaxIter to " << maxIter << '\n';
                        break;

                    case DIV2ITER:
                        maxIter /= 2;
                        computeMandelBrotPixels(pixels, ComplexPlane, resolution, maxIter, juliaCoeff);
                        texture.update(pixels.data());

                        std::cout << "Updated MaxIter to " << maxIter << '\n';
                        break;


                    case SAVE: {
                        sf::Texture screenshotTexture;
                        screenshotTexture.create(window.getSize().x, window.getSize().y);
                        screenshotTexture.update(window);

                        sf::Image screenshotImage = screenshotTexture.copyToImage();

                        std::string filePath = "/Users/federicosaitta/CLionProjects/LearningCPP/Images/";
                        std::string center {'(' + std::to_string(ComplexPlane.getXCenter()) + ',' + std::to_string(ComplexPlane.getYCenter()) + ')'};
                        std::string iterations{std::to_string(maxIter)};
                        std::string fileName {center + iterations + "iter.png"};

                        if (!screenshotImage.saveToFile(filePath + fileName)) {
                            std::cerr << "Error saving screenshot to file" << '\n';
                        } else {
                            std::cout << "Screenshot saved to " << filePath << '\n';
                        }

                        break;
                    }
                    default:
                        break;
                    }
                break;

            // MOUSE EVENTS //
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    startPos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y); // NOLINT(*-narrowing-conversions)
                    rectPosition = startPos;
                    isDrawing = true;
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isDrawing = false;
                    rectSize = sf::Vector2f(event.mouseButton.x - startPos.x, event.mouseButton.y - startPos.y); // NOLINT(*-narrowing-conversions)

                    double xTransform = ComplexPlane.getXSize() / resolution;
                    double yTransform = ComplexPlane.getYSize() / resolution;

                    ComplexPlane.xMax -= (resolution - event.mouseButton.x) * xTransform;
                    ComplexPlane.xMin += startPos.x * xTransform;

                    ComplexPlane.yMin += (resolution - event.mouseButton.y) * yTransform;
                    ComplexPlane.yMax -= startPos.y * yTransform;

                    computeMandelBrotPixels(pixels, ComplexPlane, resolution, maxIter, juliaCoeff);
                    texture.update(pixels.data());
                    std::cout << "xWindow size is " << ComplexPlane.getXSize() << '\n';
                    std::cout << "yWindow size is " << ComplexPlane.getYSize()  << 'i' << '\n';
                }
                break;

            case sf::Event::MouseMoved:
                if (isDrawing) {
                    rectSize = sf::Vector2f(event.mouseMove.x - startPos.x, event.mouseMove.y - startPos.y); // NOLINT(*-narrowing-conversions)
                }
                break;

            case sf::Event::Closed:
                window.close();
                break;

            default:
                break;
            }
        }

        window.clear();
        window.draw(sprite);

        if (isDrawing) {
            zoomRect.setSize(sf::Vector2f(std::abs(rectSize.x), std::abs(rectSize.y)));
            zoomRect.setPosition(rectPosition);
            window.draw(zoomRect);
        }

        window.display();

    }
}
