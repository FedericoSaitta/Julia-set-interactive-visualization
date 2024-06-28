#include "MandelBrot.h"
#include "CoordPlane.h"

#include <thread>

// To avoid new std::complex object creation we perform calculatations on juliaCoeff renamed as z
float mandelBrotSmooth(const Complex num, const int maxIter, const Complex z) {

    double re {z.real};
    double im {z.imag};

    int iterCount{};

    double reSquared = re * re;
    double imSquared = im * im;

    while ((reSquared + imSquared) <= 4.0 && iterCount < maxIter) {
        im = 2 * re * im + num.imag;
        re = reSquared - imSquared + num.real;

        reSquared = re * re;
        imSquared = im * im;

        iterCount++;
    }

    if (iterCount == maxIter)
        return maxIter;

    return iterCount + 1 - std::log( std::log2(reSquared + imSquared) );
}