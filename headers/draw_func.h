#ifndef DRAW_FUNCTIONS_MANDELBROT
#define DRAW_FUNCTIONS_MANDELBROT

#include <stdio.h>
#include "mandelbrot_data.h"

void SetWindowData(WindowData *data);
bool IfMeasure(const int argc, const char *argv[]);
void ProceedKeyStrokes(sf::RenderWindow &window, WindowData *data);
float GetFps(WindowData *data);
void DrawWindow(sf::RenderWindow &window, sf::VertexArray &pixels, WindowData *data);

#endif