#include <SFML/Graphics.hpp>
#include "./gbfile.cpp"
#include "iostream"

int main() {
    GBFile testFile("test-targets/tetris.gb");

    auto window = sf::RenderWindow{ { 1920u, 1080u }, testFile.getTitle() };
    window.setFramerateLimit(144);

    while (window.isOpen()) {
        for (auto event = sf::Event{}; window.pollEvent(event);) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
}