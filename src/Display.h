#ifndef DISPLAY_H
#define DISPLAY_H

#include <SFML/Graphics.hpp>

#include "CoordPlane.h"

void displayMandelBrot(CoordPlane ComplexPlane, int resolution, int maxIter, Complex juliaCoeff);

#endif //DISPLAY_H
