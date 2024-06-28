#ifndef COORDPLANE_H
#define COORDPLANE_H

#include <utility>
// doing it this way as I do not have to worry about rectangle centres and fix when the centre is a .5 pixel
struct CoordPlane {
    double xMin;
    double xMax;

    double yMin;
    double yMax;

    [[nodiscard]] double getXSize() const;
    [[nodiscard]] double getYSize() const;
    [[nodiscard]] double getXCenter() const;
    [[nodiscard]] double getYCenter() const;

};


struct Complex {

    double real;
    double imag;

};

#endif //COORDPLANE_H
