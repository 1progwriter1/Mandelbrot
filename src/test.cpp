#include <immintrin.h>
#include <emmintrin.h>
#include <stdio.h>
#include <SFML/Graphics.hpp>

int main() {

    sf::Clock clock;

    printf("%f\n", clock);

    clock.getElapsedTime().asSeconds();

    printf("%f\n", clock);

    clock.restart().asSeconds();

    printf("%f\n", clock);

}