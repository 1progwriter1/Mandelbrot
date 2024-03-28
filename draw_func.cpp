#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "draw_func.h"
#include <assert.h>

void ProccesKeyStrokes(size_t *offset_x, size_t *offset_y, float *scale) {

    assert(offset_x);
    assert(offset_y);
    assert(scale);

    sf::Event key_stroke_event;
    while (
}