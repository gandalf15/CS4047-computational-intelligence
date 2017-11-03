#include "fuzzifier.h"

double fuzify(double value, int *tuple) {
    double result = 0.0;
    if ( (value >= ((double)tuple[0] - (double)tuple[2])) && (value < (double)tuple[0]) ) {
        result = (value - (double)tuple[0] + (double)tuple[2])/(double)tuple[2];
    }
    else if ( (value <= ((double)tuple[1] + (double)tuple[3])) && (value > (double)tuple[1]) ) {
        result = ((double)tuple[1] + (double)tuple[3] - value)/(double)tuple[3];
    }
    else if (value >= (double)tuple[0] && value <= (double)tuple[1]) {
        result = 1.0;
    }
    return result;
}