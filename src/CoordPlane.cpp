#include "CoordPlane.h"

// Implement member functions of CoordPlane that are declared in CoordPlane.h

double CoordPlane::getXSize() const {
    return xMax - xMin;
}

double CoordPlane::getYSize() const {
    return yMax - yMin;
}

double CoordPlane::getXCenter() const {
    return (xMax + xMin) / 2;
}

double CoordPlane::getYCenter() const {
    return (yMax + yMin) / 2;
}