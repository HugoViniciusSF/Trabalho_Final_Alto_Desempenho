#include <math.h>
#include "math_utils.h"

double euclidean_distance(const double *p1, const double *p2, int num_features) {
    double sum = 0.0;
    for (int i = 0; i < num_features; i++) {
        double diff = p1[i] - p2[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}
