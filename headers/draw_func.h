#ifndef DRAW_FUNCTIONS_MANDELBROT
#define DRAW_FUNCTIONS_MANDELBROT

#include <stdio.h>
#include "mandelbrot_data.h"

void SetWindowData(WindowData *data);
bool IfMeasure(const int argc, const char *argv[]);
void ProceedKeyStrokes(sf::RenderWindow &window, WindowData *data);
int MeasureProgram(WindowData *data, const char *time_file, const char *ticks_file, void (*SetPixels)(sf::VertexArray &, WindowData *, bool));

#endif