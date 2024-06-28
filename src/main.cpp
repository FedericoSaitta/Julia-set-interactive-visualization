#include "Display.h"
#include "CoordPlane.h"

/* GUI COMMANDS:

UP add 10 to maxIter
DOWN subtract 10 to maxIter
RIGHT times maxIter by 2
LEFT divide maxIter by 2
MouseLeft to draw zoom rectangle
S Save current window as .png


 */


int main() {

    // Min and Max bounds of the Complex plane for the Mandelbrot set
    constexpr double xMin{-2.0};
    constexpr double xMax{2};

    constexpr double yMin{-2};
    constexpr double yMax{2};
    constexpr CoordPlane ComplexPlane{xMin, xMax, yMin, yMax};

    // The display is square and has resolution number of pixels
    constexpr int resolution{350};

    constexpr int maxIter{80};
    constexpr Complex juliaCoeff{0, 0};

    displayMandelBrot(ComplexPlane, resolution, maxIter, juliaCoeff);
}