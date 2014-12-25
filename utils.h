#ifndef UTILS_H
#define UTILS_H

inline float linear_interpolation(float x1, float x2, float y1, float y2, float x)
{
    float denom = x2 - x1;
    if(denom == 0)
        return y1; // should never happen
    // calculate decimal position of x;
    float dx = (x - x1)/(x2 - x1);
    // use weighted sum method of interpolating
    float result = dx*y2 + (1-dx)*y1;
    return result;
}

inline double scaling(double old_min, double old_max, double new_min, double new_max, double old_value)
{
    return (new_max - new_min) / (old_max - old_min) * (old_value - old_min) + new_min;
}

#endif // UTILS_H
